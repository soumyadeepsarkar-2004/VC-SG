#include "dhcod.hpp"
#include <iostream>

namespace DHCOD {

    void generateShares(const Image& secret, const Image& cover, Image& share1, Image& share2) {
        int w = secret.width;
        int h = secret.height;

        // --- Bounds checking: resize cover if dimensions don't match ---
        Image coverResized = cover;
        if (cover.width != w || cover.height != h) {
            std::cout << "  [DHCOD] Cover image (" << cover.width << "x" << cover.height
                      << ") resized to match secret (" << w << "x" << h << ")" << std::endl;
            coverResized = resizeImage(cover, w, h);
        }

        // 1. Halftone the secret image (handles both grayscale and binary input)
        Image secretBin = halftoneImage(secret);

        // 2. Halftone the cover image -> becomes Share 1
        Image coverBin = halftoneImage(coverResized);
        share1 = coverBin;
        share1.isBinary = true;

        // 3. Generate Share 2 based on secret
        share2 = Image(w, h);
        share2.isBinary = true;

        for (int r = 0; r < h; ++r) {
            for (int c = 0; c < w; ++c) {
                int s_pix = secretBin.pixels[r][c]; // 0=White, 1=Black
                int c_pix = coverBin.pixels[r][c];  // 0=White, 1=Black

                // If Secret is White (0) -> share2 = share1  (XOR gives 0 = White)
                // If Secret is Black (1) -> share2 = NOT share1 (XOR gives 1 = Black)
                if (s_pix == 0) {
                    share2.pixels[r][c] = c_pix;
                } else {
                    share2.pixels[r][c] = 1 - c_pix;
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
                // Digital decryption via XOR
                // s1 == s2 -> 0 (White, secret was White)
                // s1 != s2 -> 1 (Black, secret was Black)
                result.pixels[r][c] = (share1.pixels[r][c] == share2.pixels[r][c]) ? 0 : 1;
            }
        }
        return result;
    }

}
