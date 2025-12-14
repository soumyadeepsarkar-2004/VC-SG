#ifndef VCS_HPP
#define VCS_HPP

#include "image_utils.hpp"

namespace VCS {
    // Generate (2,2) shares using Naor-Shamir scheme.
    // Input: Secret image (binary, 0=White, 1=Black).
    // Outputs: Share1, Share2 (Width will be 2 * Input.Width).
    void generateShares(const Image& secret, Image& share1, Image& share2);

    // Simulate visual decryption (OR operation).
    // Input: Share1, Share2.
    // Output: Reconstructed image.
    Image decryptShares(const Image& share1, const Image& share2);
}

#endif // VCS_HPP
