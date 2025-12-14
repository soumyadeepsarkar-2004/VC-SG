#ifndef DHCOD_HPP
#define DHCOD_HPP

#include "image_utils.hpp"

namespace DHCOD {
    // Generate shares using DHCOD (Meaningful Shares).
    // Input: 
    //   secret: The image to hide (Grayscale or Binary).
    //   cover: The image to appear on the shares (Grayscale).
    // Outputs:
    //   share1, share2: Meaningful shares looking like 'cover'.
    // Logic:
    //   H = secret, X = cover.
    //   X1 = Halftone(X).
    //   H2 = Binary(H) (or Halftone(H)).
    //   If H2 is White (0): X2 = X1.
    //   If H2 is Black (1): X2 = Complement(X1).
    void generateShares(const Image& secret, const Image& cover, Image& share1, Image& share2);

    // Decrypt using XOR (Digital Reconstruction).
    Image decryptShares(const Image& share1, const Image& share2);
}

#endif // DHCOD_HPP
