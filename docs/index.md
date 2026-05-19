---
layout: default
title: Home
nav_order: 1
---

# Visual Cryptography & Random Grid Schemes

A research implementation comparing three visual cryptography schemes in pure C++11.

## What Is Visual Cryptography?

Visual cryptography encrypts images by splitting them into **shares** that individually look like random noise, but when stacked together, reveal the secret image — **no computer needed for decryption**.

## Implemented Schemes

| Scheme | Based On | Expansion | Decryption | Security |
|--------|----------|-----------|------------|----------|
| **VCS** | Naor & Shamir (1995) | 2× horizontal | OR (physical stacking) | Perfect |
| **Random Grid** | Kafri & Keren (1987) | None | OR (physical stacking) | Perfect |
| **DHCOD** | Chang et al. (2007) | None | XOR (digital) | Near-perfect |

## Quick Start

### Build

```bash
# Windows
build.bat

# Linux/macOS
chmod +x build.sh && ./build.sh

# Or using Make
make
```

### Run

```bash
# Run all three schemes
bin/vc_program          # Linux/macOS
bin\vc_program.exe      # Windows

# Run with custom images
bin/vc_program -i secret.pgm -c cover.pgm -o results/

# Run specific scheme only
bin/vc_program -s vcs
```

### Analyze

```bash
bin/analyze             # Print comparison analysis
bin/analyze --csv r.csv # Export results to CSV
```

### Test

```bash
bin/vc_test             # Run 13 automated tests
```

## Results at a Glance

| Metric | VCS | Random Grid | DHCOD |
|--------|-----|-------------|-------|
| Share Entropy | 1.000 bpp | 1.000 bpp | 0.999 bpp |
| Contrast | 33% | 34% | 88% |
| PSNR | 1.22 dB | 4.25 dB | 13.29 dB |
| SSIM | 0.00 | 0.30 | 0.87 |

## Project Structure

```
VC-SG/
├── src/           Source code (11 files, ~1,300 lines)
├── docs/          Documentation (GitHub Pages)
├── bin/           Compiled executables
├── input/         Input images (auto-generated if missing)
├── output/        Generated shares & decrypted images
├── reference/     Research papers & specifications
├── CMakeLists.txt CMake build system
├── Makefile       Make build system
├── build.bat      Windows build script
└── build.sh       Linux/macOS build script
```

## Requirements

- C++ compiler with C++11 support (g++, clang++, MSVC)
- No external libraries required

## References

1. Naor, M., & Shamir, A. (1995). "Visual Cryptography." *EUROCRYPT'94*.
2. Kafri, O., & Keren, E. (1987). "Encryption of pictures and shapes by random grids." *Optics Letters*.
3. Chang, C. C., et al. (2007). "Halftone visual cryptography with complementary covers."
