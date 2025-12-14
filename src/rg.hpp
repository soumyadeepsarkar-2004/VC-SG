#ifndef RG_HPP
#define RG_HPP

#include "image_utils.hpp"

namespace RG {
    // Generate (2,2) shares using Kafri-Keren Random Grid scheme.
    // Input: Secret image (binary).
    // Outputs: Share1, Share2 (Width == Input.Width).
    void generateShares(const Image& secret, Image& share1, Image& share2);

    // Simulate visual decryption (OR).
    Image decryptShares(const Image& share1, const Image& share2);
}

#endif // RG_HPP
