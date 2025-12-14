#ifndef IMAGE_UTILS_HPP
#define IMAGE_UTILS_HPP

#include <vector>
#include <string>
#include <iostream>

struct Image {
    int width;
    int height;
    std::vector<std::vector<int>> pixels; // 0 for black, 1 for white (or 0-255 for grayscale)

    Image(int w, int h, int val = 0) : width(w), height(h), pixels(h, std::vector<int>(w, val)) {}
};

// Load a PGM image (P2 or P5). Returns an empty image on failure.
Image loadPGM(const std::string& filename);

// Save a PGM image (P2 ASCII for readability, or P5 for compactness).
// Using P2 (ASCII) for easier debugging.
bool savePGM(const std::string& filename, const Image& img);

// Convert a grayscale image to strict binary (0 or 1) using a threshold.
// 0 = Black, 1 = White.
// Note: In standard PGM, 0 is black, 255 is white.
// Our VCS logic usually treats 1 as Black (ink) and 0 as White (paper) or vice versa.
// Let's standardise: INTERNAL representation: 0 = White/Transparent, 1 = Black/Opaque.
// When saving to PGM: 0 -> 255 (White), 1 -> 0 (Black).
Image binarizeImage(const Image& input, int threshold = 128);

// Halftone an image using Ordered Dithering (Bayer Matrix).
// Returns a binary image (0=White, 1=Black).
Image halftoneImage(const Image& input);

#endif // IMAGE_UTILS_HPP
