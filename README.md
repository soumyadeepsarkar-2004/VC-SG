# 🔐 VC-SG — Visual Cryptography & Random Grid Schemes

[![Language](https://img.shields.io/badge/Language-C%2B%2B11-blue.svg)](https://en.cppreference.com/w/cpp/11)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)](#quick-start)
[![Tests](https://img.shields.io/badge/Tests-13%2F13%20Passing-brightgreen.svg)](#testing)
[![Docs](https://img.shields.io/badge/Docs-GitHub%20Pages-blue.svg)](https://soumyadeepsarkar-2004.github.io/VC-SG/)

A research implementation comparing three visual cryptography schemes in pure C++11 with zero dependencies.

**[📖 Documentation](https://soumyadeepsarkar-2004.github.io/VC-SG/)** · **[🔬 Algorithms](https://soumyadeepsarkar-2004.github.io/VC-SG/algorithms)** · **[📊 Results](https://soumyadeepsarkar-2004.github.io/VC-SG/results)** · **[📋 API Reference](https://soumyadeepsarkar-2004.github.io/VC-SG/api)**

---

## Implemented Schemes

| Scheme | Paper | Expansion | Decryption | Security |
|--------|-------|-----------|------------|----------|
| **VCS** | Naor & Shamir (1995) | 2× horizontal | OR (physical stacking) | Perfect |
| **Random Grid** | Kafri & Keren (1987) | None | OR (physical stacking) | Perfect |
| **DHCOD** | Chang et al. (2007) | None | XOR (digital) | Near-perfect |

## Quick Start

```bash
# Build (Windows)
build.bat

# Build (Linux/macOS)
make

# Run all schemes
bin/vc_program

# Analyze results
bin/analyze

# Run tests
bin/vc_test
```

### CLI Options

```bash
bin/vc_program -i secret.pgm -c cover.pgm -o results/ -s dhcod
bin/analyze --csv output/results.csv
```

## Results Summary

| Metric | VCS | Random Grid | DHCOD |
|--------|-----|-------------|-------|
| Entropy | 1.000 bpp | 1.000 bpp | 0.999 bpp |
| Contrast | 33% | 34% | 88% |
| SSIM | 0.00 | 0.30 | 0.87 |
| PSNR | 1.22 dB | 4.25 dB | 13.29 dB |

## Project Structure

```
src/
├── main.cpp          Main program with CLI
├── vcs.cpp/.hpp      Naor-Shamir VCS
├── rg.cpp/.hpp       Kafri-Keren Random Grid
├── dhcod.cpp/.hpp    DHCOD meaningful shares
├── image_utils.cpp/.hpp  Image I/O, halftoning, PRNG
├── analyze.cpp       Analysis & comparison tool
└── test.cpp          Automated test suite (13 tests)
```

## Testing

```
============================================
  VC-SG Automated Test Suite
============================================

[Image Utilities]     4/4 PASSED
[VCS — Naor-Shamir]   3/3 PASSED
[RG — Random Grid]    3/3 PASSED
[DHCOD]               2/2 PASSED
[Edge Cases]          1/1 PASSED

Results: 13 passed, 0 failed
```

## Requirements

- C++ compiler with C++11 support (g++, clang++, MSVC)
- No external libraries

## References

1. Naor, M., & Shamir, A. (1995). "Visual Cryptography." *EUROCRYPT'94*.
2. Kafri, O., & Keren, E. (1987). "Encryption of pictures and shapes by random grids." *Optics Letters*.
3. Chang, C. C., et al. (2007). "Halftone visual cryptography with complementary covers."

## License

Academic project — for educational and research purposes.
