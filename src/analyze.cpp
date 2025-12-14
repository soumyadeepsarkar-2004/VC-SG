#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>
#include "image_utils.hpp"
#include "vcs.hpp"
#include "rg.hpp"
#include "dhcod.hpp"

// Calculate Mean Squared Error
double calculateMSE(const Image &img1, const Image &img2)
{
    if (img1.width != img2.width || img1.height != img2.height)
    {
        std::cerr << "Error: Image dimensions don't match!" << std::endl;
        return -1.0;
    }

    double mse = 0.0;
    int count = 0;

    for (int r = 0; r < img1.height; ++r)
    {
        for (int c = 0; c < img1.width; ++c)
        {
            double diff = img1.pixels[r][c] - img2.pixels[r][c];
            mse += diff * diff;
            count++;
        }
    }

    return (count > 0) ? (mse / count) : 0.0;
}

// Calculate PSNR (Peak Signal-to-Noise Ratio)
double calculatePSNR(const Image &original, const Image &decoded)
{
    double mse = calculateMSE(original, decoded);
    if (mse == 0)
        return INFINITY; // Perfect match
    if (mse < 0)
        return -1.0; // Error

    double maxPixelValue = 1.0; // Binary images use 0 and 1
    return 10.0 * log10((maxPixelValue * maxPixelValue) / mse);
}

// Calculate contrast ratio
double calculateContrast(const Image &img, const Image &original)
{
    int blackPixels = 0, whitePixels = 0;
    int blackIntensity = 0, whiteIntensity = 0;

    for (int r = 0; r < img.height; ++r)
    {
        for (int c = 0; c < img.width; ++c)
        {
            int orig_c = c;
            int orig_r = r;

            // Handle VCS expansion (2x width)
            if (img.width == original.width * 2)
            {
                orig_c = c / 2;
            }

            if (orig_r < original.height && orig_c < original.width)
            {
                if (original.pixels[orig_r][orig_c] == 0)
                { // Original white
                    whiteIntensity += img.pixels[r][c];
                    whitePixels++;
                }
                else
                { // Original black
                    blackIntensity += img.pixels[r][c];
                    blackPixels++;
                }
            }
        }
    }

    double avgWhite = (whitePixels > 0) ? (double)whiteIntensity / whitePixels : 0;
    double avgBlack = (blackPixels > 0) ? (double)blackIntensity / blackPixels : 0;

    // Contrast ratio
    if (avgWhite + avgBlack == 0)
        return 0;
    return (avgBlack - avgWhite) / (avgBlack + avgWhite);
}

// Calculate file size
long getFileSize(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.good())
        return -1;
    return file.tellg();
}

// Analyze share randomness (entropy)
double calculateEntropy(const Image &img)
{
    int histogram[256] = {0};
    int totalPixels = img.width * img.height;

    // Build histogram
    for (int r = 0; r < img.height; ++r)
    {
        for (int c = 0; c < img.width; ++c)
        {
            int val = img.pixels[r][c];
            if (val >= 0 && val < 256)
            {
                histogram[val]++;
            }
        }
    }

    // Calculate entropy
    double entropy = 0.0;
    for (int i = 0; i < 256; ++i)
    {
        if (histogram[i] > 0)
        {
            double probability = (double)histogram[i] / totalPixels;
            entropy -= probability * log2(probability);
        }
    }

    return entropy;
}

// Print comparison table
void printComparison()
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "VISUAL CRYPTOGRAPHY SCHEMES COMPARISON" << std::endl;
    std::cout << "========================================\n"
              << std::endl;

    // Load images
    Image original = binarizeImage(loadPGM("input/input.pgm"));

    // VCS
    Image vcs_s1 = loadPGM("output/vcs/vcs_share1.pgm");
    Image vcs_s2 = loadPGM("output/vcs/vcs_share2.pgm");
    Image vcs_dec = loadPGM("output/vcs/vcs_decrypted.pgm");

    // RG
    Image rg_s1 = loadPGM("output/rg/rg_share1.pgm");
    Image rg_s2 = loadPGM("output/rg/rg_share2.pgm");
    Image rg_dec = loadPGM("output/rg/rg_decrypted.pgm");

    // DHCOD
    Image dh_s1 = loadPGM("output/dhcod/dhcod_share1_meaningful.pgm");
    Image dh_s2 = loadPGM("output/dhcod/dhcod_share2_meaningful.pgm");
    Image dh_dec = loadPGM("output/dhcod/dhcod_decrypted.pgm");

    std::cout << std::fixed << std::setprecision(2);

    // Dimensions comparison
    std::cout << "1. IMAGE DIMENSIONS" << std::endl;
    std::cout << "   Original: " << original.width << " x " << original.height << std::endl;
    std::cout << "   VCS Shares: " << vcs_s1.width << " x " << vcs_s1.height
              << " (Expansion: " << (double)vcs_s1.width / original.width << "x)" << std::endl;
    std::cout << "   RG Shares: " << rg_s1.width << " x " << rg_s1.height
              << " (Expansion: " << (double)rg_s1.width / original.width << "x)" << std::endl;
    std::cout << "   DHCOD Shares: " << dh_s1.width << " x " << dh_s1.height
              << " (Expansion: " << (double)dh_s1.width / original.width << "x)" << std::endl;
    std::cout << std::endl;

    // File sizes
    std::cout << "2. FILE SIZES (bytes)" << std::endl;
    std::cout << "   VCS Share1: " << getFileSize("output/vcs/vcs_share1.pgm") << std::endl;
    std::cout << "   VCS Share2: " << getFileSize("output/vcs/vcs_share2.pgm") << std::endl;
    std::cout << "   RG Share1: " << getFileSize("output/rg/rg_share1.pgm") << std::endl;
    std::cout << "   RG Share2: " << getFileSize("output/rg/rg_share2.pgm") << std::endl;
    std::cout << "   DHCOD Share1: " << getFileSize("output/dhcod/dhcod_share1_meaningful.pgm") << std::endl;
    std::cout << "   DHCOD Share2: " << getFileSize("output/dhcod/dhcod_share2_meaningful.pgm") << std::endl;
    std::cout << std::endl;

    // Contrast ratios
    std::cout << "3. CONTRAST RATIO (Higher is better)" << std::endl;
    double vcs_contrast = calculateContrast(vcs_dec, original);
    double rg_contrast = calculateContrast(rg_dec, original);
    double dh_contrast = calculateContrast(dh_dec, original);
    std::cout << "   VCS: " << vcs_contrast << " (" << (vcs_contrast * 100) << "%)" << std::endl;
    std::cout << "   RG: " << rg_contrast << " (" << (rg_contrast * 100) << "%)" << std::endl;
    std::cout << "   DHCOD: " << dh_contrast << " (" << (dh_contrast * 100) << "%)" << std::endl;
    std::cout << std::endl;

    // Entropy (randomness)
    std::cout << "4. SHARE ENTROPY (bits per pixel)" << std::endl;
    std::cout << "   Higher entropy = more random = better security" << std::endl;
    std::cout << "   VCS Share1: " << calculateEntropy(vcs_s1) << " bits" << std::endl;
    std::cout << "   VCS Share2: " << calculateEntropy(vcs_s2) << " bits" << std::endl;
    std::cout << "   RG Share1: " << calculateEntropy(rg_s1) << " bits" << std::endl;
    std::cout << "   RG Share2: " << calculateEntropy(rg_s2) << " bits" << std::endl;
    std::cout << "   DHCOD Share1: " << calculateEntropy(dh_s1) << " bits" << std::endl;
    std::cout << "   DHCOD Share2: " << calculateEntropy(dh_s2) << " bits" << std::endl;
    std::cout << "   (Max entropy for binary = 1.0 bits)" << std::endl;
    std::cout << std::endl;

    // Quality metrics
    std::cout << "5. RECONSTRUCTION QUALITY" << std::endl;

    // For VCS, need to account for 2x expansion
    Image vcs_dec_resized(original.width, original.height);
    for (int r = 0; r < original.height; ++r)
    {
        for (int c = 0; c < original.width; ++c)
        {
            // Take OR of the two sub-pixels
            int val1 = vcs_dec.pixels[r][2 * c];
            int val2 = vcs_dec.pixels[r][2 * c + 1];
            vcs_dec_resized.pixels[r][c] = (val1 | val2);
        }
    }

    double vcs_psnr = calculatePSNR(original, vcs_dec_resized);
    double rg_psnr = calculatePSNR(original, rg_dec);
    double dh_psnr = calculatePSNR(original, dh_dec);

    std::cout << "   VCS PSNR: " << vcs_psnr << " dB" << std::endl;
    std::cout << "   RG PSNR: " << rg_psnr << " dB" << std::endl;
    std::cout << "   DHCOD PSNR: " << dh_psnr << " dB" << std::endl;
    std::cout << std::endl;

    // Summary table
    std::cout << "6. SUMMARY TABLE" << std::endl;
    std::cout << "   +----------+------------+----------+-----------+" << std::endl;
    std::cout << "   | Scheme   | Expansion  | Contrast | Entropy   |" << std::endl;
    std::cout << "   +----------+------------+----------+-----------+" << std::endl;
    std::cout << "   | VCS      | 2x         | "
              << std::setw(7) << (vcs_contrast * 100) << "% | "
              << std::setw(8) << calculateEntropy(vcs_s1) << " |" << std::endl;
    std::cout << "   | RG       | 1x         | "
              << std::setw(7) << (rg_contrast * 100) << "% | "
              << std::setw(8) << calculateEntropy(rg_s1) << " |" << std::endl;
    std::cout << "   | DHCOD    | 1x         | "
              << std::setw(7) << (dh_contrast * 100) << "% | "
              << std::setw(8) << calculateEntropy(dh_s1) << " |" << std::endl;
    std::cout << "   +----------+------------+----------+-----------+" << std::endl;
    std::cout << std::endl;

    // Advantages/Disadvantages
    std::cout << "7. KEY OBSERVATIONS" << std::endl;
    std::cout << "\n   VCS (Naor-Shamir):" << std::endl;
    std::cout << "   + Perfect security (random shares)" << std::endl;
    std::cout << "   + Visual decryption possible" << std::endl;
    std::cout << "   + Well-studied, proven secure" << std::endl;
    std::cout << "   - 2x pixel expansion" << std::endl;
    std::cout << "   - 50% contrast loss" << std::endl;

    std::cout << "\n   RG (Random Grid):" << std::endl;
    std::cout << "   + No pixel expansion" << std::endl;
    std::cout << "   + Perfect security (random shares)" << std::endl;
    std::cout << "   + Simpler algorithm" << std::endl;
    std::cout << "   + Visual decryption possible" << std::endl;
    std::cout << "   - 50% contrast loss" << std::endl;

    std::cout << "\n   DHCOD (Meaningful Shares):" << std::endl;
    std::cout << "   + No pixel expansion" << std::endl;
    std::cout << "   + Meaningful shares (less suspicious)" << std::endl;
    std::cout << "   + Better visual quality with XOR" << std::endl;
    std::cout << "   + Flexible (custom cover images)" << std::endl;
    std::cout << "   - Shares not perfectly random" << std::endl;
    std::cout << "   - XOR required for best results" << std::endl;

    std::cout << "\n========================================" << std::endl;
}

int main()
{
    std::cout << "Visual Cryptography Comparison and Analysis Tool" << std::endl;
    std::cout << "=================================================" << std::endl;

    printComparison();

    std::cout << "\nAnalysis complete!" << std::endl;
    std::cout << "Check the output files for visual comparison." << std::endl;

    return 0;
}
