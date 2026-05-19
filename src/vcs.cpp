#include "vcs.hpp"

namespace VCS {

    void generateShares(const Image& secret, Image& share1, Image& share2) {
        int w = secret.width;
        int h = secret.height;

        // Expansion factor: 2 (horizontal)
        share1 = Image(w * 2, h);
        share1.isBinary = true;
        share2 = Image(w * 2, h);
        share2.isBinary = true;

        for (int r = 0; r < h; ++r) {
            for (int c = 0; c < w; ++c) {
                int pixel = secret.pixels[r][c]; // 0=White, 1=Black

                // Base patterns:
                //   Pattern A: [1, 0] (Black, White)
                //   Pattern B: [0, 1] (White, Black)
                int coin = randomBit();  // Secure PRNG
                int s1_sub[2];
                int s2_sub[2];

                if (pixel == 0) { // White Pixel -> Shares should be identical
                    if (coin == 0) {
                        s1_sub[0] = 1; s1_sub[1] = 0;
                        s2_sub[0] = 1; s2_sub[1] = 0;
                    } else {
                        s1_sub[0] = 0; s1_sub[1] = 1;
                        s2_sub[0] = 0; s2_sub[1] = 1;
                    }
                } else { // Black Pixel -> Shares should be complements
                    if (coin == 0) {
                        s1_sub[0] = 1; s1_sub[1] = 0;
                        s2_sub[0] = 0; s2_sub[1] = 1;
                    } else {
                        s1_sub[0] = 0; s1_sub[1] = 1;
                        s2_sub[0] = 1; s2_sub[1] = 0;
                    }
                }

                share1.pixels[r][2 * c]     = s1_sub[0];
                share1.pixels[r][2 * c + 1] = s1_sub[1];
                share2.pixels[r][2 * c]     = s2_sub[0];
                share2.pixels[r][2 * c + 1] = s2_sub[1];
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
                // Visual Cryptography: OR operation (stacking transparencies)
                // 1=Black (Opaque), 0=White (Transparent)
                result.pixels[r][c] = share1.pixels[r][c] | share2.pixels[r][c];
            }
        }
        return result;
    }

}
