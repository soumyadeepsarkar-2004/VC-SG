#include "rg.hpp"
#include <cstdlib>

namespace RG {

    void generateShares(const Image& secret, Image& share1, Image& share2) {
        int w = secret.width;
        int h = secret.height;

        share1 = Image(w, h);
        share2 = Image(w, h);

        for (int r = 0; r < h; ++r) {
            for (int c = 0; c < w; ++c) {
                // Step 1: Randomize R1
                share1.pixels[r][c] = rand() % 2; // 0 or 1

                // Step 2: Determine R2 based on Secret Pixel
                // Secret: 0=White, 1=Black
                int secretPixel = secret.pixels[r][c];

                if (secretPixel == 0) { // White
                    share2.pixels[r][c] = share1.pixels[r][c];
                } else { // Black
                    share2.pixels[r][c] = 1 - share1.pixels[r][c];
                }
            }
        }
    }

    Image decryptShares(const Image& share1, const Image& share2) {
        int w = share1.width;
        int h = share1.height;
        Image result(w, h);

        for (int r = 0; r < h; ++r) {
            for (int c = 0; c < w; ++c) {
                // Visual decryption is superposition (OR)
                result.pixels[r][c] = share1.pixels[r][c] | share2.pixels[r][c];
            }
        }
        return result;
    }

}
