#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "image_utils.hpp"
#include "vcs.hpp"
#include "rg.hpp"

void createSampleImage(const std::string &filename, int w, int h)
{
    Image img(w, h);
    // Draw a simple pattern: Black circle/square in middle
    for (int r = 0; r < h; ++r)
    {
        for (int c = 0; c < w; ++c)
        {
            // White background (0 per internal logic for PGM... wait)
            // Internal: 0=White, 1=Black.
            // Let's make background White (0).
            img.pixels[r][c] = 0;

            // Black square in center
            if (r > h / 4 && r < 3 * h / 4 && c > w / 4 && c < 3 * w / 4)
            {
                img.pixels[r][c] = 1;
            }
        }
    }
    savePGM(filename, img);
}

#include "dhcod.hpp"

void createSampleCover(const std::string &filename, int w, int h)
{
    Image img(w, h);
    // Gradient pattern for cover
    for (int r = 0; r < h; ++r)
    {
        for (int c = 0; c < w; ++c)
        {
            // Horizontal gradient 0->255
            img.pixels[r][c] = (c * 255) / w;
        }
    }
    savePGM(filename, img);
}

int main(int argc, char *argv[])
{
    srand(time(0));
    std::string inputFilename = "input/input.pgm";
    std::string coverFilename = "input/cover.pgm";

    // If file doesn't exist, create a sample
    std::ifstream check(inputFilename);
    if (!check.good())
    {
        std::cout << "Input file not found. Creating sample input.pgm..." << std::endl;
        createSampleImage(inputFilename, 200, 200);
    }
    check.close();

    // Check/Create Cover
    std::ifstream checkC(coverFilename);
    if (!checkC.good())
    {
        std::cout << "Cover file not found. Creating sample cover.pgm..." << std::endl;
        createSampleCover(coverFilename, 200, 200);
    }
    checkC.close();

    std::cout << "Loading " << inputFilename << "..." << std::endl;
    Image input = loadPGM(inputFilename);
    Image cover = loadPGM(coverFilename);

    if (input.width == 0 || cover.width == 0)
        return 1;

    // Resize cover if needed? For now assume user/sample provided correct sizes.
    // If we generated samples, we simply used 200x200 for both.

    std::cout << "Binarizing input for VCS/RG..." << std::endl;
    Image binary = binarizeImage(input);
    savePGM("output/binary_input.pgm", binary);

    // Run VCS
    std::cout << "\nRunning (2,2) Visual Cryptography Scheme..." << std::endl;
    Image vcs_s1(0, 0), vcs_s2(0, 0);
    VCS::generateShares(binary, vcs_s1, vcs_s2);

    Image vcs_dec = VCS::decryptShares(vcs_s1, vcs_s2);

    savePGM("output/vcs/vcs_share1.pgm", vcs_s1);
    savePGM("output/vcs/vcs_share2.pgm", vcs_s2);
    savePGM("output/vcs/vcs_decrypted.pgm", vcs_dec);
    std::cout << "Saved VCS files." << std::endl;

    // Run RG
    std::cout << "\nRunning (2,2) Random Grid Scheme..." << std::endl;
    Image rg_s1(0, 0), rg_s2(0, 0);
    RG::generateShares(binary, rg_s1, rg_s2);

    Image rg_dec = RG::decryptShares(rg_s1, rg_s2);

    savePGM("output/rg/rg_share1.pgm", rg_s1);
    savePGM("output/rg/rg_share2.pgm", rg_s2);
    savePGM("output/rg/rg_decrypted.pgm", rg_dec);
    std::cout << "Saved RG files." << std::endl;

    // Run DHCOD
    std::cout << "\nRunning DHCOD (Meaningful Shares)..." << std::endl;
    Image dh_s1(0, 0), dh_s2(0, 0);
    // DHCOD takes Grayscale input (handles halftoning internally effectively)
    // or we can pass original input logic.
    DHCOD::generateShares(input, cover, dh_s1, dh_s2);

    Image dh_dec = DHCOD::decryptShares(dh_s1, dh_s2);

    savePGM("output/dhcod/dhcod_share1_meaningful.pgm", dh_s1);
    savePGM("output/dhcod/dhcod_share2_meaningful.pgm", dh_s2);
    savePGM("output/dhcod/dhcod_decrypted.pgm", dh_dec);
    std::cout << "Saved DHCOD files." << std::endl;

    std::cout << "\nDone. Check the output PGM files." << std::endl;

    return 0;
}
