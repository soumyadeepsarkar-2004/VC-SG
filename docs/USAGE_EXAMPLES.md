# 📘 Usage Examples & Tutorials

## Visual Cryptography Project - Practical Guide

This guide provides practical examples and tutorials for using the Visual Cryptography implementation.

---

## 🎯 Table of Contents

1. [Basic Usage](#basic-usage)
2. [Creating Custom Images](#creating-custom-images)
3. [Analyzing Results](#analyzing-results)
4. [Advanced Modifications](#advanced-modifications)
5. [Use Case Scenarios](#use-case-scenarios)
6. [Tips & Best Practices](#tips--best-practices)

---

## 🚀 Basic Usage

### Example 1: Run All Schemes (Default)

**Windows:**
```cmd
bin\vc_program.exe
```

**Linux/macOS:**
```bash
./bin/vc_program
```

**What happens:**
1. Creates sample secret image (200×200 black square on white)
2. Creates sample cover image (gradient pattern)
3. Generates shares for all three schemes
4. Decrypts and reconstructs the secret
5. Saves all outputs to `output/` directory

**Output Files:**
- `output/vcs/vcs_share1.pgm` & `vcs_share2.pgm` (400×200 each)
- `output/rg/rg_share1.pgm` & `rg_share2.pgm` (200×200 each)
- `output/dhcod/dhcod_share1_meaningful.pgm` & `dhcod_share2_meaningful.pgm` (200×200 each)
- All decrypted images in respective folders

---

### Example 2: Analyze Generated Shares

**Windows:**
```cmd
bin\analyze.exe
```

**Linux/macOS:**
```bash
./bin/analyze
```

**Output:**
```
==========================================
VISUAL CRYPTOGRAPHY COMPARATIVE ANALYSIS
==========================================

VCS (Visual Cryptography Scheme)
  Share 1 Entropy: 1.000000 bits/pixel
  Share 2 Entropy: 1.000000 bits/pixel
  Average Entropy: 1.000000 bits/pixel
  
Random Grid
  Share 1 Entropy: 1.000000 bits/pixel
  Share 2 Entropy: 1.000000 bits/pixel
  Average Entropy: 1.000000 bits/pixel
  
DHCOD (Meaningful Shares)
  Share 1 Entropy: 0.992341 bits/pixel
  Share 2 Entropy: 0.991876 bits/pixel
  Average Entropy: 0.992109 bits/pixel

... (more metrics) ...
```

---

## 🎨 Creating Custom Images

### Example 3: Use Your Own Secret Image

1. **Prepare your image:**
   - Convert to grayscale PGM format
   - Recommended size: 200×200 or smaller (for testing)
   - Binary content works best

2. **Save as PGM:**
   - Use GIMP, ImageMagick, or Python (see below)

3. **Replace input:**
   ```bash
   # Backup original
   cp input/input.pgm input/input_backup.pgm
   
   # Copy your image
   cp your_image.pgm input/input.pgm
   ```

4. **Run the program:**
   ```bash
   ./bin/vc_program
   ```

**Using ImageMagick to convert:**
```bash
# Install ImageMagick first
convert your_image.png -resize 200x200 -colorspace Gray input/input.pgm
```

**Using Python to create PGM:**
```python
from PIL import Image
import numpy as np

# Load your image
img = Image.open('your_image.jpg').convert('L')
img = img.resize((200, 200))

# Convert to binary (threshold)
threshold = 128
binary = img.point(lambda p: 255 if p > threshold else 0)

# Save as PGM
binary.save('input/input.pgm')
```

---

### Example 4: Create Text-Based Secret

**Python script to create text image:**

```python
from PIL import Image, ImageDraw, ImageFont

# Create white canvas
img = Image.new('L', (200, 200), 255)
draw = ImageDraw.Draw(img)

# Draw text
font = ImageFont.load_default()
text = "SECRET"
draw.text((50, 90), text, fill=0, font=font)

# Save as PGM
img.save('input/input.pgm')
print("Created text secret image!")
```

---

### Example 5: Custom Cover Image for DHCOD

Create a meaningful cover image that will appear in DHCOD shares:

**Using Python:**
```python
from PIL import Image
import numpy as np

# Create gradient or pattern
width, height = 200, 200
cover = np.zeros((height, width), dtype=np.uint8)

# Circular gradient
for r in range(height):
    for c in range(width):
        dist = np.sqrt((r - 100)**2 + (c - 100)**2)
        cover[r, c] = int(255 * (1 - min(dist / 141, 1)))

# Save
Image.fromarray(cover).save('input/cover.pgm')
```

**Using ImageMagick:**
```bash
# Create gradient
convert -size 200x200 gradient: -colorspace Gray input/cover.pgm

# Or use existing image
convert photo.jpg -resize 200x200 -colorspace Gray input/cover.pgm
```

---

## 🔬 Analyzing Results

### Example 6: Detailed Analysis with Custom Metrics

Modify `src/analyze.cpp` to add custom analysis:

**View specific entropy values:**
```bash
./bin/analyze | grep Entropy
```

**Save analysis to file:**
```bash
./bin/analyze > analysis_results.txt
```

**Compare schemes:**
```bash
./bin/analyze | grep "Average Entropy"
```

---

### Example 7: Visual Inspection

**View PGM files:**

**Windows:**
- Use IrfanView, XnView, or GIMP

**Linux:**
```bash
display output/vcs/vcs_share1.pgm          # ImageMagick
eog output/vcs/vcs_share1.pgm              # Eye of GNOME
gimp output/vcs/vcs_share1.pgm             # GIMP
```

**macOS:**
```bash
open -a Preview output/vcs/vcs_share1.pgm
```

**Convert PGM to PNG for easier viewing:**
```bash
convert output/vcs/vcs_share1.pgm output/vcs/vcs_share1.png
```

---

## ⚙️ Advanced Modifications

### Example 8: Modify Secret Image Size

Edit `src/main.cpp`:

```cpp
// Line ~60, in main()
// Original: 200x200
createSampleImage("input/input.pgm", 200, 200);

// Change to 400x400
createSampleImage("input/input.pgm", 400, 400);
```

Rebuild:
```bash
make clean && make
```

**Note:** Larger images take more processing time and memory.

---

### Example 9: Modify Sample Pattern

Edit `src/main.cpp` in `createSampleImage()` function:

**Original (centered square):**
```cpp
if (r > h/4 && r < 3*h/4 && c > w/4 && c < 3*w/4) {
    img.pixels[r][c] = 1;  // Black
}
```

**Create circle:**
```cpp
int centerR = h / 2;
int centerC = w / 2;
int radius = h / 4;
int dr = r - centerR;
int dc = c - centerC;
if (dr*dr + dc*dc < radius*radius) {
    img.pixels[r][c] = 1;  // Black circle
}
```

**Create diagonal stripes:**
```cpp
if ((r + c) % 20 < 10) {
    img.pixels[r][c] = 1;  // Black stripes
}
```

---

### Example 10: Modify DHCOD Halftoning

Edit `src/image_utils.cpp` - change Bayer matrix size:

```cpp
// Current: 4x4 Bayer matrix
// Modify to 8x8 for finer dithering (more detail)

// Find bayerMatrix definition and expand it
```

Or adjust threshold scaling for different halftone appearance.

---

## 💼 Use Case Scenarios

### Scenario 1: Secure Document Sharing

**Problem:** Share confidential document between two parties.

**Solution using VCS:**
```bash
# 1. Convert document to image (200x200)
convert document.pdf[0] -resize 200x200 -colorspace Gray input/input.pgm

# 2. Run VCS
./bin/vc_program

# 3. Send shares via different channels
# Email share1 to party, SMS share2 to party

# 4. Party prints both, overlays them → sees document
```

---

### Scenario 2: Watermark Embedding (DHCOD)

**Problem:** Embed secret watermark in meaningful image.

**Solution:**
```bash
# 1. Prepare watermark (logo/text)
python create_watermark.py  # Creates input/input.pgm

# 2. Prepare cover image (product photo)
convert product.jpg -resize 200x200 -colorspace Gray input/cover.pgm

# 3. Generate DHCOD shares
./bin/vc_program

# 4. Share 1 looks like product photo
# Distribute share1 publicly
# Keep share2 secure

# 5. XOR both to reveal watermark
```

---

### Scenario 3: Authentication System

**Problem:** Two-factor authentication using physical shares.

**Solution using Random Grid:**
```bash
# 1. Create unique QR code per user
python generate_qr.py user_id  # Creates input/input.pgm

# 2. Generate shares
./bin/vc_program

# 3. Print share1 on plastic card (give to user)
# Store share2 in secure database

# 4. For authentication:
#    - User presents card (share1)
#    - System retrieves share2
#    - Overlay reveals QR code
#    - Scan QR for validation
```

---

### Scenario 4: Batch Processing Multiple Secrets

**Bash script for batch processing:**

```bash
#!/bin/bash
# process_multiple.sh

for secret in secrets/*.pgm; do
    # Copy secret to input
    cp "$secret" input/input.pgm
    
    # Run encryption
    ./bin/vc_program
    
    # Rename outputs with timestamp
    timestamp=$(date +%Y%m%d_%H%M%S)
    mkdir -p archive/$timestamp
    mv output/* archive/$timestamp/
    
    echo "Processed: $secret"
done
```

---

## 💡 Tips & Best Practices

### Performance Tips

1. **Image Size:**
   - Keep images ≤ 512×512 for fast processing
   - Larger images exponentially increase processing time

2. **Binary Images:**
   - Pre-binarize images for better results
   - Avoid grayscale gradients in secret images

3. **Compilation Optimization:**
   ```bash
   # Add optimization flags
   g++ -std=c++11 -O3 -o bin/vc_program src/*.cpp
   ```

### Quality Tips

1. **Input Preparation:**
   - Use high-contrast images
   - Ensure proper thresholding
   - Check PGM format (P2 or P5 both work)

2. **DHCOD Cover Selection:**
   - Choose covers with varied intensity
   - Avoid solid colors
   - Natural images work best

3. **Viewing Results:**
   - View at 100% zoom (no interpolation)
   - Use proper image viewers
   - Print on high-quality printer for physical overlay

### Debugging Tips

1. **Check intermediate outputs:**
   ```cpp
   // In main.cpp, after each step:
   std::cout << "Step completed: " << stepName << std::endl;
   ```

2. **Validate PGM format:**
   ```bash
   file output/vcs/vcs_share1.pgm
   # Should show: Netpbm PGM "rawbits" image data
   ```

3. **Compare file sizes:**
   ```bash
   ls -lh output/vcs/
   # VCS shares should be ~2x size of RG shares
   ```

---

## 📊 Comparison Workflow

**Complete evaluation workflow:**

```bash
# 1. Build project
make clean && make

# 2. Generate outputs with default image
./bin/vc_program

# 3. Run analysis
./bin/analyze > results_default.txt

# 4. Test with custom image
cp my_image.pgm input/input.pgm
./bin/vc_program

# 5. Analyze custom results
./bin/analyze > results_custom.txt

# 6. Compare
diff results_default.txt results_custom.txt
```

---

## 🎓 Learning Path

**Beginner:**
1. Run default program
2. View all outputs
3. Understand basic workflow

**Intermediate:**
1. Create custom input images
2. Modify sample patterns
3. Run comparative analysis

**Advanced:**
1. Modify algorithm parameters
2. Implement additional metrics
3. Extend to (k,n) threshold schemes

---

## 📞 Getting Help

- 📖 Quick commands: [QUICK_REFERENCE.md](docs/QUICK_REFERENCE.md)
- 🔧 Algorithm details: [TECHNICAL_GUIDE.md](docs/TECHNICAL_GUIDE.md)
- 📚 Full guide: [docs/README.md](docs/README.md)

---

**✅ Now you're ready to use Visual Cryptography like a pro!**
