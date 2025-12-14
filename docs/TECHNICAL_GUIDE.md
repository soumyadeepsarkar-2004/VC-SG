# Technical Implementation Guide

## Detailed Algorithm Walkthrough

### 1. Visual Cryptography Scheme (VCS) - Naor-Shamir Implementation

#### Mathematical Foundation
For a (2,2) threshold scheme:
- 2 shares are generated
- Any 2 shares can decrypt the secret
- Individual shares reveal no information

#### Share Generation Process

**Input**: Binary secret image S (m × n pixels)  
**Output**: Share1 and Share2 (m × 2n pixels each)

**Algorithm**:
```
For each pixel S[i,j]:
  Generate random bit R ∈ {0, 1}
  
  If S[i,j] = 0 (White pixel):
    If R = 0:
      Share1[i, 2j:2j+1] = [1, 0]  // Black, White
      Share2[i, 2j:2j+1] = [1, 0]  // Same pattern
    Else:
      Share1[i, 2j:2j+1] = [0, 1]  // White, Black
      Share2[i, 2j:2j+1] = [0, 1]  // Same pattern
      
  If S[i,j] = 1 (Black pixel):
    If R = 0:
      Share1[i, 2j:2j+1] = [1, 0]  // Black, White
      Share2[i, 2j:2j+1] = [0, 1]  // Complementary
    Else:
      Share1[i, 2j:2j+1] = [0, 1]  // White, Black
      Share2[i, 2j:2j+1] = [1, 0]  // Complementary
```

**Decryption** (Visual OR):
```
For each pixel position [i,j]:
  Result[i,j] = Share1[i,j] OR Share2[i,j]
  
  White pixel: [1,0] OR [1,0] = [1,0] → 50% black
  Black pixel: [1,0] OR [0,1] = [1,1] → 100% black
```

**Contrast Analysis**:
- White pixels: 50% of sub-pixels are black
- Black pixels: 100% of sub-pixels are black
- Relative contrast: (100% - 50%) / 100% = 50%

#### Code Implementation Details

```cpp
// Share generation uses rand() % 2 for randomization
int coin = rand() % 2;

// White pixel case - identical shares
if (pixel == 0) {
    if (coin == 0) {
        s1_sub[0] = 1; s1_sub[1] = 0;  // Pattern A
        s2_sub[0] = 1; s2_sub[1] = 0;
    } else {
        s1_sub[0] = 0; s1_sub[1] = 1;  // Pattern B
        s2_sub[0] = 0; s2_sub[1] = 1;
    }
}

// Black pixel case - complementary shares
else {
    if (coin == 0) {
        s1_sub[0] = 1; s1_sub[1] = 0;  // Pattern A
        s2_sub[0] = 0; s2_sub[1] = 1;  // Complement of A
    } else {
        s1_sub[0] = 0; s1_sub[1] = 1;  // Pattern B
        s2_sub[0] = 1; s2_sub[1] = 0;  // Complement of B
    }
}
```

---

### 2. Random Grid (RG) - Kafri-Keren Implementation

#### Mathematical Foundation
Random Grid scheme provides perfect security without pixel expansion.

**Probability Analysis**:
For any secret pixel s:
- P(R1[i,j] = 0) = P(R1[i,j] = 1) = 0.5
- When s = 0: R2 = R1 (deterministic given R1)
- When s = 1: R2 = ¬R1 (deterministic given R1)

Individual shares are uniformly random → Perfect security

#### Share Generation Process

**Input**: Binary secret image S (m × n pixels)  
**Output**: Share1 and Share2 (m × n pixels each)

**Algorithm**:
```
For each pixel S[i,j]:
  R1[i,j] = random(0 or 1)
  
  If S[i,j] = 0 (White):
    R2[i,j] = R1[i,j]        // Same as R1
  Else (S[i,j] = 1, Black):
    R2[i,j] = 1 - R1[i,j]    // Complement of R1
```

**Decryption Truth Table**:
```
Secret | R1 | R2 | R1 OR R2 | Probability
-------|----|----|----------|------------
   0   | 0  | 0  |    0     |    0.5
   0   | 1  | 1  |    1     |    0.5
   1   | 0  | 1  |    1     |    0.5
   1   | 1  | 0  |    1     |    0.5
```

**Expected Values**:
- White pixel: E[OR] = 0.5 (50% black)
- Black pixel: E[OR] = 1.0 (100% black)
- Relative contrast: 50%

#### Code Implementation

```cpp
// Randomize first share
share1.pixels[r][c] = rand() % 2;

// Determine second share based on secret
int secretPixel = secret.pixels[r][c];

if (secretPixel == 0) { // White
    share2.pixels[r][c] = share1.pixels[r][c];
} else { // Black
    share2.pixels[r][c] = 1 - share1.pixels[r][c];
}
```

**Advantages over VCS**:
1. No pixel expansion (m × n → m × n)
2. Simpler algorithm (no sub-pixel mapping)
3. Same security level (perfect)
4. Less memory and storage required

---

### 3. DHCOD - Meaningful Shares Implementation

#### Theoretical Background

DHCOD combines digital halftoning with visual cryptography to create meaningful shares.

**Key Concepts**:
1. **Halftoning**: Convert grayscale to binary while preserving visual appearance
2. **Ordered Dithering**: Use threshold matrix (Bayer matrix)
3. **Meaningful Shares**: Shares appear as chosen cover images

#### Bayer Matrix (4×4)

```
M = [ 0   8   2  10 ]
    [ 12  4  14   6 ]
    [ 3  11   1   9 ]
    [ 15  7  13   5 ]
```

**Threshold Calculation**:
```
For pixel at (x, y) with value V:
  threshold = M[y mod 4][x mod 4] × 17
  
  If V > threshold:
    Output = 0 (White)
  Else:
    Output = 1 (Black)
```

The value 17 comes from: 255/15 ≈ 17 (to scale 0-15 range to 0-255)

#### Share Generation Process

**Input**: 
- Secret image S (m × n)
- Cover image C (m × n)

**Output**:
- Share1 and Share2 (m × n each)

**Algorithm**:
```
Step 1: Halftone secret
  S_binary = Halftone(S, BayerMatrix)

Step 2: Halftone cover to create Share1
  Share1 = Halftone(C, BayerMatrix)

Step 3: Generate Share2 based on secret
  For each pixel (i,j):
    If S_binary[i,j] = 0 (White):
      Share2[i,j] = Share1[i,j]
    Else (S_binary[i,j] = 1, Black):
      Share2[i,j] = 1 - Share1[i,j]
```

#### Decryption Methods

**1. Digital XOR (Perfect Recovery)**:
```
For each pixel (i,j):
  If Share1[i,j] XOR Share2[i,j] = 0:
    Result[i,j] = 0 (White)
  Else:
    Result[i,j] = 1 (Black)
```

**2. Visual OR (Lossy)**:
```
For each pixel (i,j):
  Result[i,j] = Share1[i,j] OR Share2[i,j]
```

**Comparison**:
- XOR: Perfect reconstruction, requires computation
- OR: Visual decryption, 50% contrast loss (similar to VCS/RG)

#### Code Implementation

```cpp
// Halftone secret and cover
Image secretBin = halftoneImage(secret);
Image coverBin = halftoneImage(cover);

// Share1 is the halftoned cover
share1 = coverBin;
share2 = Image(w, h);

// Generate Share2 based on secret
for (int r = 0; r < h; ++r) {
    for (int c = 0; c < w; ++c) {
        int s_pix = secretBin.pixels[r][c];
        int c_pix = coverBin.pixels[r][c];
        
        if (s_pix == 0) { // Secret is White
            share2.pixels[r][c] = c_pix;
        } else { // Secret is Black
            share2.pixels[r][c] = 1 - c_pix;
        }
    }
}
```

---

## Image Processing Functions

### 1. PGM Loading

**Supported Formats**:
- P2: ASCII format
- P5: Binary format

**Algorithm**:
```
1. Read magic number (P2 or P5)
2. Skip comments (lines starting with #)
3. Read width, height, maxval
4. Read pixel data:
   - P2: Read as integers
   - P5: Read as bytes
```

**Error Handling**:
- Invalid format → Return empty image
- File not found → Return empty image
- Corrupted data → Partial read

### 2. PGM Saving

**Output Format**: P2 (ASCII) for readability

**Conversion Logic**:
```
Internal value | PGM value | Meaning
---------------|-----------|--------
      0        |    255    | White
      1        |     0     | Black
   > 1 (raw)   |  as-is    | Grayscale
```

### 3. Binarization

**Threshold Method**:
```
For each pixel P:
  If P < threshold:
    Output = 1 (Black)
  Else:
    Output = 0 (White)
```

**Default Threshold**: 128 (middle of 0-255 range)

### 4. Halftoning (Ordered Dithering)

**Process**:
```
For pixel at (x, y) with value V:
  i = y mod 4
  j = x mod 4
  threshold = Bayer[i][j] × 17
  
  If V > threshold:
    Output = 0 (White)
  Else:
    Output = 1 (Black)
```

**Advantages**:
- Fast (no error propagation)
- Consistent (deterministic)
- Good visual quality for covers

**Bayer Matrix Properties**:
- 4×4 dispersed-dot ordered dither
- Values 0-15 evenly distributed
- Blue noise characteristics

---

## Performance Analysis

### Time Complexity

**VCS**:
- Share Generation: O(m × n) 
  - For each pixel: constant time operations
- Decryption: O(m × 2n)
  - OR operation per output pixel

**RG**:
- Share Generation: O(m × n)
  - For each pixel: one random, one comparison
- Decryption: O(m × n)
  - OR operation per pixel

**DHCOD**:
- Halftoning: O(m × n)
  - Bayer lookup and comparison per pixel
- Share Generation: O(m × n)
  - XOR/complement per pixel
- Decryption: O(m × n)
  - XOR per pixel

### Space Complexity

**VCS**:
- Input: m × n
- Shares: 2 × (m × 2n) = 4mn
- Output: m × 2n
- Total: 7mn

**RG**:
- Input: m × n
- Shares: 2 × (m × n) = 2mn
- Output: m × n
- Total: 4mn

**DHCOD**:
- Input: m × n (secret)
- Cover: m × n
- Shares: 2 × (m × n) = 2mn
- Output: m × n
- Total: 5mn

### Memory Optimization Tips

1. **In-place Operations**: Reuse buffers when possible
2. **Stream Processing**: Process row-by-row for large images
3. **Share Distribution**: Don't store both shares locally

---

## Security Considerations

### Perfect Security (Information-Theoretic)

**VCS and RG** achieve perfect security:

**Proof Sketch for RG**:
```
Given Share1 alone:
  P(S[i,j] = 0 | Share1[i,j] = b) = ?
  
  For Share1[i,j] = 0:
    Could come from: (S=0, Share1=0, Share2=0)
                 or: (S=1, Share1=0, Share2=1)
    Both equally likely → P(S=0|Share1=0) = 0.5
  
  For Share1[i,j] = 1:
    Could come from: (S=0, Share1=1, Share2=1)
                 or: (S=1, Share1=1, Share2=0)
    Both equally likely → P(S=0|Share1=1) = 0.5
  
  Conclusion: Share1 alone reveals nothing
  Same proof applies to Share2 alone
```

### DHCOD Security

**Not Perfect but Good**:
- Shares look like cover image (not random)
- Statistical analysis might reveal patterns
- Trade-off: Meaningfulness vs. Perfect Security

**Mitigation**:
- Choose appropriate cover images
- Use good quality halftoning
- Consider context of usage

---

## Testing Methodology

### Unit Tests

**Image I/O**:
```
1. Create image with known pattern
2. Save to PGM
3. Load back
4. Verify pixel values match
```

**Binarization**:
```
1. Create gradient image (0-255)
2. Binarize with threshold 128
3. Verify: values < 128 → 1, others → 0
```

**Halftoning**:
```
1. Create solid gray image (value 128)
2. Apply halftoning
3. Verify: approximately 50% black pixels
4. Check distribution pattern
```

### Integration Tests

**VCS**:
```
1. Create simple binary pattern (checkerboard)
2. Generate shares
3. Verify share dimensions (2x width)
4. Decrypt
5. Check contrast reduction
```

**RG**:
```
1. Create test pattern
2. Generate shares
3. Verify no size expansion
4. Decrypt
5. Compare with VCS output
```

**DHCOD**:
```
1. Create secret and cover
2. Generate shares
3. Verify shares resemble cover
4. XOR decrypt
5. Verify perfect recovery
6. OR decrypt
7. Compare with XOR result
```

### Quality Metrics

**1. Contrast Ratio**:
```
CR = (B_black - B_white) / (B_black + B_white)

Where:
  B_black = Average brightness of black regions
  B_white = Average brightness of white regions
```

**2. PSNR (Peak Signal-to-Noise Ratio)**:
```
MSE = (1/(m×n)) × Σ(Original[i,j] - Decrypted[i,j])²

PSNR = 10 × log₁₀(MAX²/MSE)
```

**3. Visual Quality**:
- Subjective assessment
- Pattern recognition
- Edge preservation

---

## Debugging Tips

### Common Issues

**1. Compilation Errors**:
```
Problem: undefined reference to functions
Solution: Ensure all .cpp files are compiled together
Command: g++ -std=c++11 main.cpp image_utils.cpp vcs.cpp rg.cpp dhcod.cpp
```

**2. Wrong Colors in Output**:
```
Problem: Black appears white, vice versa
Cause: Confusion between PGM format (0=black) and internal (1=black)
Solution: Check savePGM conversion logic
```

**3. Decryption Not Working**:
```
Problem: Decrypted image is blank or random
Cause: Wrong operator (AND vs OR) or share order
Solution: Visual decryption uses OR, digital uses XOR
```

**4. Poor Halftone Quality**:
```
Problem: Halftoned image too dark/light
Cause: Incorrect Bayer matrix scaling
Solution: Verify threshold = Bayer[i][j] × 17
```

### Debugging Workflow

1. **Enable Intermediate Saves**: Save images after each step
2. **Print Dimensions**: Verify image sizes match expectations
3. **Sample Pixels**: Print values of specific pixels
4. **Visual Inspection**: Open PGM files in image viewer
5. **Use Simple Patterns**: Test with checkerboard, solid colors

---

## Extensions and Modifications

### 1. Color Images

**Approach**: Process each channel (R, G, B) separately
```cpp
struct ColorImage {
    Image red, green, blue;
};

void generateColorShares(ColorImage& secret, 
                        ColorImage& share1, 
                        ColorImage& share2) {
    VCS::generateShares(secret.red, share1.red, share2.red);
    VCS::generateShares(secret.green, share1.green, share2.green);
    VCS::generateShares(secret.blue, share1.blue, share2.blue);
}
```

### 2. (k, n) Threshold Schemes

**Concept**: k out of n shares needed to decrypt

**Example (2,3)**:
- 3 shares generated
- Any 2 can decrypt
- Implementation: Use polynomial secret sharing

### 3. Error Diffusion Halftoning

**Floyd-Steinberg Dithering**:
```
For pixel (x,y):
  error = original[y][x] - quantized[y][x]
  Distribute error to neighbors:
    (x+1, y  ) gets 7/16 of error
    (x-1, y+1) gets 3/16 of error
    (x  , y+1) gets 5/16 of error
    (x+1, y+1) gets 1/16 of error
```

Better visual quality than ordered dithering.

### 4. Cheating Prevention

**Problem**: One party might produce fake share

**Solution**: Add verification:
```cpp
struct VerifiableShare {
    Image share;
    std::string hash;  // SHA-256 of share
    std::string signature;  // RSA signature
};
```

---

## Conclusion

This implementation provides a complete framework for:
- Understanding visual cryptography fundamentals
- Comparing different schemes
- Experimenting with parameters
- Building upon for advanced features

The code is modular, well-documented, and suitable for both academic study and practical experimentation.
