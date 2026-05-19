#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>
#include <chrono>
#include "image_utils.hpp"
#include "vcs.hpp"
#include "rg.hpp"
#include "dhcod.hpp"

// ============================================================
// Metrics
// ============================================================

double calculateMSE(const Image &img1, const Image &img2) {
    if (img1.width != img2.width || img1.height != img2.height) {
        std::cerr << "Error: Image dimensions don't match for MSE!" << std::endl;
        return -1.0;
    }
    double mse = 0.0;
    int count = img1.width * img1.height;
    for (int r = 0; r < img1.height; ++r) {
        for (int c = 0; c < img1.width; ++c) {
            double diff = img1.pixels[r][c] - img2.pixels[r][c];
            mse += diff * diff;
        }
    }
    return (count > 0) ? (mse / count) : 0.0;
}

double calculatePSNR(const Image &original, const Image &decoded) {
    double mse = calculateMSE(original, decoded);
    if (mse == 0) return 999.0; // Perfect match
    if (mse < 0)  return -1.0;  // Error
    double maxVal = 1.0; // Binary images: range is 0-1
    return 10.0 * log10((maxVal * maxVal) / mse);
}

// SSIM (Structural Similarity Index) — global computation for binary images
double calculateSSIM(const Image &img1, const Image &img2) {
    if (img1.width != img2.width || img1.height != img2.height) return -1.0;
    int N = img1.width * img1.height;
    if (N == 0) return 0.0;

    // Compute means
    double mu_x = 0, mu_y = 0;
    for (int r = 0; r < img1.height; ++r) {
        for (int c = 0; c < img1.width; ++c) {
            mu_x += img1.pixels[r][c];
            mu_y += img2.pixels[r][c];
        }
    }
    mu_x /= N;
    mu_y /= N;

    // Compute variances and covariance
    double sig_x2 = 0, sig_y2 = 0, sig_xy = 0;
    for (int r = 0; r < img1.height; ++r) {
        for (int c = 0; c < img1.width; ++c) {
            double dx = img1.pixels[r][c] - mu_x;
            double dy = img2.pixels[r][c] - mu_y;
            sig_x2 += dx * dx;
            sig_y2 += dy * dy;
            sig_xy += dx * dy;
        }
    }
    sig_x2 /= N;
    sig_y2 /= N;
    sig_xy /= N;

    // SSIM constants (L=1 for binary images)
    double C1 = 0.0001; // (0.01 * 1)^2
    double C2 = 0.0009; // (0.03 * 1)^2

    double ssim = ((2 * mu_x * mu_y + C1) * (2 * sig_xy + C2)) /
                  ((mu_x * mu_x + mu_y * mu_y + C1) * (sig_x2 + sig_y2 + C2));
    return ssim;
}

double calculateContrast(const Image &img, const Image &original) {
    int blackPixels = 0, whitePixels = 0;
    int blackIntensity = 0, whiteIntensity = 0;

    for (int r = 0; r < img.height; ++r) {
        for (int c = 0; c < img.width; ++c) {
            int orig_c = c, orig_r = r;
            if (img.width == original.width * 2) orig_c = c / 2;
            if (orig_r < original.height && orig_c < original.width) {
                if (original.pixels[orig_r][orig_c] == 0) {
                    whiteIntensity += img.pixels[r][c];
                    whitePixels++;
                } else {
                    blackIntensity += img.pixels[r][c];
                    blackPixels++;
                }
            }
        }
    }
    double avgWhite = (whitePixels > 0) ? (double)whiteIntensity / whitePixels : 0;
    double avgBlack = (blackPixels > 0) ? (double)blackIntensity / blackPixels : 0;
    if (avgWhite + avgBlack == 0) return 0;
    return (avgBlack - avgWhite) / (avgBlack + avgWhite);
}

long getFileSize(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    return file.good() ? static_cast<long>(file.tellg()) : -1;
}

double calculateEntropy(const Image &img) {
    int histogram[256] = {0};
    int totalPixels = img.width * img.height;
    for (int r = 0; r < img.height; ++r)
        for (int c = 0; c < img.width; ++c) {
            int val = img.pixels[r][c];
            if (val >= 0 && val < 256) histogram[val]++;
        }
    double entropy = 0.0;
    for (int i = 0; i < 256; ++i) {
        if (histogram[i] > 0) {
            double p = (double)histogram[i] / totalPixels;
            entropy -= p * log2(p);
        }
    }
    return entropy;
}

// Reduce VCS 2x-expanded decrypted image back to original dimensions
Image reduceVCS(const Image &vcs_dec, int origWidth) {
    Image result(origWidth, vcs_dec.height);
    result.isBinary = true;
    for (int r = 0; r < vcs_dec.height; ++r)
        for (int c = 0; c < origWidth; ++c)
            result.pixels[r][c] = vcs_dec.pixels[r][2 * c] | vcs_dec.pixels[r][2 * c + 1];
    return result;
}

// ============================================================
// Benchmark helper
// ============================================================
struct BenchResult {
    double encryptMs;
    double decryptMs;
};

BenchResult benchmarkVCS(const Image& binary) {
    BenchResult res;
    Image s1(0,0), s2(0,0);
    auto t0 = std::chrono::high_resolution_clock::now();
    VCS::generateShares(binary, s1, s2);
    auto t1 = std::chrono::high_resolution_clock::now();
    VCS::decryptShares(s1, s2);
    auto t2 = std::chrono::high_resolution_clock::now();
    res.encryptMs = std::chrono::duration<double, std::milli>(t1 - t0).count();
    res.decryptMs = std::chrono::duration<double, std::milli>(t2 - t1).count();
    return res;
}

BenchResult benchmarkRG(const Image& binary) {
    BenchResult res;
    Image s1(0,0), s2(0,0);
    auto t0 = std::chrono::high_resolution_clock::now();
    RG::generateShares(binary, s1, s2);
    auto t1 = std::chrono::high_resolution_clock::now();
    RG::decryptShares(s1, s2);
    auto t2 = std::chrono::high_resolution_clock::now();
    res.encryptMs = std::chrono::duration<double, std::milli>(t1 - t0).count();
    res.decryptMs = std::chrono::duration<double, std::milli>(t2 - t1).count();
    return res;
}

BenchResult benchmarkDHCOD(const Image& input, const Image& cover) {
    BenchResult res;
    Image s1(0,0), s2(0,0);
    auto t0 = std::chrono::high_resolution_clock::now();
    DHCOD::generateShares(input, cover, s1, s2);
    auto t1 = std::chrono::high_resolution_clock::now();
    DHCOD::decryptShares(s1, s2);
    auto t2 = std::chrono::high_resolution_clock::now();
    res.encryptMs = std::chrono::duration<double, std::milli>(t1 - t0).count();
    res.decryptMs = std::chrono::duration<double, std::milli>(t2 - t1).count();
    return res;
}

// ============================================================
// CSV Export
// ============================================================
void exportCSV(const std::string& filename,
               double vcs_contrast, double rg_contrast, double dh_contrast,
               double vcs_entropy, double rg_entropy, double dh_entropy,
               double vcs_psnr, double rg_psnr, double dh_psnr,
               double vcs_ssim, double rg_ssim, double dh_ssim,
               const BenchResult& bVCS, const BenchResult& bRG, const BenchResult& bDH)
{
    std::ofstream csv(filename);
    if (!csv.is_open()) {
        std::cerr << "Error: Could not create CSV file " << filename << std::endl;
        return;
    }
    csv << "Scheme,Expansion,Contrast,Entropy_bpp,PSNR_dB,SSIM,Encrypt_ms,Decrypt_ms\n";
    csv << std::fixed << std::setprecision(4);
    csv << "VCS,2x," << vcs_contrast << "," << vcs_entropy << "," << vcs_psnr << "," << vcs_ssim
        << "," << bVCS.encryptMs << "," << bVCS.decryptMs << "\n";
    csv << "RG,1x," << rg_contrast << "," << rg_entropy << "," << rg_psnr << "," << rg_ssim
        << "," << bRG.encryptMs << "," << bRG.decryptMs << "\n";
    csv << "DHCOD,1x," << dh_contrast << "," << dh_entropy << "," << dh_psnr << "," << dh_ssim
        << "," << bDH.encryptMs << "," << bDH.decryptMs << "\n";
    csv.close();
    std::cout << "\n  CSV exported to: " << filename << std::endl;
}

// ============================================================
// Main Analysis
// ============================================================
void printComparison(const std::string& csvFile) {
    std::cout << "\n============================================================" << std::endl;
    std::cout << "     VISUAL CRYPTOGRAPHY SCHEMES — COMPARISON ANALYSIS" << std::endl;
    std::cout << "============================================================\n" << std::endl;

    // Load images — binarize decoded images to match internal domain
    Image original = binarizeImage(loadPGM("input/input.pgm"));
    Image input_raw = loadPGM("input/input.pgm");
    Image cover_raw = loadPGM("input/cover.pgm");

    Image vcs_s1 = binarizeImage(loadPGM("output/vcs/vcs_share1.pgm"));
    Image vcs_s2 = binarizeImage(loadPGM("output/vcs/vcs_share2.pgm"));
    Image vcs_dec = binarizeImage(loadPGM("output/vcs/vcs_decrypted.pgm"));

    Image rg_s1 = binarizeImage(loadPGM("output/rg/rg_share1.pgm"));
    Image rg_s2 = binarizeImage(loadPGM("output/rg/rg_share2.pgm"));
    Image rg_dec = binarizeImage(loadPGM("output/rg/rg_decrypted.pgm"));

    Image dh_s1 = binarizeImage(loadPGM("output/dhcod/dhcod_share1_meaningful.pgm"));
    Image dh_s2 = binarizeImage(loadPGM("output/dhcod/dhcod_share2_meaningful.pgm"));
    Image dh_dec = binarizeImage(loadPGM("output/dhcod/dhcod_decrypted.pgm"));

    std::cout << std::fixed << std::setprecision(4);

    // 1. Dimensions
    std::cout << "1. IMAGE DIMENSIONS" << std::endl;
    std::cout << "   Original:     " << original.width << " x " << original.height << std::endl;
    std::cout << "   VCS Shares:   " << vcs_s1.width << " x " << vcs_s1.height
              << "  (Expansion: " << std::setprecision(1) << (double)vcs_s1.width / original.width << "x)" << std::endl;
    std::cout << "   RG Shares:    " << rg_s1.width << " x " << rg_s1.height
              << "  (Expansion: " << (double)rg_s1.width / original.width << "x)" << std::endl;
    std::cout << "   DHCOD Shares: " << dh_s1.width << " x " << dh_s1.height
              << "  (Expansion: " << (double)dh_s1.width / original.width << "x)" << std::endl;
    std::cout << std::setprecision(4) << std::endl;

    // 2. File Sizes
    std::cout << "2. FILE SIZES (bytes)" << std::endl;
    std::cout << "   VCS  Share1: " << getFileSize("output/vcs/vcs_share1.pgm")
              << "  Share2: " << getFileSize("output/vcs/vcs_share2.pgm") << std::endl;
    std::cout << "   RG   Share1: " << getFileSize("output/rg/rg_share1.pgm")
              << "  Share2: " << getFileSize("output/rg/rg_share2.pgm") << std::endl;
    std::cout << "   DHCOD Share1: " << getFileSize("output/dhcod/dhcod_share1_meaningful.pgm")
              << "  Share2: " << getFileSize("output/dhcod/dhcod_share2_meaningful.pgm") << std::endl;
    std::cout << std::endl;

    // 3. Contrast
    double vcs_contrast = calculateContrast(vcs_dec, original);
    double rg_contrast  = calculateContrast(rg_dec, original);
    double dh_contrast  = calculateContrast(dh_dec, original);
    std::cout << "3. CONTRAST RATIO (Higher = better)" << std::endl;
    std::cout << "   VCS:   " << vcs_contrast << "  (" << std::setprecision(1) << (vcs_contrast * 100) << "%)" << std::endl;
    std::cout << "   RG:    " << std::setprecision(4) << rg_contrast << "  (" << std::setprecision(1) << (rg_contrast * 100) << "%)" << std::endl;
    std::cout << "   DHCOD: " << std::setprecision(4) << dh_contrast << "  (" << std::setprecision(1) << (dh_contrast * 100) << "%)" << std::endl;
    std::cout << std::setprecision(4) << std::endl;

    // 4. Entropy
    double vcs_e1 = calculateEntropy(vcs_s1), vcs_e2 = calculateEntropy(vcs_s2);
    double rg_e1  = calculateEntropy(rg_s1),  rg_e2  = calculateEntropy(rg_s2);
    double dh_e1  = calculateEntropy(dh_s1),  dh_e2  = calculateEntropy(dh_s2);
    std::cout << "4. SHARE ENTROPY (bits/pixel — closer to 1.0 = more secure)" << std::endl;
    std::cout << "   VCS   Share1: " << vcs_e1 << "  Share2: " << vcs_e2 << std::endl;
    std::cout << "   RG    Share1: " << rg_e1  << "  Share2: " << rg_e2  << std::endl;
    std::cout << "   DHCOD Share1: " << dh_e1  << "  Share2: " << dh_e2  << std::endl;
    std::cout << std::endl;

    // 5. Reconstruction Quality (PSNR + SSIM)
    Image vcs_dec_reduced = reduceVCS(vcs_dec, original.width);
    double vcs_psnr = calculatePSNR(original, vcs_dec_reduced);
    double rg_psnr  = calculatePSNR(original, rg_dec);
    double dh_psnr  = calculatePSNR(original, dh_dec);
    double vcs_ssim = calculateSSIM(original, vcs_dec_reduced);
    double rg_ssim  = calculateSSIM(original, rg_dec);
    double dh_ssim  = calculateSSIM(original, dh_dec);

    std::cout << "5. RECONSTRUCTION QUALITY" << std::endl;
    std::cout << "   VCS   — PSNR: " << vcs_psnr << " dB  |  SSIM: " << vcs_ssim << std::endl;
    std::cout << "   RG    — PSNR: " << rg_psnr  << " dB  |  SSIM: " << rg_ssim  << std::endl;
    std::cout << "   DHCOD — PSNR: " << dh_psnr  << " dB  |  SSIM: " << dh_ssim  << std::endl;
    std::cout << std::endl;

    // 6. Performance Benchmarks
    std::cout << "6. PERFORMANCE BENCHMARKS" << std::endl;
    Image binary = binarizeImage(input_raw);
    BenchResult bVCS = benchmarkVCS(binary);
    BenchResult bRG  = benchmarkRG(binary);
    BenchResult bDH  = benchmarkDHCOD(input_raw, cover_raw);
    std::cout << std::setprecision(2);
    std::cout << "   VCS   — Encrypt: " << bVCS.encryptMs << " ms  |  Decrypt: " << bVCS.decryptMs << " ms" << std::endl;
    std::cout << "   RG    — Encrypt: " << bRG.encryptMs  << " ms  |  Decrypt: " << bRG.decryptMs  << " ms" << std::endl;
    std::cout << "   DHCOD — Encrypt: " << bDH.encryptMs  << " ms  |  Decrypt: " << bDH.decryptMs  << " ms" << std::endl;
    std::cout << std::setprecision(4) << std::endl;

    // 7. Summary Table
    std::cout << "7. SUMMARY TABLE" << std::endl;
    std::cout << "   +---------+------+----------+---------+--------+-------+" << std::endl;
    std::cout << "   | Scheme  | Exp. | Contrast | Entropy | PSNR   | SSIM  |" << std::endl;
    std::cout << "   +---------+------+----------+---------+--------+-------+" << std::endl;
    std::cout << std::setprecision(2);
    std::cout << "   | VCS     |  2x  | " << std::setw(7) << (vcs_contrast*100) << "% | " << std::setw(7) << vcs_e1
              << " | " << std::setw(6) << vcs_psnr << " | " << std::setw(5) << vcs_ssim << " |" << std::endl;
    std::cout << "   | RG      |  1x  | " << std::setw(7) << (rg_contrast*100) << "% | " << std::setw(7) << rg_e1
              << " | " << std::setw(6) << rg_psnr << " | " << std::setw(5) << rg_ssim << " |" << std::endl;
    std::cout << "   | DHCOD   |  1x  | " << std::setw(7) << (dh_contrast*100) << "% | " << std::setw(7) << dh_e1
              << " | " << std::setw(6) << dh_psnr << " | " << std::setw(5) << dh_ssim << " |" << std::endl;
    std::cout << "   +---------+------+----------+---------+--------+-------+" << std::endl;
    std::cout << std::endl;

    // 8. Key Observations
    std::cout << "8. KEY OBSERVATIONS" << std::endl;
    std::cout << "\n   VCS (Naor-Shamir):" << std::endl;
    std::cout << "   + Perfect security (information-theoretic)" << std::endl;
    std::cout << "   + Visual decryption possible (physical stacking)" << std::endl;
    std::cout << "   + Well-studied, proven secure" << std::endl;
    std::cout << "   - 2x pixel expansion (larger shares)" << std::endl;
    std::cout << "   - 50% contrast loss on reconstruction" << std::endl;

    std::cout << "\n   RG (Random Grid):" << std::endl;
    std::cout << "   + No pixel expansion (space-efficient)" << std::endl;
    std::cout << "   + Perfect security (random shares)" << std::endl;
    std::cout << "   + Simpler algorithm" << std::endl;
    std::cout << "   + Visual decryption possible" << std::endl;
    std::cout << "   - 50% contrast loss on reconstruction" << std::endl;

    std::cout << "\n   DHCOD (Meaningful Shares):" << std::endl;
    std::cout << "   + No pixel expansion" << std::endl;
    std::cout << "   + Meaningful shares (steganographic camouflage)" << std::endl;
    std::cout << "   + Perfect XOR reconstruction quality" << std::endl;
    std::cout << "   + Flexible (custom cover images)" << std::endl;
    std::cout << "   - Shares not perfectly random (lower entropy)" << std::endl;
    std::cout << "   - Requires digital XOR (no physical decryption)" << std::endl;

    std::cout << "\n============================================================" << std::endl;

    // CSV export
    if (!csvFile.empty()) {
        exportCSV(csvFile, vcs_contrast, rg_contrast, dh_contrast,
                  vcs_e1, rg_e1, dh_e1, vcs_psnr, rg_psnr, dh_psnr,
                  vcs_ssim, rg_ssim, dh_ssim, bVCS, bRG, bDH);
    }
}

// ============================================================
// CLI
// ============================================================
int main(int argc, char* argv[]) {
    std::string csvFile;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "--csv") && i + 1 < argc) {
            csvFile = argv[++i];
        } else if (arg == "-h" || arg == "--help") {
            std::cout << "Usage: " << argv[0] << " [options]\n"
                      << "  --csv <file>  Export results to CSV file\n"
                      << "  -h, --help    Show this help\n";
            return 0;
        }
    }

    std::cout << "Visual Cryptography — Comparison & Analysis Tool" << std::endl;
    std::cout << "=================================================" << std::endl;

    printComparison(csvFile);

    std::cout << "\n✓ Analysis complete!" << std::endl;
    return 0;
}
