#include "dhcod.hpp"

namespace DHCOD {

    void generateShares(const Image& secret, const Image& cover, Image& share1, Image& share2) {
        int w = secret.width;
        int h = secret.height;
        // Assume cover is same size or resize/crop?
        // For this project, we assume inputs match.
        
        // 1. Process Secret
        // If secret is already binary (0/1), use as is. Note: 'binarizeImage' creates 0/1.
        // If it's grayscale, we should probably halftone it to preserve details if it's a photo.
        // Let's halftone it to be safe/advanced.
        Image secretBin = halftoneImage(secret);

        // 2. Process Cover -> Halftone to get Share 1
        Image coverBin = halftoneImage(cover);
        share1 = coverBin;
        share2 = Image(w, h);

        for (int r = 0; r < h; ++r) {
            for (int c = 0; c < w; ++c) {
                int s_pix = secretBin.pixels[r][c]; // 0=White, 1=Black
                int c_pix = coverBin.pixels[r][c];  // 0=White, 1=Black

                // Logic:
                // If Secret is White (0) -> share2 = share1
                // If Secret is Black (1) -> share2 = NOT share1
                
                if (s_pix == 0) { // White
                    share2.pixels[r][c] = c_pix;
                } else { // Black
                    share2.pixels[r][c] = 1 - c_pix; // Invert (0->1, 1->0)
                }
            }
        }
    }

    Image decryptShares(const Image& share1, const Image& share2) {
        // Digital Decryption via XOR
        // If s1 == s2 -> XOR is 0 (White). This happens when Secret was White.
        // If s1 != s2 -> XOR is 1 (Black). This happens when Secret was Black.
        int w = share1.width;
        int h = share1.height;
        Image result(w, h);

        for (int r = 0; r < h; ++r) {
            for (int c = 0; c < w; ++c) {
                // XOR
                if (share1.pixels[r][c] == share2.pixels[r][c]) {
                    result.pixels[r][c] = 0; // White
                } else {
                    result.pixels[r][c] = 1; // Black
                }
            }
        }
        return result;
    }

}
