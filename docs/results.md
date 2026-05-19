---
layout: default
title: Results
nav_order: 3
---

# Experimental Results & Analysis

All results obtained from 200×200 pixel test images using the built-in analysis tool.

## Security Analysis — Share Entropy

Higher entropy = more random = more secure. Maximum for binary images is **1.0 bits/pixel**.

| Scheme | Share 1 | Share 2 | Security Level |
|--------|---------|---------|----------------|
| **VCS** | 1.0000 | 1.0000 | Perfect (information-theoretic) |
| **RG** | 1.0000 | 1.0000 | Perfect (information-theoretic) |
| **DHCOD** | 0.9999 | 0.9942 | Near-perfect (computational) |

**Key insight:** VCS and RG shares are indistinguishable from random noise — even quantum computers cannot extract the secret from a single share.

## Reconstruction Quality

| Metric | VCS | Random Grid | DHCOD |
|--------|-----|-------------|-------|
| **Contrast** | 33.3% | 33.5% | 88.3% |
| **PSNR** | 1.22 dB | 4.25 dB | 13.29 dB |
| **SSIM** | 0.002 | 0.303 | 0.873 |

**Key insight:** DHCOD achieves dramatically better reconstruction quality because XOR decryption has no inherent contrast loss, unlike OR-based VCS/RG.

## Storage Efficiency

| Scheme | Input | Share Size | Expansion | Total Storage |
|--------|-------|------------|-----------|---------------|
| **VCS** | 200×200 | 400×200 each | 2× width | 4× original |
| **RG** | 200×200 | 200×200 each | None | 2× original |
| **DHCOD** | 200×200 | 200×200 each | None | 2× original |

## File Sizes (P2 ASCII PGM)

| Scheme | Share 1 | Share 2 |
|--------|---------|---------|
| **VCS** | 240,258 B | 240,258 B |
| **RG** | 120,386 B | 120,388 B |
| **DHCOD** | 119,858 B | 123,838 B |

## When to Use Each Scheme

| Requirement | Best Scheme |
|-------------|-------------|
| Maximum security + physical decryption | **VCS** or **RG** |
| Space efficiency + security | **RG** |
| Steganographic camouflage | **DHCOD** |
| Best reconstruction quality | **DHCOD** |
| Simplest implementation | **RG** |
| Classic/well-studied approach | **VCS** |

## Reproducing Results

```bash
# Generate shares
bin/vc_program

# Run full analysis
bin/analyze

# Export to CSV for graphing
bin/analyze --csv output/results.csv
```
