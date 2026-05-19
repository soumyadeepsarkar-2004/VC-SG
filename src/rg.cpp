#include "rg.hpp"

namespace RG {

    void generateShares(const Image& secret, Image& share1, Image& share2) {
        int w = secret.width;
        int h = secret.height;

        share1 = Image(w, h);
        share1.isBinary = true;
        share2 = Image(w, h);
        share2.isBinary = true;

        for (int r = 0; r < h; ++r) {
            for (int c = 0; c < w; ++c) {
                // Step 1: Randomize R1 using secure PRNG
                share1.pixels[r][c] = randomBit();

                // Step 2: Determine R2 based on Secret Pixel
                int secretPixel = secret.pixels[r][c]; // 0=White, 1=Black

                if (secretPixel == 0) { // White -> same as share1
                    share2.pixels[r][c] = share1.pixels[r][c];
                } else { // Black -> complement of share1
                    share2.pixels[r][c] = 1 - share1.pixels[r][c];
                }
            }
        }
    }

    Image decryptShares(const Image& share1, const Image& share2) {
        int w = share1.width;
        int h = share1.height;
        Image result(w, h);
        result.isBinary = true;

        for (int r = 0; r < h; ++r) {
            for (int c = 0; c < w; ++c) {
                // Visual decryption is superposition (OR)
                result.pixels[r][c] = share1.pixels[r][c] | share2.pixels[r][c];
            }
        }
        return result;
    }

}
