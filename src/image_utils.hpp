#ifndef IMAGE_UTILS_HPP
#define IMAGE_UTILS_HPP

#include <vector>
#include <string>
#include <iostream>
#include <random>

struct Image {
    int width;
    int height;
    int maxVal;      // PGM max value (typically 255)
    bool isBinary;   // true if pixels are 0/1 (internal: 0=White, 1=Black)
    std::vector<std::vector<int>> pixels;

    Image() : width(0), height(0), maxVal(255), isBinary(false) {}
    Image(int w, int h, int val = 0)
        : width(w), height(h), maxVal(255), isBinary(false),
          pixels(h, std::vector<int>(w, val)) {}

    bool empty() const { return width == 0 || height == 0; }
};

// --- I/O ---
// Load a PGM image (P2 or P5). Returns an empty image on failure.
Image loadPGM(const std::string& filename);

// Save a PGM image (P2 ASCII).
// If img.isBinary: maps internal 0->255(White), 1->0(Black).
// If !img.isBinary: writes pixel values as-is.
bool savePGM(const std::string& filename, const Image& img);

// --- Processing ---
// Convert grayscale to binary (internal: 0=White, 1=Black). Output has isBinary=true.
Image binarizeImage(const Image& input, int threshold = 128);

// Halftone using 4x4 Bayer ordered dithering. Output has isBinary=true.
Image halftoneImage(const Image& input);

// Nearest-neighbor resize.
Image resizeImage(const Image& input, int newWidth, int newHeight);

// --- Utilities ---
// Cryptographically better random bit (0 or 1) using Mersenne Twister.
int randomBit();

// Ensure a directory path exists (creates parent dirs as needed).
void ensureDirectories(const std::string& path);

#endif // IMAGE_UTILS_HPP
