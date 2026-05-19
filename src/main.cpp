#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "image_utils.hpp"
#include "vcs.hpp"
#include "rg.hpp"
#include "dhcod.hpp"

// ============================================================
// Sample Image Generators (used when input files don't exist)
// ============================================================

void createSampleImage(const std::string &filename, int w, int h) {
    Image img(w, h);
    img.isBinary = true;
    // Draw a black square in the center on a white background
    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            img.pixels[r][c] = 0; // White
            if (r > h / 4 && r < 3 * h / 4 && c > w / 4 && c < 3 * w / 4) {
                img.pixels[r][c] = 1; // Black
            }
        }
    }
    savePGM(filename, img);
}

void createSampleCover(const std::string &filename, int w, int h) {
    Image img(w, h);
    img.isBinary = false;
    // Horizontal gradient pattern for cover
    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            img.pixels[r][c] = (c * 255) / w;
        }
    }
    savePGM(filename, img);
}

// ============================================================
// CLI Help
// ============================================================

void printHelp(const char* progName) {
    std::cout << "Usage: " << progName << " [options]\n\n"
              << "Options:\n"
              << "  -i <file>    Input secret image   (default: input/input.pgm)\n"
              << "  -c <file>    Cover image (DHCOD)   (default: input/cover.pgm)\n"
              << "  -o <dir>     Output directory       (default: output/)\n"
              << "  -s <scheme>  Scheme to run: vcs, rg, dhcod, all (default: all)\n"
              << "  -h           Show this help\n\n"
              << "Examples:\n"
              << "  " << progName << "\n"
              << "  " << progName << " -i myimage.pgm -s vcs\n"
              << "  " << progName << " -i secret.pgm -c cover.pgm -o results/\n";
}

// ============================================================
// Main
// ============================================================

int main(int argc, char *argv[]) {
    std::string inputFilename = "input/input.pgm";
    std::string coverFilename = "input/cover.pgm";
    std::string outputDir     = "output";
    std::string scheme        = "all";

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-i") && i + 1 < argc) {
            inputFilename = argv[++i];
        } else if ((arg == "-c") && i + 1 < argc) {
            coverFilename = argv[++i];
        } else if ((arg == "-o") && i + 1 < argc) {
            outputDir = argv[++i];
        } else if ((arg == "-s") && i + 1 < argc) {
            scheme = argv[++i];
        } else if (arg == "-h" || arg == "--help") {
            printHelp(argv[0]);
            return 0;
        } else {
            std::cerr << "Unknown argument: " << arg << "\n";
            printHelp(argv[0]);
            return 1;
        }
    }

    // Ensure output directories exist
    ensureDirectories(outputDir + "/vcs/");
    ensureDirectories(outputDir + "/rg/");
    ensureDirectories(outputDir + "/dhcod/");

    // Check if input files exist, create samples if not
    {
        std::ifstream check(inputFilename);
        if (!check.good()) {
            std::cout << "Input file not found. Creating sample input.pgm..." << std::endl;
            ensureDirectories("input/");
            createSampleImage(inputFilename, 200, 200);
        }
    }
    {
        std::ifstream check(coverFilename);
        if (!check.good()) {
            std::cout << "Cover file not found. Creating sample cover.pgm..." << std::endl;
            ensureDirectories("input/");
            createSampleCover(coverFilename, 200, 200);
        }
    }

    // Load images
    std::cout << "Loading " << inputFilename << "..." << std::endl;
    Image input = loadPGM(inputFilename);
    Image cover = loadPGM(coverFilename);

    if (input.empty()) {
        std::cerr << "Error: Failed to load input image." << std::endl;
        return 1;
    }
    if (cover.empty()) {
        std::cerr << "Error: Failed to load cover image." << std::endl;
        return 1;
    }

    std::cout << "Input: " << input.width << "x" << input.height << " pixels" << std::endl;
    std::cout << "Cover: " << cover.width << "x" << cover.height << " pixels" << std::endl;

    // Binarize input for VCS/RG
    std::cout << "\nBinarizing input for VCS/RG..." << std::endl;
    Image binary = binarizeImage(input);
    binary.isBinary = true;
    if (!savePGM(outputDir + "/binary_input.pgm", binary)) {
        std::cerr << "Warning: Could not save binary_input.pgm" << std::endl;
    }

    bool runVCS   = (scheme == "all" || scheme == "vcs");
    bool runRG    = (scheme == "all" || scheme == "rg");
    bool runDHCOD = (scheme == "all" || scheme == "dhcod");

    // --- VCS ---
    if (runVCS) {
        std::cout << "\n=== Running (2,2) Visual Cryptography Scheme ===" << std::endl;
        Image vcs_s1(0, 0), vcs_s2(0, 0);
        VCS::generateShares(binary, vcs_s1, vcs_s2);
        Image vcs_dec = VCS::decryptShares(vcs_s1, vcs_s2);

        savePGM(outputDir + "/vcs/vcs_share1.pgm", vcs_s1);
        savePGM(outputDir + "/vcs/vcs_share2.pgm", vcs_s2);
        savePGM(outputDir + "/vcs/vcs_decrypted.pgm", vcs_dec);
        std::cout << "  Shares: " << vcs_s1.width << "x" << vcs_s1.height
                  << " (2x expansion)" << std::endl;
        std::cout << "  Saved VCS files to " << outputDir << "/vcs/" << std::endl;
    }

    // --- Random Grid ---
    if (runRG) {
        std::cout << "\n=== Running (2,2) Random Grid Scheme ===" << std::endl;
        Image rg_s1(0, 0), rg_s2(0, 0);
        RG::generateShares(binary, rg_s1, rg_s2);
        Image rg_dec = RG::decryptShares(rg_s1, rg_s2);

        savePGM(outputDir + "/rg/rg_share1.pgm", rg_s1);
        savePGM(outputDir + "/rg/rg_share2.pgm", rg_s2);
        savePGM(outputDir + "/rg/rg_decrypted.pgm", rg_dec);
        std::cout << "  Shares: " << rg_s1.width << "x" << rg_s1.height
                  << " (no expansion)" << std::endl;
        std::cout << "  Saved RG files to " << outputDir << "/rg/" << std::endl;
    }

    // --- DHCOD ---
    if (runDHCOD) {
        std::cout << "\n=== Running DHCOD (Meaningful Shares) ===" << std::endl;
        Image dh_s1(0, 0), dh_s2(0, 0);
        DHCOD::generateShares(input, cover, dh_s1, dh_s2);
        Image dh_dec = DHCOD::decryptShares(dh_s1, dh_s2);

        savePGM(outputDir + "/dhcod/dhcod_share1_meaningful.pgm", dh_s1);
        savePGM(outputDir + "/dhcod/dhcod_share2_meaningful.pgm", dh_s2);
        savePGM(outputDir + "/dhcod/dhcod_decrypted.pgm", dh_dec);
        std::cout << "  Shares: " << dh_s1.width << "x" << dh_s1.height
                  << " (meaningful, no expansion)" << std::endl;
        std::cout << "  Saved DHCOD files to " << outputDir << "/dhcod/" << std::endl;
    }

    std::cout << "\n✓ Done. Check the output PGM files in " << outputDir << "/" << std::endl;

    return 0;
}
