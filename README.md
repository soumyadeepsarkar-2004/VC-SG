# 🔐 Visual Cryptography & Random Grid Schemes

> **A comprehensive research implementation and comparison of three visual cryptography schemes**

[![Language](https://img.shields.io/badge/Language-C%2B%2B11-blue.svg)](https://en.cppreference.com/w/cpp/11)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)](https://github.com)
[![License](https://img.shields.io/badge/License-Academic-green.svg)](LICENSE)
[![Status](https://img.shields.io/badge/Status-Complete-success.svg)](docs/PROJECT_SUMMARY.md)

**Visual cryptography** is a cryptographic technique that allows visual information (images) to be encrypted in such a way that decryption becomes a simple mechanical operation that does not require a computer. This project implements and compares three cutting-edge schemes with detailed analysis.

---

## 🎯 START HERE - Choose Your Path

**New to the project?** Pick your role and follow the guide:

| 👤 Your Role | 📍 Start Here | ⏱️ Time |
|-------------|--------------|---------|
| **🚀 First-Time User** | [docs/INSTALLATION.md](docs/INSTALLATION.md) → [docs/USAGE_EXAMPLES.md](docs/USAGE_EXAMPLES.md) | 20 min |
| **💻 Developer** | [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) → [docs/TECHNICAL_GUIDE.md](docs/TECHNICAL_GUIDE.md) | 45 min |
| **🔬 Researcher** | [docs/README.md](docs/README.md) → [docs/EXPERIMENTAL_RESULTS.md](docs/EXPERIMENTAL_RESULTS.md) | 60 min |
| **🎤 Presenter** | [docs/PRESENTATION.md](docs/PRESENTATION.md) → [docs/PROJECT_OVERVIEW.md](docs/PROJECT_OVERVIEW.md) | 30 min |
| **❓ Have Questions?** | [docs/FAQ.md](docs/FAQ.md) - 30+ answers | 10 min |
| **🗺️ Need Navigation?** | [docs/INDEX.md](docs/INDEX.md) - Complete guide | 5 min |

💡 **Quick Tip**: All documentation is in the **[docs/](docs/)** folder - explore it anytime!

---

## 🌟 Key Highlights

- ✅ **Three Complete Implementations**: VCS, Random Grid, and DHCOD schemes
- ✅ **Zero Dependencies**: Pure C++11 standard library only
- ✅ **Cross-Platform**: Works on Windows, Linux, and macOS
- ✅ **Production Quality**: ~780 lines of well-documented code
- ✅ **Comprehensive Analysis**: Built-in metrics tool with entropy, PSNR, contrast ratio
- ✅ **Extensive Documentation**: 4,100+ lines across 17 documentation files
- ✅ **Academic Grade**: Senior engineering student level implementation

## 📁 Project Structure

```
VC_NS/
├── src/                      # Source code
│   ├── main.cpp             # Main program (runs all schemes)
│   ├── vcs.cpp/.hpp         # Visual Cryptography Scheme
│   ├── rg.cpp/.hpp          # Random Grid implementation
│   ├── dhcod.cpp/.hpp       # DHCOD meaningful shares
│   ├── image_utils.cpp/.hpp # Image I/O and processing
│   └── analyze.cpp          # Analysis and comparison tool
│
├── docs/                     # Complete Documentation (16 files)
│   ├── README.md            # Complete user guide
│   ├── INSTALLATION.md      # Setup & installation guide
│   ├── USAGE_EXAMPLES.md    # Practical tutorials
│   ├── FAQ.md               # 30+ questions answered
│   ├── QUICK_REFERENCE.md   # Quick reference guide
│   ├── TECHNICAL_GUIDE.md   # Algorithm documentation
│   ├── ARCHITECTURE.md      # System design document
│   ├── EXPERIMENTAL_RESULTS.md  # Results and analysis
│   ├── PROJECT_SUMMARY.md   # Project completion status
│   ├── PROJECT_STRUCTURE.md # Folder organization
│   ├── PROJECT_OVERVIEW.md  # Complete project summary
│   ├── PRESENTATION.md      # Academic presentation guide
│   ├── CONTRIBUTING.md      # Extension & contribution guide
│   ├── DOCUMENTATION_SUMMARY.md  # Documentation overview
│   ├── VALIDATION_REPORT.txt    # Testing validation
│   └── INDEX.md             # Documentation navigation
│
├── bin/                      # Compiled executables
│   ├── vc_program.exe       # Main program
│   └── analyze.exe          # Analysis tool
│
├── input/                    # Input images
│   ├── input.pgm            # Secret image
│   └── cover.pgm            # Cover image for DHCOD
│
├── output/                   # Generated outputs
│   ├── vcs/                 # VCS scheme outputs
│   │   ├── vcs_share1.pgm
│   │   ├── vcs_share2.pgm
│   │   └── vcs_decrypted.pgm
│   ├── rg/                  # Random Grid outputs
│   │   ├── rg_share1.pgm
│   │   ├── rg_share2.pgm
│   │   └── rg_decrypted.pgm
│   ├── dhcod/               # DHCOD outputs
│   │   ├── dhcod_share1_meaningful.pgm
│   │   ├── dhcod_share2_meaningful.pgm
│   │   └── dhcod_decrypted.pgm
│   └── binary_input.pgm     # Binarized input
│
├── reference/                # Reference documents
│   ├── Final_report.pdf     # Project specification
│   └── VCS_and_RGD.pdf      # Research papers
│
├── Makefile                  # Universal build system
├── build.bat                 # Windows build script
└── build.sh                  # Linux/macOS build script
```

## 🚀 Quick Start

### Windows
```bash
# Build
build.bat

# Run
bin\vc_program.exe
bin\analyze.exe
```

### Linux/macOS
```bash
# Build
chmod +x build.sh
./build.sh

# Run
bin/vc_program
bin/analyze
```

### Using Makefile
```bash
make          # Build all
make run      # Build and run main program
make analyze  # Build and run analysis tool
make clean    # Clean executables
```

## 📖 Comprehensive Documentation

All documentation is now organized in the **`docs/`** folder for easy navigation.

### 📚 Getting Started Guides

| Document | Description | When to Read |
|----------|-------------|--------------|
| **[docs/INSTALLATION.md](docs/INSTALLATION.md)** 🔧 | Complete setup & installation guide | First time setup |
| **[docs/USAGE_EXAMPLES.md](docs/USAGE_EXAMPLES.md)** 💡 | Practical examples & tutorials | Learning how to use |
| **[docs/FAQ.md](docs/FAQ.md)** ❓ | 30+ common questions answered | When stuck or curious |
| **[docs/QUICK_REFERENCE.md](docs/QUICK_REFERENCE.md)** ⚡ | Command cheat sheet | Quick lookup |

### 🎓 Technical Documentation

| Document | Description | Audience |
|----------|-------------|----------|
| **[docs/README.md](docs/README.md)** 📖 | Complete user guide & overview | Everyone |
| **[docs/TECHNICAL_GUIDE.md](docs/TECHNICAL_GUIDE.md)** 🔬 | Algorithm details & theory | Developers/Researchers |
| **[docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)** 🏗️ | System design & architecture | Developers |
| **[docs/EXPERIMENTAL_RESULTS.md](docs/EXPERIMENTAL_RESULTS.md)** 📊 | Results & analysis | Researchers |

### 🎯 Project Information

| Document | Description | Purpose |
|----------|-------------|---------|
| **[docs/PROJECT_SUMMARY.md](docs/PROJECT_SUMMARY.md)** 📋 | Completion status & achievements | Project overview |
| **[docs/PROJECT_STRUCTURE.md](docs/PROJECT_STRUCTURE.md)** 🗂️ | Folder organization explained | Understanding layout |
| **[docs/VALIDATION_REPORT.txt](docs/VALIDATION_REPORT.txt)** ✅ | Testing & quality assurance | Verification results |
| **[docs/PRESENTATION.md](docs/PRESENTATION.md)** 🎤 | How to present this project | Academic presentation |
| **[docs/CONTRIBUTING.md](docs/CONTRIBUTING.md)** 🤝 | Extension & contribution guide | Developers |
| **[docs/INDEX.md](docs/INDEX.md)** 🗺️ | Navigation guide | Finding information |

### 📂 Complete docs/ Folder Contents

All documentation is organized in the **`docs/`** directory:

| File | Size | Purpose |
|------|------|---------|
| **[docs/README.md](docs/README.md)** | 11.6 KB | Complete user guide with theory |
| **[docs/INSTALLATION.md](docs/INSTALLATION.md)** | 7.6 KB | Complete setup guide |
| **[docs/USAGE_EXAMPLES.md](docs/USAGE_EXAMPLES.md)** | 10.8 KB | Practical tutorials |
| **[docs/FAQ.md](docs/FAQ.md)** | 16.3 KB | 30+ questions answered |
| **[docs/QUICK_REFERENCE.md](docs/QUICK_REFERENCE.md)** | 10.8 KB | Command cheat sheet |
| **[docs/TECHNICAL_GUIDE.md](docs/TECHNICAL_GUIDE.md)** | 13.9 KB | Deep dive into algorithms |
| **[docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)** | 18.4 KB | System design document |
| **[docs/EXPERIMENTAL_RESULTS.md](docs/EXPERIMENTAL_RESULTS.md)** | 12.4 KB | Results & metrics |
| **[docs/PROJECT_SUMMARY.md](docs/PROJECT_SUMMARY.md)** | 13.7 KB | Project achievements |
| **[docs/PROJECT_STRUCTURE.md](docs/PROJECT_STRUCTURE.md)** | 8.7 KB | Folder organization |
| **[docs/PROJECT_OVERVIEW.md](docs/PROJECT_OVERVIEW.md)** | 19.3 KB | Complete project summary |
| **[docs/PRESENTATION.md](docs/PRESENTATION.md)** | 23.0 KB | Academic presentation guide |
| **[docs/CONTRIBUTING.md](docs/CONTRIBUTING.md)** | 17.1 KB | Extension guide |
| **[docs/DOCUMENTATION_SUMMARY.md](docs/DOCUMENTATION_SUMMARY.md)** | 13.0 KB | Documentation overview |
| **[docs/VALIDATION_REPORT.txt](docs/VALIDATION_REPORT.txt)** | 14.1 KB | Testing validation |
| **[docs/INDEX.md](docs/INDEX.md)** | 14.7 KB | Navigation guide |

**Total: 16 files | ~225 KB | 4,100+ lines**

### 🎯 Quick Navigation

```
New to the project?          → Start with docs/INSTALLATION.md
Want to use it?              → Read docs/USAGE_EXAMPLES.md
Have questions?              → Check docs/FAQ.md
Need quick commands?         → See docs/QUICK_REFERENCE.md
Understanding algorithms?    → Read docs/TECHNICAL_GUIDE.md
Understanding structure?     → Check docs/PROJECT_STRUCTURE.md
System architecture?         → Read docs/ARCHITECTURE.md
Presenting this project?     → Use docs/PRESENTATION.md
Extending the project?       → See docs/CONTRIBUTING.md
Complete technical guide?    → docs/README.md
All documentation?           → docs/INDEX.md
Project overview?            → docs/PROJECT_OVERVIEW.md
Documentation summary?       → docs/DOCUMENTATION_SUMMARY.md
```

## 🎯 Implemented Schemes

### 1️⃣ VCS (Visual Cryptography Scheme) - Naor & Shamir
**Algorithm**: (2,2) Threshold scheme with pixel expansion  
**Security**: Perfect (information-theoretic)  
**Expansion**: 2× horizontal  
**Decryption**: Visual OR (stacking transparencies)  
**Use Case**: Maximum security, physical decryption

### 2️⃣ Random Grid - Kafri & Keren
**Algorithm**: Random grid without expansion  
**Security**: Perfect (information-theoretic)  
**Expansion**: None (1:1)  
**Decryption**: Visual OR (stacking)  
**Use Case**: Space-efficient secure sharing

### 3️⃣ DHCOD - Digital Halftoning Cryptography
**Algorithm**: Ordered dithering with meaningful shares  
**Security**: Good (near-maximum entropy: 0.99 bits/pixel)  
**Expansion**: None  
**Decryption**: Digital XOR  
**Use Case**: Steganographic applications, meaningful camouflage

## 📊 Performance Metrics

| Metric | VCS | Random Grid | DHCOD |
|--🧪 Sample Outputs

After running the program, you'll find organized outputs in the `output/` directory:

```
output/
├── vcs/
│   ├── vcs_share1.pgm          # Random share 1 (400×200 pixels)
│   ├── vcs_share2.pgm          # Random share 2 (400×200 pixels)
│   └── vcs_decrypted.pgm       # Reconstructed secret
├── rg/
│   ├── rg_share1.pgm           # Random share 1 (200×200 pixels)
│   ├── rg_share2.pgm           # Random share 2 (200×200 pixels)
│   └── rg_decrypted.pgm        # Reconstructed secret
└── dhcod/
    ├── dhcod_share1_meaningful.pgm  # Meaningful share 1 (200×200 pixels)
    ├── dhcod_share2_meaningful.pgm  # Meaningful share 2 (200×200 pixels)
    └── dhcod_decrypted.pgm          # Reconstructed secret
```

**All shares appear random (noise-like) except DHCOD shares which resemble the cover image!**

## 📈 Analysis Tool

Run the analysis tool to get comprehensive metrics:

```bash
bin/analyze         # Linux/macOS
bin\analyze.exe     # Windows
```

**Output includes:**
- Information entropy (bits per pixel)
- Mean Squared Error (MSE)
- Peak Signal-to-Noise Ratio (PSNR)
- Contrast ratio analysis
- File size comparison
- Decryption quality metrics

## 🔬 Research References

This implementation is based on seminal research papers:

1. **Naor, M., & Shamir, A. (1995)**. "Visual Cryptography." *Advances in Cryptology - EUROCRYPT'94*.
2. **Kafri, O., & Keren, E. (1987)**. "Encryption of pictures and shapes by random grids." *Optics Letters*.
3. **Chang, C. C., et al. (2007)**. "Halftone visual cryptography with complementary covers."

See `reference/` directory for full papers and project specifications.

## 🎓 Academic Context

**Project Type**: Senior Engineering Research Implementation  
**Course**: Visual Cryptography and Information Security  
**Group**: Group 9  
**Level**: Advanced undergraduate / Graduate level  
**Date**: December 2025

## 🤝 Contributing

This is an academic project. For educational use:
1. Study the implementation in `src/` directory
2. Review comprehensive documentation in `docs/` directory
3. Run experiments and modify parameters
4. Extend with additional schemes (e.g., (k,n) threshold)

## 📝 License

**Academic Project** - For educational and research purposes.

This implementation is provided for learning and research. If you use this code in academic work, please cite appropriately.

## 👨‍💻 Authors

**Group 9** - Visual Cryptography Research Team  
Senior Engineering Students  
*Complete implementation with comprehensive analysis*

## 📞 Support

For questions or issues:
- 📖 Check [docs/QUICK_REFERENCE.md](docs/QUICK_REFERENCE.md) for troubleshooting
- 📚 Read [docs/README.md](docs/README.md) for detailed guide
- 🔧 See [docs/TECHNICAL_GUIDE.md](docs/TECHNICAL_GUIDE.md) for algorithm details

---

**⭐ If this project helped you understand visual cryptography, consider giving it a star!**

📖 For complete document
- `output/vcs/` - VCS shares and decrypted image (2x width)
- `output/rg/` - Random Grid shares and decrypted image (same size)
- `output/dhcod/` - DHCOD meaningful shares and decrypted image

## 🔧 Requirements

- C++ compiler with C++11 support (g++, clang++, MSVC)
- No external libraries required

## 📝 License

Academic project for educational purposes.

## 👨‍💻 Author

Group 9 - Visual Cryptography Project
Senior Engineering Student Implementation

---

**For detailed information, see [docs/README.md](docs/README.md)**

