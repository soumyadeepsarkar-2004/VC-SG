# 📖 Visual Cryptography - Complete Technical Guide

> **For quick start, see [../README.md](../README.md) | For navigation, see [INDEX.md](INDEX.md)**

## Project Overview

This project implements and compares three visual cryptography schemes:

1. **VCS (Visual Cryptography Scheme)** - Naor-Shamir (2,2) scheme with pixel expansion
2. **RG (Random Grid)** - Kafri-Keren scheme without pixel expansion
3. **DHCOD (Digital Halftoning Cryptography with Ordered Dithering)** - Meaningful shares scheme

## 📁 Current Project Structure

```
VC_NS/
├── src/                        # Source code
│   ├── main.cpp               # Main driver program
│   ├── image_utils.cpp/hpp    # Image I/O and processing utilities
│   ├── vcs.cpp/hpp            # Visual Cryptography Scheme implementation
│   ├── rg.cpp/hpp             # Random Grid scheme implementation
│   ├── dhcod.cpp/hpp          # DHCOD meaningful shares implementation
│   └── analyze.cpp            # Analysis and comparison tool
├── docs/                       # This directory - technical documentation
│   ├── README.md              # This file
│   ├── TECHNICAL_GUIDE.md     # Deep algorithm documentation
│   ├── QUICK_REFERENCE.md     # Command cheat sheet
│   ├── EXPERIMENTAL_RESULTS.md # Results & analysis
│   ├── PROJECT_SUMMARY.md     # Achievements & status
│   ├── PROJECT_STRUCTURE.md   # Folder organization
│   ├── VALIDATION_REPORT.txt  # Testing validation
│   └── INDEX.md               # Navigation guide
├── input/                      # Input images
│   ├── input.pgm              # Input secret image
│   └── cover.pgm              # Cover image for DHCOD
├── output/                     # Generated outputs (organized by scheme)
├── bin/                        # Compiled executables
├── reference/                  # Research papers & specifications
└── Documentation files         # See ../README.md for complete list
```

## Theoretical Background

### 1. Visual Cryptography Scheme (VCS)
- **Algorithm**: Naor-Shamir (2,2) threshold scheme
- **Key Feature**: Perfect security with pixel expansion (2x width)
- **Encryption**: Secret pixel → 2 sub-pixels per share
  - White pixel: Shares are identical (random pattern)
  - Black pixel: Shares are complementary
- **Decryption**: Visual OR operation (stacking transparencies)
- **Limitations**: 50% contrast loss, 2x expansion factor

### 2. Random Grid (RG)
- **Algorithm**: Kafri-Keren random grid method
- **Key Feature**: No pixel expansion, same size as secret
- **Encryption**:
  - Share 1: Random binary pixels
  - Share 2: Depends on secret
    - If secret[i,j] = White → Share2[i,j] = Share1[i,j]
    - If secret[i,j] = Black → Share2[i,j] = NOT Share1[i,j]
- **Decryption**: Visual OR operation
- **Advantages**: No expansion, simpler implementation
- **Limitations**: 50% contrast loss

### 3. DHCOD (Digital Halftoning Cryptography)
- **Algorithm**: Ordered dithering with meaningful shares
- **Key Feature**: Shares appear as meaningful images (cover image)
- **Encryption**:
  1. Halftone cover image → Share 1
  2. For each pixel:
     - If secret[i,j] = White → Share2[i,j] = Share1[i,j]
     - If secret[i,j] = Black → Share2[i,j] = NOT Share1[i,j]
- **Decryption**: XOR operation (digital)
- **Advantages**: Meaningful shares, better visual quality
- **Halftoning**: 4x4 Bayer matrix ordered dithering

## Implementation Details

### Image Representation
- **Internal Convention**: 
  - 0 = White (transparent)
  - 1 = Black (opaque)
- **PGM Format**: 
  - 0 = Black
  - 255 = White
  - Conversion handled automatically in save/load

### Algorithms Implemented

#### VCS Share Generation
```
For each pixel (r, c) in secret:
  If pixel = White:
    Both shares get same random pattern [1,0] or [0,1]
  If pixel = Black:
    Shares get complementary patterns
  Expand horizontally by 2x
```

#### RG Share Generation
```
For each pixel (r, c) in secret:
  Share1[r,c] = random(0 or 1)
  If secret[r,c] = White:
    Share2[r,c] = Share1[r,c]
  Else:
    Share2[r,c] = 1 - Share1[r,c]
```

#### DHCOD Share Generation
```
Halftone(secret) → Secret_binary
Halftone(cover) → Share1
For each pixel (r, c):
  If Secret_binary[r,c] = White:
    Share2[r,c] = Share1[r,c]
  Else:
    Share2[r,c] = 1 - Share1[r,c]
```

### Image Processing Utilities

#### Binarization
- Threshold-based conversion (default: 128)
- Values < threshold → Black (1)
- Values >= threshold → White (0)

#### Halftoning (Ordered Dithering)
- 4x4 Bayer matrix
- Converts grayscale to binary while preserving visual appearance
- Used in DHCOD for creating meaningful shares

## Compilation and Usage

### Prerequisites
- C++ compiler with C++11 support (g++, clang++, MSVC)
- No external dependencies required

### Compilation

#### Windows (MinGW/g++)
```bash
g++ -std=c++11 -o vc_program main.cpp image_utils.cpp vcs.cpp rg.cpp dhcod.cpp
```

#### Linux/Mac
```bash
g++ -std=c++11 -o vc_program main.cpp image_utils.cpp vcs.cpp rg.cpp dhcod.cpp
./vc_program
```

### Running the Program

```bash
./vc_program
```

The program will:
1. Create sample `input.pgm` and `cover.pgm` if they don't exist
2. Load and process the input image
3. Generate shares for all three schemes
4. Decrypt and save results

### Input Files
- **input.pgm**: Secret image to be encrypted (binary/grayscale)
- **cover.pgm**: Cover image for DHCOD (grayscale)

### Output Files

#### VCS Outputs
- `vcs_share1.pgm` - First share (2x width)
- `vcs_share2.pgm` - Second share (2x width)
- `vcs_decrypted.pgm` - Reconstructed secret

#### RG Outputs
- `rg_share1.pgm` - First share (same size)
- `rg_share2.pgm` - Second share (same size)
- `rg_decrypted.pgm` - Reconstructed secret

#### DHCOD Outputs
- `dhcod_share1_meaningful.pgm` - First meaningful share
- `dhcod_share2_meaningful.pgm` - Second meaningful share
- `dhcod_decrypted.pgm` - Reconstructed secret

#### Intermediate Files
- `binary_input.pgm` - Binarized version of input

## Comparative Analysis

### Feature Comparison

| Feature | VCS | RG | DHCOD |
|---------|-----|----|----|
| Pixel Expansion | 2x | 1x | 1x |
| Contrast Loss | 50% | 50% | Variable |
| Share Appearance | Random noise | Random noise | Meaningful |
| Decryption | Visual (OR) | Visual (OR) | Digital (XOR) |
| Security | Perfect | Perfect | Good |
| Complexity | Low | Low | Medium |

### Advantages

**VCS:**
- Perfect security with information-theoretic proof
- Simple implementation
- Visual decryption possible

**RG:**
- No pixel expansion
- Perfect security
- Simpler share generation
- Visual decryption possible

**DHCOD:**
- Meaningful shares (not suspicious)
- Better visual quality
- Flexible cover image choice
- No pixel expansion

### Limitations

**VCS:**
- 2x size increase
- 50% contrast reduction
- Random-looking shares

**RG:**
- 50% contrast reduction
- Random-looking shares

**DHCOD:**
- Requires digital XOR for perfect recovery
- Slightly more complex
- Cover image quality affects share quality

## Performance Metrics

### Time Complexity
- **VCS**: O(W × H) where W, H are image dimensions
- **RG**: O(W × H)
- **DHCOD**: O(W × H) with halftoning overhead

### Space Complexity
- **VCS**: O(2W × H) - doubled width
- **RG**: O(W × H) - no expansion
- **DHCOD**: O(W × H) - no expansion

## Security Analysis

### VCS and RG
- **Perfect Security**: Individual shares reveal no information about the secret
- Each share appears as random noise
- Only when stacked/combined is the secret revealed
- Based on Shamir's secret sharing principles

### DHCOD
- **Good Security**: Shares look like the cover image
- Less suspicious than random noise
- Digital decryption provides perfect reconstruction
- Trade-off: Shares not completely random

## Applications

1. **Secure Image Distribution**
   - Split confidential images into shares
   - Distribute to different parties
   
2. **Two-Factor Authentication**
   - One share on physical card
   - One share on mobile device
   
3. **Copyright Protection**
   - Embed watermarks using visual cryptography
   
4. **Steganography**
   - DHCOD: Hide secrets in innocent-looking images

## PGM File Format

The program uses PGM (Portable Gray Map) format:

```
P2
# Comment
width height
255
pixel_data...
```

- P2: ASCII format
- P5: Binary format (also supported for loading)
- Values: 0-255 (0=black, 255=white)

## Testing and Validation

### Sample Test Case
1. Create a simple geometric secret (square, circle)
2. Run all three schemes
3. Verify:
   - VCS shares are 2x width
   - RG shares are same size
   - DHCOD shares resemble cover
   - All decrypted images match original pattern

### Quality Metrics
- **Contrast Ratio**: Measure black/white ratio in decrypted image
- **PSNR**: Peak Signal-to-Noise Ratio for quality assessment
- **Size Comparison**: Compare share sizes across schemes

## Future Enhancements

1. **Color Support**: Extend to RGB images
2. **(k,n) Threshold**: Implement k-out-of-n schemes
3. **Advanced Halftoning**: Error diffusion (Floyd-Steinberg)
4. **GUI Interface**: Visual share generation and stacking
5. **Performance Optimization**: Parallel processing for large images
6. **Additional Metrics**: Automated quality assessment

## References

1. Naor, M., & Shamir, A. (1995). Visual Cryptography. EUROCRYPT.
2. Kafri, O., & Keren, E. (1987). Encryption of pictures and shapes by random grids. Optics Letters.
3. Zhou, Z., et al. (2006). Halftone visual cryptography. IEEE Transactions on Image Processing.
4. Bayer, B. E. (1973). An optimum method for two-level rendition of continuous-tone pictures.

---

## 📚 Related Documentation

**Other docs/ Files:**
- **[TECHNICAL_GUIDE.md](TECHNICAL_GUIDE.md)** - Deep dive into algorithms with mathematical proofs
- **[QUICK_REFERENCE.md](QUICK_REFERENCE.md)** - Quick command reference and troubleshooting
- **[EXPERIMENTAL_RESULTS.md](EXPERIMENTAL_RESULTS.md)** - Complete experimental analysis
- **[PROJECT_SUMMARY.md](PROJECT_SUMMARY.md)** - Project achievements and deliverables
- **[PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md)** - Detailed folder organization
- **[VALIDATION_REPORT.txt](VALIDATION_REPORT.txt)** - Testing and validation results
- **[INDEX.md](INDEX.md)** - Complete navigation guide for all documentation

**Root Documentation:**
- **[../README.md](../README.md)** - Main project overview (start here!)
- **[../INSTALLATION.md](../INSTALLATION.md)** - Complete setup guide
- **[../USAGE_EXAMPLES.md](../USAGE_EXAMPLES.md)** - Practical tutorials
- **[../FAQ.md](../FAQ.md)** - 30+ frequently asked questions
- **[../ARCHITECTURE.md](../ARCHITECTURE.md)** - System architecture and design
- **[../PRESENTATION.md](../PRESENTATION.md)** - Academic presentation guide
- **[../CONTRIBUTING.md](../CONTRIBUTING.md)** - How to extend the project
- **[../PROJECT_OVERVIEW.md](../PROJECT_OVERVIEW.md)** - Complete project summary

**📖 For complete navigation, see [INDEX.md](INDEX.md)**

---

## Author

**Group 9 - Visual Cryptography Project**

Engineering Student Implementation  
Course: Network Security / Cryptography  
Date: 2025

## License

This is an academic project for educational purposes.

## Acknowledgments

- Course instructors for guidance
- Research papers for theoretical foundation
- Open-source community for development tools
