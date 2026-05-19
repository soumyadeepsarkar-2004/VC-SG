---
layout: default
title: Algorithms
nav_order: 2
---

# Algorithm Details

## 1. VCS — Naor-Shamir (2,2) Threshold Scheme

**Paper:** Naor & Shamir, "Visual Cryptography", EUROCRYPT 1994

### How It Works

Each secret pixel is encoded into **2 sub-pixels** per share (2× horizontal expansion):

```
White pixel → Both shares get SAME random pattern
  Coin=0: Share1=[■□], Share2=[■□]  → OR=[■□] (50% black)
  Coin=1: Share1=[□■], Share2=[□■]  → OR=[□■] (50% black)

Black pixel → Shares get COMPLEMENTARY patterns
  Coin=0: Share1=[■□], Share2=[□■]  → OR=[■■] (100% black)
  Coin=1: Share1=[□■], Share2=[■□]  → OR=[■■] (100% black)
```

### Properties
- **Security:** Perfect (information-theoretic). Each share is uniformly random.
- **Expansion:** 2× horizontal. A 200×200 secret produces 400×200 shares.
- **Contrast:** 50% loss. White regions appear gray (50% black sub-pixels).
- **Decryption:** OR operation (physically stack transparencies).

### Implementation
```cpp
// src/vcs.cpp — Core logic
int coin = randomBit();  // Secure PRNG (mt19937)
if (pixel == 0) {        // White → identical sub-pixels
    s1 = s2 = (coin == 0) ? {1,0} : {0,1};
} else {                 // Black → complementary sub-pixels
    s1 = (coin == 0) ? {1,0} : {0,1};
    s2 = (coin == 0) ? {0,1} : {1,0};
}
```

---

## 2. Random Grid — Kafri-Keren Scheme

**Paper:** Kafri & Keren, "Encryption of pictures and shapes by random grids", Optics Letters 1987

### How It Works

No pixel expansion — each pixel maps 1:1:

```
For each pixel (r,c):
  Share1[r,c] = random(0 or 1)

  If secret[r,c] == White:
    Share2[r,c] = Share1[r,c]      → OR = Share1 (50% chance black)

  If secret[r,c] == Black:
    Share2[r,c] = NOT Share1[r,c]  → OR = always 1 (black)
```

### Properties
- **Security:** Perfect (same as VCS). Share1 is uniformly random; Share2 is also uniformly random.
- **Expansion:** None. Shares are same size as secret.
- **Contrast:** 50% loss (same as VCS, inherent to OR-based decryption).
- **Decryption:** OR operation.

### Why It's Better Than VCS
Same security, half the storage. The only tradeoff is identical — 50% contrast loss.

---

## 3. DHCOD — Digital Halftoning Cryptography with Ordered Dithering

**Paper:** Chang et al., "Halftone visual cryptography with complementary covers", 2007

### How It Works

Produces **meaningful shares** that look like a cover image (steganographic):

```
1. Halftone the cover image using 4×4 Bayer ordered dithering → Share1
2. Halftone the secret image → SecretBin

3. For each pixel (r,c):
   If SecretBin[r,c] == White:
     Share2[r,c] = Share1[r,c]       → XOR = 0 (White)
   If SecretBin[r,c] == Black:
     Share2[r,c] = NOT Share1[r,c]   → XOR = 1 (Black)
```

### Bayer Matrix (4×4 Ordered Dithering)

```
 0  8  2 10
12  4 14  6
 3 11  1  9
15  7 13  5
```

Threshold = `bayer[y%4][x%4] × 17`. Pixel > threshold → White, else → Black.

### Properties
- **Security:** Near-perfect (entropy ≈ 0.999 bits/pixel).
- **Expansion:** None.
- **Contrast:** Excellent with XOR decryption (no contrast loss).
- **Decryption:** XOR (digital). OR also works but with contrast loss.
- **Steganography:** Share1 looks like the cover image — not suspicious.

---

## Image Processing Pipeline

### Internal Pixel Convention
- `0 = White` (transparent/paper)
- `1 = Black` (opaque/ink)

### PGM Convention
- `0 = Black`, `255 = White`

The `isBinary` flag on the `Image` struct controls which convention `savePGM()` uses.

### Binarization
```
Input pixel < 128 → Black (internal 1)
Input pixel ≥ 128 → White (internal 0)
```

### Random Number Generation
Uses `std::mt19937` seeded by `std::random_device` — cryptographically better than `rand()/srand()`.
