# QUICK REFERENCE GUIDE

## Visual Cryptography Project - Cheat Sheet

### 🚀 Quick Start (30 seconds)

```bash
# Compile
g++ -std=c++11 -o vc_program main.cpp image_utils.cpp vcs.cpp rg.cpp dhcod.cpp

# Run
./vc_program  # or vc_program.exe on Windows

# Analyze
g++ -std=c++11 -o analyze analyze.cpp image_utils.cpp vcs.cpp rg.cpp dhcod.cpp
./analyze
```

---

## 📁 File Reference

### What Each File Does

| File | What it does | When to modify |
|------|-------------|----------------|
| `main.cpp` | Runs all three schemes | Change input/output filenames |
| `image_utils.*` | Load/save images, halftoning | Add new image formats |
| `vcs.*` | VCS implementation | Modify VCS algorithm |
| `rg.*` | Random Grid implementation | Modify RG algorithm |
| `dhcod.*` | DHCOD implementation | Modify DHCOD or halftoning |
| `analyze.cpp` | Compare schemes | Add new metrics |

---

## 🎯 Common Tasks

### 1. Use Your Own Image

```cpp
// In main.cpp, replace:
std::string inputFilename = "input.pgm";  // Your secret image
std::string coverFilename = "cover.pgm";  // Your cover image
```

### 2. Change Image Size

Images must be PGM format. Create with GIMP, ImageMagick, or similar:
```bash
# Convert JPG to PGM using ImageMagick
convert image.jpg -compress none image.pgm

# Resize
convert image.pgm -resize 200x200 resized.pgm
```

### 3. Only Run One Scheme

Comment out unwanted sections in `main.cpp`:
```cpp
// Comment these lines to skip VCS:
// VCS::generateShares(binary, vcs_s1, vcs_s2);
// savePGM("vcs_share1.pgm", vcs_s1);
```

### 4. Modify Halftoning

In `image_utils.cpp`, change Bayer matrix or threshold:
```cpp
// Current: 4x4 Bayer matrix
// Modify: Use 8x8 or custom matrix
```

### 5. Add Debug Output

```cpp
// In any .cpp file:
std::cout << "Debug: width=" << img.width << std::endl;
```

---

## 🔧 Compilation Options

### Debug Build
```bash
g++ -std=c++11 -g -DDEBUG -o vc_program main.cpp image_utils.cpp vcs.cpp rg.cpp dhcod.cpp
```

### Optimized Build
```bash
g++ -std=c++11 -O3 -o vc_program main.cpp image_utils.cpp vcs.cpp rg.cpp dhcod.cpp
```

### With Warnings
```bash
g++ -std=c++11 -Wall -Wextra -o vc_program main.cpp image_utils.cpp vcs.cpp rg.cpp dhcod.cpp
```

---

## 🐛 Troubleshooting

### Problem: "g++ not found"
**Solution**: Install MinGW (Windows) or gcc (Linux/Mac)

### Problem: "Cannot open file input.pgm"
**Solution**: Program creates it automatically. Just run it.

### Problem: "Shares look wrong"
**Solution**: Check internal vs PGM conversion in `savePGM()`

### Problem: "Compilation error in main.cpp"
**Solution**: Make sure `#include <fstream>` is present

### Problem: "Black and white reversed"
**Solution**: This is normal - internal (0=white, 1=black) vs PGM (0=black, 255=white)

---

## 📊 Understanding Output Files

### VCS Files
- `vcs_share1.pgm` - Random noise (400×200)
- `vcs_share2.pgm` - Random noise (400×200)
- `vcs_decrypted.pgm` - Secret revealed (400×200, 50% contrast)

### RG Files
- `rg_share1.pgm` - Random noise (200×200)
- `rg_share2.pgm` - Random noise (200×200)
- `rg_decrypted.pgm` - Secret revealed (200×200, 50% contrast)

### DHCOD Files
- `dhcod_share1_meaningful.pgm` - Looks like cover (200×200)
- `dhcod_share2_meaningful.pgm` - Looks like cover (200×200)
- `dhcod_decrypted.pgm` - Secret revealed (200×200, perfect with XOR)

---

## 🎨 Viewing PGM Files

### Windows
- IrfanView: https://www.irfanview.com/
- XnView: https://www.xnview.com/
- GIMP: https://www.gimp.org/

### Linux
- GIMP
- ImageMagick: `display image.pgm`
- Eye of GNOME: `eog image.pgm`

### macOS
- Preview (built-in)
- GIMP
- ImageMagick: `open image.pgm`

### Online
- Upload to: https://www.photopea.com/

---

## 📐 Algorithm Reference

### VCS Share Generation
```
For each secret pixel:
  Random coin flip
  If white: Both shares same pattern [1,0] or [0,1]
  If black: Shares complementary [1,0] and [0,1]
```

### RG Share Generation
```
For each secret pixel:
  Share1 = random(0 or 1)
  If white: Share2 = Share1
  If black: Share2 = NOT Share1
```

### DHCOD Share Generation
```
Share1 = Halftone(Cover)
For each pixel:
  If secret[i] white: Share2[i] = Share1[i]
  If secret[i] black: Share2[i] = NOT Share1[i]
```

### Decryption
- **VCS/RG**: Visual OR (stack transparencies)
- **DHCOD**: XOR (digital) or OR (visual)

---

## 📈 Performance Tips

### For Large Images
1. Process in chunks (not implemented, but possible)
2. Use -O3 optimization flag
3. Consider multi-threading (future work)

### For Better Quality
1. Use error diffusion instead of ordered dithering
2. Post-process with contrast enhancement
3. Use higher resolution input

### For Better Security
1. Use cryptographically secure random (replace `rand()`)
2. Add authentication codes
3. Implement verifiable shares

---

## 🔐 Security Notes

### Perfect Security (VCS, RG)
- Individual shares reveal ZERO information
- Cannot be broken even with infinite computing power
- Based on information theory, not computational hardness

### Good Security (DHCOD)
- Shares look meaningful (trade-off)
- Statistical analysis might reveal patterns
- Still very secure for practical purposes

### Random Number Quality
Current: `rand()` - sufficient for demonstration
Production: Use `std::random_device` and `std::mt19937`

---

## 📝 Code Snippets

### Read a PGM Image
```cpp
#include "image_utils.hpp"
Image img = loadPGM("myimage.pgm");
if (img.width == 0) {
    std::cerr << "Failed to load image" << std::endl;
}
```

### Save a PGM Image
```cpp
Image img(100, 100);  // Create 100x100 image
// ... fill with data ...
savePGM("output.pgm", img);
```

### Binarize an Image
```cpp
Image gray = loadPGM("grayscale.pgm");
Image binary = binarizeImage(gray, 128);  // threshold=128
```

### Halftone an Image
```cpp
Image gray = loadPGM("photo.pgm");
Image halftoned = halftoneImage(gray);
```

### Generate VCS Shares
```cpp
Image secret = loadPGM("secret.pgm");
Image binary = binarizeImage(secret);
Image share1, share2;
VCS::generateShares(binary, share1, share2);
```

### Decrypt VCS Shares
```cpp
Image decrypted = VCS::decryptShares(share1, share2);
savePGM("decrypted.pgm", decrypted);
```

---

## 🎯 Testing Checklist

Before submitting/presenting:

- [ ] Code compiles without errors
- [ ] Code compiles without warnings
- [ ] All three schemes execute successfully
- [ ] Output files are generated
- [ ] Output files can be opened in image viewer
- [ ] VCS shares are 2x width
- [ ] RG shares are same size as input
- [ ] DHCOD shares look like cover
- [ ] Decrypted images show the secret
- [ ] Analysis tool runs and shows metrics
- [ ] README is clear and complete
- [ ] Code is commented

---

## 🏆 Grading Rubric Self-Check

| Criterion | Weight | Self-Score | Notes |
|-----------|--------|------------|-------|
| Implementation | 40% | ___/40 | All schemes working? |
| Code Quality | 20% | ___/20 | Clean, modular, commented? |
| Documentation | 20% | ___/20 | README, guides complete? |
| Testing | 10% | ___/10 | All schemes tested? |
| Innovation | 10% | ___/10 | Extra features added? |
| **Total** | **100%** | **___/100** | |

---

## 🌟 Presentation Tips

### What to Show
1. **Live compilation** (30 sec)
2. **Run program** (30 sec)
3. **Show outputs in image viewer** (1 min)
4. **Explain one algorithm** (2 min)
5. **Show analysis results** (1 min)
6. **Discuss trade-offs** (1 min)

### What to Emphasize
- ✅ All three schemes implemented
- ✅ Perfect security in VCS/RG
- ✅ No pixel expansion in RG/DHCOD
- ✅ Meaningful shares in DHCOD
- ✅ Comprehensive documentation

### Common Questions
**Q**: Why use C++?  
**A**: Fast, low-level control, no dependencies

**Q**: Why PGM format?  
**A**: Simple, text-based, easy to debug

**Q**: Why three schemes?  
**A**: Compare trade-offs: security vs size vs appearance

**Q**: What's the main contribution?  
**A**: Complete implementation with comparative analysis

---

## 📚 Further Reading

### Papers
1. Naor & Shamir (1995) - Original VCS paper
2. Kafri & Keren (1987) - Random Grid paper
3. Zhou et al. (2006) - Halftone VC paper

### Books
1. "Visual Cryptography and Secret Image Sharing" - Cimato & Yang
2. "Digital Halftoning" - Ulichney

### Online
- Wikipedia: Visual Cryptography
- Google Scholar: Search "visual cryptography"
- GitHub: Search "visual cryptography" for similar projects

---

## 💡 Extension Ideas

### Easy (1-2 hours)
- Add different binarization thresholds
- Support different output formats
- Add color to shares (not cryptographic, just visual)

### Medium (1-2 days)
- Implement (2,3) threshold scheme
- Add GUI with file dialogs
- Support JPEG/PNG input (with conversion)

### Advanced (1-2 weeks)
- Color image support (separate RGB channels)
- (k,n) general threshold scheme
- Error diffusion halftoning
- Cheating prevention mechanisms

---

## 🎓 Learning Path

### Beginner
1. Read README.md
2. Compile and run
3. View output images
4. Understand basic VCS algorithm

### Intermediate
1. Read TECHNICAL_GUIDE.md
2. Study code in vcs.cpp, rg.cpp
3. Modify parameters
4. Run analysis tool

### Advanced
1. Read original research papers
2. Understand security proofs
3. Implement extensions
4. Optimize algorithms

---

## 🚧 Known Limitations

### Current Implementation
1. Binary images only (no grayscale secrets in VCS/RG)
2. No cheating prevention
3. Basic random number generator (`rand()`)
4. No compression
5. ASCII PGM only for output (P2)

### By Design
1. 50% contrast loss in VCS/RG (theoretical limit)
2. 2x expansion in VCS (can't avoid without changing scheme)
3. DHCOD less secure than VCS/RG (trade-off for meaningfulness)

---

## ⚡ Power User Tips

### Batch Processing
```bash
# Process multiple images (create script)
for img in *.pgm; do
  ./vc_program "$img"
done
```

### Custom Covers for DHCOD
Use different textures, photos, or patterns as covers for variety.

### Measure Execution Time
```bash
# Windows
Measure-Command { .\vc_program.exe }

# Linux/Mac
time ./vc_program
```

### Compare with Reference Implementation
If you have another VC implementation, compare outputs.

---

## 📞 Getting Help

1. **Check README.md** first
2. **Read error messages** carefully
3. **Verify file paths** are correct
4. **Check image format** (must be PGM)
5. **Recompile** after any changes
6. **Start fresh** if confused (delete .exe and .o files)

---

**Quick Reference Version**: 1.0  
**Last Updated**: December 14, 2025  
**Status**: Complete and Ready

---

*Keep this file handy during development, testing, and presentation!*
