# 🤝 Contributing Guide

## How to Extend & Contribute to This Project

This guide helps you understand how to extend, modify, and contribute to the Visual Cryptography project.

---

## 🎯 Quick Start for Contributors

### Before You Start

**Read these first:**
1. [PROJECT_OVERVIEW.md](PROJECT_OVERVIEW.md) - Understand the project
2. [ARCHITECTURE.md](ARCHITECTURE.md) - System design
3. [docs/TECHNICAL_GUIDE.md](docs/TECHNICAL_GUIDE.md) - Algorithm details

**Time needed:** 1-2 hours to understand the codebase

---

## 📚 Understanding the Codebase

### Code Organization

```
src/
├── main.cpp           → Entry point & workflow
├── image_utils.cpp/hpp → Image I/O & processing
├── vcs.cpp/hpp        → VCS implementation
├── rg.cpp/hpp         → Random Grid implementation
├── dhcod.cpp/hpp      → DHCOD implementation
└── analyze.cpp        → Analysis tool
```

### Dependency Graph

```
main.cpp
  ├─→ vcs.cpp ────────┐
  ├─→ rg.cpp ─────────┤
  ├─→ dhcod.cpp ──────┤
  └─→ image_utils.cpp ←┘

analyze.cpp
  ├─→ vcs.cpp ────────┐
  ├─→ rg.cpp ─────────┤
  ├─→ dhcod.cpp ──────┤
  └─→ image_utils.cpp ←┘
```

**Key insight:** Each scheme module (vcs, rg, dhcod) only depends on image_utils.

---

## 🔧 Extension Ideas

### 🟢 Easy Extensions (Beginner)

#### 1. Add New Sample Image Patterns

**File:** `src/main.cpp`
**Function:** `createSampleImage()`

**Current pattern:** Centered black square

**Add patterns like:**
- Circles
- Diagonal lines
- Text/letters
- Random noise

**Example:**
```cpp
// Add to createSampleImage()
void createSampleCircle(const std::string &filename, int w, int h) {
    Image img(w, h);
    int centerR = h / 2;
    int centerC = w / 2;
    int radius = h / 4;
    
    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            int dr = r - centerR;
            int dc = c - centerC;
            if (dr*dr + dc*dc < radius*radius) {
                img.pixels[r][c] = 1;  // Black
            } else {
                img.pixels[r][c] = 0;  // White
            }
        }
    }
    savePGM(filename, img);
}
```

#### 2. Add More Cover Image Patterns (DHCOD)

**File:** `src/main.cpp`
**Function:** `createSampleCover()`

**Add patterns like:**
- Checkerboard
- Circular gradient
- Natural textures
- Perlin noise

**Example:**
```cpp
void createCheckerboardCover(const std::string &filename, int w, int h) {
    Image img(w, h);
    int squareSize = 20;
    
    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            bool isBlack = ((r/squareSize) + (c/squareSize)) % 2 == 0;
            img.pixels[r][c] = isBlack ? 0 : 255;
        }
    }
    savePGM(filename, img);
}
```

#### 3. Add Command-Line Arguments

**File:** `src/main.cpp`
**Function:** `main()`

**Allow users to specify:**
- Input image path
- Output directory
- Image size
- Which schemes to run

**Example:**
```cpp
int main(int argc, char *argv[]) {
    std::string inputFile = "input/input.pgm";
    std::string outputDir = "output/";
    
    // Parse arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-i" && i+1 < argc) {
            inputFile = argv[++i];
        } else if (arg == "-o" && i+1 < argc) {
            outputDir = argv[++i];
        }
    }
    
    // Use parsed arguments
    Image secret = loadPGM(inputFile);
    // ... rest of code
}
```

#### 4. Add Progress Indicators

**File:** All scheme files

**Show progress during processing:**

```cpp
// In vcs.cpp, generateShares()
void generateShares(const Image &secret, Image &share1, Image &share2) {
    // ... initialization ...
    
    int totalPixels = h * w;
    int processedPixels = 0;
    int lastPercent = 0;
    
    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            // ... processing ...
            
            processedPixels++;
            int percent = (processedPixels * 100) / totalPixels;
            if (percent > lastPercent) {
                std::cout << "\rProgress: " << percent << "%" << std::flush;
                lastPercent = percent;
            }
        }
    }
    std::cout << std::endl;
}
```

---

### 🟡 Medium Extensions (Intermediate)

#### 5. Implement (2, n) Threshold Scheme

**Description:** Create n shares, any 2 can decrypt.

**Files to create:**
- `src/vcs_2n.cpp`
- `src/vcs_2n.hpp`

**Algorithm:**
```cpp
void generate_2_n_Shares(const Image &secret, 
                         std::vector<Image> &shares,
                         int n) {
    // For each pixel in secret:
    // - Generate n-1 random shares
    // - Last share ensures any 2 can reconstruct
    
    // Implementation based on basis matrices
    // See: Naor & Shamir 1995, Section 4
}
```

**Difficulty:** Medium
**Time:** 1-2 weeks
**Good for:** Research extension

#### 6. Add Different Halftoning Methods

**File:** `src/image_utils.cpp`
**Function:** `halftoneImage()`

**Current:** 4×4 Bayer ordered dithering

**Add:**
- 8×8 Bayer matrix (finer detail)
- Floyd-Steinberg error diffusion
- Stucki dithering
- Sierra dithering

**Example (Floyd-Steinberg):**
```cpp
Image halftoneFloydSteinberg(const Image &gray) {
    Image result = gray;  // Copy
    
    for (int r = 0; r < gray.height; ++r) {
        for (int c = 0; c < gray.width; ++c) {
            int oldPixel = result.pixels[r][c];
            int newPixel = (oldPixel > 127) ? 255 : 0;
            result.pixels[r][c] = newPixel;
            
            int error = oldPixel - newPixel;
            
            // Distribute error to neighbors
            if (c + 1 < gray.width)
                result.pixels[r][c+1] += error * 7/16;
            if (r + 1 < gray.height && c > 0)
                result.pixels[r+1][c-1] += error * 3/16;
            if (r + 1 < gray.height)
                result.pixels[r+1][c] += error * 5/16;
            if (r + 1 < gray.height && c + 1 < gray.width)
                result.pixels[r+1][c+1] += error * 1/16;
        }
    }
    
    // Binarize
    for (int r = 0; r < result.height; ++r) {
        for (int c = 0; c < result.width; ++c) {
            result.pixels[r][c] = (result.pixels[r][c] > 127) ? 1 : 0;
        }
    }
    
    return result;
}
```

#### 7. Add Color Image Support

**Files to modify:**
- `src/image_utils.cpp/hpp` - Add RGB support
- All scheme files - Process each channel

**Approach:**
1. Load color image (PPM format)
2. Split into R, G, B channels
3. Process each channel separately
4. Combine results

**Example:**
```cpp
struct ColorImage {
    int width;
    int height;
    std::vector<std::vector<int>> red;
    std::vector<std::vector<int>> green;
    std::vector<std::vector<int>> blue;
};

ColorImage loadPPM(const std::string &filename);
void savePPM(const std::string &filename, const ColorImage &img);

void generateColorShares(const ColorImage &secret,
                        ColorImage &share1,
                        ColorImage &share2) {
    // Process each channel
    Image r1, r2, g1, g2, b1, b2;
    
    generateShares(secret.red, r1, r2);
    generateShares(secret.green, g1, g2);
    generateShares(secret.blue, b1, b2);
    
    // Combine
    share1.red = r1;
    share1.green = g1;
    share1.blue = b1;
    // ... etc
}
```

#### 8. Implement Cheating Prevention

**Description:** Prevent malicious shareholders from cheating

**Files to create:**
- `src/vcs_cheating_prevention.cpp/hpp`

**Algorithm:**
- Add authentication patterns
- Verification shares
- Self-checking mechanism

**Reference:** "Cheating prevention in visual cryptography" (Yang et al., 2004)

---

### 🔴 Advanced Extensions (Advanced)

#### 9. Implement General (k, n) Threshold

**Description:** n shares, any k can decrypt

**Difficulty:** Advanced
**Time:** 2-4 weeks

**Algorithm:**
- Basis matrix construction
- Exponential number of patterns
- Optimization required

**Example structure:**
```cpp
class ThresholdVCS {
private:
    int k;  // Threshold
    int n;  // Total shares
    
    // Basis matrices for k-out-of-n
    std::vector<std::vector<std::vector<int>>> basisMatrices;
    
    void constructBasisMatrices();
    
public:
    ThresholdVCS(int threshold, int totalShares);
    
    void generateShares(const Image &secret,
                       std::vector<Image> &shares);
    
    Image decryptShares(const std::vector<Image> &shares);
};
```

#### 10. Video Cryptography

**Description:** Extend to video sequences

**Files to create:**
- `src/video_crypto.cpp/hpp`
- Video frame extraction
- Temporal consistency

**Approach:**
1. Extract frames from video
2. Process each frame
3. Reconstruct video from decrypted frames

**Tools needed:**
- FFmpeg for video I/O
- Frame synchronization

#### 11. GPU Acceleration

**Description:** Use GPU for faster processing

**Technology:** CUDA or OpenCL

**Files to create:**
- `src/gpu/vcs_kernel.cu`
- `src/gpu/rg_kernel.cu`

**Example (CUDA):**
```cuda
__global__ void vcsShareGeneration(
    unsigned char *secret,
    unsigned char *share1,
    unsigned char *share2,
    int width,
    int height,
    unsigned int *randStates
) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= width * height) return;
    
    // Random bit generation
    unsigned int r = curand(&randStates[idx]) % 2;
    
    // VCS logic
    if (secret[idx] == 0) {  // White
        share1[2*idx] = r;
        share1[2*idx+1] = 1-r;
        share2[2*idx] = r;
        share2[2*idx+1] = 1-r;
    } else {  // Black
        share1[2*idx] = r;
        share1[2*idx+1] = 1-r;
        share2[2*idx] = 1-r;
        share2[2*idx+1] = r;
    }
}
```

**Expected speedup:** 10-100x for large images

#### 12. Progressive Scheme

**Description:** Gradually improve image quality with more shares

**Example:** 2 shares → low quality, 3 shares → medium, 4 shares → high

**Algorithm:**
- Multi-level encoding
- Hierarchical reconstruction
- Quality tiers

---

## 🎨 Code Style Guidelines

### Naming Conventions

```cpp
// Functions: camelCase
void generateShares();
Image decryptShares();

// Variables: camelCase
int pixelValue;
Image secretImage;

// Constants: UPPER_SNAKE_CASE
const int MAX_IMAGE_SIZE = 4096;
const double PI = 3.14159;

// Classes/Structs: PascalCase
struct Image { ... };
class VisualCrypto { ... };
```

### File Organization

```cpp
// Header file (.hpp)
#ifndef MODULENAME_HPP
#define MODULENAME_HPP

#include <vector>
#include <string>

// Data structures
struct MyStruct {
    // ...
};

// Function declarations
void myFunction();

#endif

// Implementation file (.cpp)
#include "modulename.hpp"
#include <iostream>

// Function implementations
void myFunction() {
    // ...
}
```

### Comments

```cpp
// Single-line comments for brief explanations
int threshold = 128;  // Binary threshold

/* Multi-line comments for
   longer explanations that
   span multiple lines */

/**
 * Function documentation
 * @param input: Description of input
 * @return: Description of return value
 */
Image processImage(const Image &input);
```

---

## 🧪 Testing Your Changes

### 1. Compile and Test

```bash
# Clean build
make clean
make

# Run main program
./bin/vc_program

# Run analysis
./bin/analyze
```

### 2. Verify Outputs

**Check that:**
- [ ] All shares are generated
- [ ] Decryption works correctly
- [ ] Entropy values are correct
- [ ] No compilation warnings
- [ ] Cross-platform compatibility

### 3. Visual Inspection

```bash
# View outputs
display output/vcs/vcs_share1.pgm
display output/vcs/vcs_decrypted.pgm
```

### 4. Automated Testing

**Create test script:**
```bash
#!/bin/bash
# test.sh

echo "Testing VCS scheme..."
./bin/vc_program

# Check if outputs exist
if [ ! -f "output/vcs/vcs_share1.pgm" ]; then
    echo "ERROR: VCS share1 not generated"
    exit 1
fi

echo "All tests passed!"
```

---

## 📝 Documentation Guidelines

### When Adding Features

1. **Update relevant documentation:**
   - [ ] README.md - Add to features list
   - [ ] docs/TECHNICAL_GUIDE.md - Explain algorithm
   - [ ] docs/QUICK_REFERENCE.md - Add commands
   - [ ] FAQ.md - Add related Q&A
   - [ ] USAGE_EXAMPLES.md - Add examples

2. **Add code comments:**
   - [ ] Function documentation
   - [ ] Complex algorithm explanations
   - [ ] Parameter descriptions
   - [ ] Return value documentation

3. **Update build system:**
   - [ ] Makefile
   - [ ] build.bat
   - [ ] build.sh

---

## 🐛 Debugging Tips

### Common Issues

**Issue 1: Segmentation Fault**
```cpp
// Check array bounds
if (r < 0 || r >= height || c < 0 || c >= width) {
    std::cerr << "Error: Index out of bounds" << std::endl;
    return;
}
```

**Issue 2: Wrong Entropy Values**
```cpp
// Verify probability calculation
double p0 = (double)count0 / (width * height);
double p1 = (double)count1 / (width * height);

// Check for division by zero
if (p0 > 0) entropy += -p0 * log2(p0);
if (p1 > 0) entropy += -p1 * log2(p1);
```

**Issue 3: Incorrect Decryption**
```cpp
// Verify OR operation
result.pixels[r][c] = share1.pixels[r][c] | share2.pixels[r][c];

// Verify XOR operation (DHCOD)
result.pixels[r][c] = share1.pixels[r][c] ^ share2.pixels[r][c];
```

### Debugging Tools

**GDB (Linux/macOS):**
```bash
g++ -g -std=c++11 -o bin/vc_program src/*.cpp
gdb bin/vc_program
(gdb) run
(gdb) backtrace
```

**Visual Studio Debugger (Windows):**
- Set breakpoints
- Step through code
- Inspect variables

**Print Debugging:**
```cpp
std::cout << "Debug: r=" << r << " c=" << c << " pixel=" << pixel << std::endl;
```

---

## 🚀 Submitting Contributions

### For Academic Projects

1. **Document your extension:**
   - Write clear explanation
   - Include mathematical background
   - Add usage examples
   - Provide test results

2. **Create presentation:**
   - Use PRESENTATION.md as template
   - Add slides for your extension
   - Prepare demo

3. **Update project summary:**
   - Add to PROJECT_SUMMARY.md
   - Update metrics
   - Document achievements

### For Open Source (if applicable)

1. **Fork the repository**
2. **Create feature branch**
3. **Make changes with tests**
4. **Update documentation**
5. **Submit pull request**

---

## 📊 Performance Optimization

### Profiling

```bash
# Compile with profiling
g++ -std=c++11 -pg -o bin/vc_program src/*.cpp

# Run and generate profile
./bin/vc_program
gprof bin/vc_program gmon.out > profile.txt
```

### Optimization Flags

```bash
# Level 2 optimization
g++ -std=c++11 -O2 -o bin/vc_program src/*.cpp

# Level 3 optimization (aggressive)
g++ -std=c++11 -O3 -o bin/vc_program src/*.cpp

# With profiling feedback
g++ -std=c++11 -O3 -march=native -o bin/vc_program src/*.cpp
```

### Parallel Processing

```cpp
#include <omp.h>

// In scheme files
#pragma omp parallel for
for (int r = 0; r < h; ++r) {
    for (int c = 0; c < w; ++c) {
        // Process pixel
    }
}

// Compile with:
// g++ -std=c++11 -fopenmp -o bin/vc_program src/*.cpp
```

---

## 📚 Learning Resources

### Books
- "Visual Cryptography and Secret Image Sharing" - Stinson & Liu
- "Introduction to Modern Cryptography" - Katz & Lindell

### Papers
- Naor & Shamir (1995) - Original VCS paper
- Kafri & Keren (1987) - Random Grid
- Recent survey papers on visual cryptography

### Online Resources
- IEEE Xplore - Latest research
- arXiv - Preprints
- GitHub - Other implementations

---

## ✅ Contribution Checklist

Before submitting:

- [ ] Code compiles without errors
- [ ] Code compiles without warnings
- [ ] All tests pass
- [ ] Documentation updated
- [ ] Code follows style guidelines
- [ ] Changes tested on target platforms
- [ ] Examples added if applicable
- [ ] Performance acceptable
- [ ] No memory leaks (valgrind)
- [ ] Cross-platform compatibility verified

---

## 🎯 Good First Issues

**Start with these if you're new:**

1. Add new sample image patterns (Easy)
2. Add command-line argument parsing (Easy)
3. Implement progress indicators (Easy)
4. Add 8×8 Bayer matrix halftoning (Medium)
5. Add Floyd-Steinberg dithering (Medium)

---

## 💡 Ideas Welcome!

Have an idea for extending this project?

1. Check if it's already in this document
2. Research if it's feasible
3. Create a prototype
4. Document your approach
5. Share with others!

---

## 📞 Getting Help

- 📖 Read [FAQ.md](FAQ.md) first
- 🔧 Check [ARCHITECTURE.md](ARCHITECTURE.md) for design
- 📚 Review [docs/TECHNICAL_GUIDE.md](docs/TECHNICAL_GUIDE.md) for algorithms
- 💡 See [USAGE_EXAMPLES.md](USAGE_EXAMPLES.md) for patterns

---

**Happy contributing! Let's make visual cryptography better together! 🚀**
