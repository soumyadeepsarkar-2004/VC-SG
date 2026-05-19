#include "image_utils.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

// ============================================================
// Secure PRNG (replaces insecure rand()/srand())
// ============================================================
int randomBit() {
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<int> dist(0, 1);
    return dist(rng);
}

// ============================================================
// Directory creation
// ============================================================
void ensureDirectories(const std::string& path) {
    std::string current;
    for (size_t i = 0; i < path.size(); ++i) {
        current += path[i];
        if (path[i] == '/' || path[i] == '\\') {
            MKDIR(current.c_str());
        }
    }
    if (!current.empty()) {
        MKDIR(current.c_str());
    }
}

// ============================================================
// PGM I/O — with proper validation
// ============================================================

// Skip comments and whitespace in PGM header (with EOF safety)
static void ignoreComments(std::ifstream& file) {
    while (file.good()) {
        int c = file.peek();
        if (c == EOF) return;
        if (c == '#') {
            std::string line;
            std::getline(file, line);
        } else if (c == '\n' || c == '\r' || c == '\t' || c == ' ') {
            file.get();
        } else {
            break;
        }
    }
}

Image loadPGM(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return Image(0, 0);
    }

    std::string format;
    file >> format;
    if (format != "P2" && format != "P5") {
        std::cerr << "Error: Unsupported PGM format " << format
                  << " (Only P2/P5 supported)" << std::endl;
        return Image(0, 0);
    }

    ignoreComments(file);

    int w = 0, h = 0, maxVal = 0;
    file >> w >> h;
    ignoreComments(file);
    file >> maxVal;

    // Validate dimensions
    if (w <= 0 || h <= 0 || maxVal <= 0 || maxVal > 65535) {
        std::cerr << "Error: Invalid PGM header in " << filename
                  << " (w=" << w << ", h=" << h << ", maxVal=" << maxVal << ")" << std::endl;
        return Image(0, 0);
    }
    if (static_cast<long long>(w) * h > 100000000LL) {
        std::cerr << "Error: Image too large in " << filename << std::endl;
        return Image(0, 0);
    }

    Image img(w, h);
    img.maxVal = maxVal;
    img.isBinary = false;

    if (format == "P2") {
        for (int i = 0; i < h && file.good(); ++i) {
            for (int j = 0; j < w && file.good(); ++j) {
                file >> img.pixels[i][j];
            }
        }
    } else { // P5 binary
        // Consume exactly one whitespace character after maxVal
        file.get();
        for (int i = 0; i < h && file.good(); ++i) {
            for (int j = 0; j < w && file.good(); ++j) {
                if (maxVal > 255) {
                    // 16-bit: read two bytes big-endian
                    unsigned char hi = static_cast<unsigned char>(file.get());
                    unsigned char lo = static_cast<unsigned char>(file.get());
                    img.pixels[i][j] = (hi << 8) | lo;
                } else {
                    unsigned char c = static_cast<unsigned char>(file.get());
                    img.pixels[i][j] = static_cast<int>(c);
                }
            }
        }
    }

    if (!file.good() && !file.eof()) {
        std::cerr << "Warning: Possible read error in " << filename << std::endl;
    }

    return img;
}

bool savePGM(const std::string& filename, const Image& img) {
    if (img.empty()) {
        std::cerr << "Error: Cannot save empty image to " << filename << std::endl;
        return false;
    }

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not create file " << filename << std::endl;
        return false;
    }

    file << "P2\n";
    file << "# Created by VC-SG Visual Cryptography\n";
    file << img.width << " " << img.height << "\n";
    file << "255\n";

    for (int i = 0; i < img.height; ++i) {
        for (int j = 0; j < img.width; ++j) {
            int val;
            if (img.isBinary) {
                // Internal: 0=White, 1=Black → PGM: 255=White, 0=Black
                val = (img.pixels[i][j] == 1) ? 0 : 255;
            } else {
                // Grayscale: clamp to [0, 255]
                val = std::max(0, std::min(255, img.pixels[i][j]));
            }
            file << val;
            if (j < img.width - 1) file << " ";
        }
        file << "\n";
    }

    return file.good();
}

// ============================================================
// Image Processing
// ============================================================

Image binarizeImage(const Image& input, int threshold) {
    Image res(input.width, input.height);
    res.isBinary = true;
    for (int i = 0; i < input.height; ++i) {
        for (int j = 0; j < input.width; ++j) {
            int val = input.pixels[i][j];
            // If input is already binary (0/1), just copy
            if (input.isBinary) {
                res.pixels[i][j] = val;
            } else {
                // PGM: low values = dark/black, high values = light/white
                // Internal: 1 = Black, 0 = White
                res.pixels[i][j] = (val < threshold) ? 1 : 0;
            }
        }
    }
    return res;
}

Image halftoneImage(const Image& input) {
    // 4x4 Bayer ordered dithering matrix
    static constexpr int bayer[4][4] = {
        { 0,  8,  2, 10},
        {12,  4, 14,  6},
        { 3, 11,  1,  9},
        {15,  7, 13,  5}
    };

    Image res(input.width, input.height);
    res.isBinary = true;

    for (int y = 0; y < input.height; ++y) {
        for (int x = 0; x < input.width; ++x) {
            int val = input.pixels[y][x];

            // If already binary, just pass through
            if (input.isBinary) {
                res.pixels[y][x] = val;
                continue;
            }

            // Scale Bayer value (0-15) to threshold (0-255)
            int threshold = bayer[y % 4][x % 4] * 17; // 15*17 = 255

            // PGM: high value = white, low value = black
            // Internal: 0 = White, 1 = Black
            res.pixels[y][x] = (val > threshold) ? 0 : 1;
        }
    }
    return res;
}

Image resizeImage(const Image& input, int newWidth, int newHeight) {
    if (input.empty() || newWidth <= 0 || newHeight <= 0) {
        return Image(0, 0);
    }

    Image result(newWidth, newHeight);
    result.isBinary = input.isBinary;
    result.maxVal = input.maxVal;

    for (int r = 0; r < newHeight; ++r) {
        for (int c = 0; c < newWidth; ++c) {
            int srcR = r * input.height / newHeight;
            int srcC = c * input.width / newWidth;
            srcR = std::min(srcR, input.height - 1);
            srcC = std::min(srcC, input.width - 1);
            result.pixels[r][c] = input.pixels[srcR][srcC];
        }
    }
    return result;
}
