---
layout: default
title: API Reference
nav_order: 4
---

# API Reference

## Image Struct

```cpp
struct Image {
    int width;
    int height;
    int maxVal;       // PGM max value (default: 255)
    bool isBinary;    // true: pixels are 0/1; false: pixels are 0-255

    Image();                            // Empty image
    Image(int w, int h, int val = 0);   // Create w×h image filled with val
    bool empty() const;                 // True if width or height is 0
};
```

## Image I/O

```cpp
// Load PGM image (P2 ASCII or P5 binary). Returns empty Image on failure.
Image loadPGM(const std::string& filename);

// Save as P2 ASCII PGM. Returns false on failure.
// If img.isBinary: 0→255 (white), 1→0 (black).
// If !img.isBinary: writes pixel values as-is (clamped to 0-255).
bool savePGM(const std::string& filename, const Image& img);
```

## Image Processing

```cpp
// Convert grayscale to binary. Output: isBinary=true.
// Dark pixels (< threshold) → 1 (black), bright → 0 (white).
Image binarizeImage(const Image& input, int threshold = 128);

// Ordered dithering with 4×4 Bayer matrix. Output: isBinary=true.
Image halftoneImage(const Image& input);

// Nearest-neighbor resize. Preserves isBinary and maxVal.
Image resizeImage(const Image& input, int newWidth, int newHeight);
```

## Utilities

```cpp
// Returns 0 or 1 using std::mt19937 + std::random_device.
int randomBit();

// Create directory path recursively.
void ensureDirectories(const std::string& path);
```

## VCS Namespace

```cpp
namespace VCS {
    // Generate (2,2) shares. Output width = 2× input width.
    void generateShares(const Image& secret, Image& share1, Image& share2);

    // Decrypt via OR operation.
    Image decryptShares(const Image& share1, const Image& share2);
}
```

## RG Namespace

```cpp
namespace RG {
    // Generate (2,2) shares. Output same size as input.
    void generateShares(const Image& secret, Image& share1, Image& share2);

    // Decrypt via OR operation.
    Image decryptShares(const Image& share1, const Image& share2);
}
```

## DHCOD Namespace

```cpp
namespace DHCOD {
    // Generate meaningful shares. Cover is auto-resized if dimensions mismatch.
    void generateShares(const Image& secret, const Image& cover,
                        Image& share1, Image& share2);

    // Decrypt via XOR operation.
    Image decryptShares(const Image& share1, const Image& share2);
}
```

## CLI — vc_program

```
Usage: vc_program [options]
  -i <file>    Input secret image   (default: input/input.pgm)
  -c <file>    Cover image (DHCOD)   (default: input/cover.pgm)
  -o <dir>     Output directory       (default: output/)
  -s <scheme>  Scheme: vcs, rg, dhcod, all (default: all)
  -h           Show help
```

## CLI — analyze

```
Usage: analyze [options]
  --csv <file>  Export results to CSV
  -h            Show help
```

Outputs: dimensions, file sizes, contrast ratios, entropy, PSNR, SSIM, and performance benchmarks.
