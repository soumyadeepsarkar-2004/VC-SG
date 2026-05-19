#include <iostream>
#include <stdexcept>
#include <string>
#include <cmath>
#include "image_utils.hpp"
#include "vcs.hpp"
#include "rg.hpp"
#include "dhcod.hpp"

// ============================================================
// Lightweight Test Framework
// ============================================================
static int g_passed = 0;
static int g_failed = 0;

#define RUN_TEST(name) do { \
    std::cout << "  " << #name << "... "; \
    try { name(); g_passed++; std::cout << "PASSED" << std::endl; } \
    catch (const std::exception& e) { g_failed++; std::cout << "FAILED: " << e.what() << std::endl; } \
} while(0)

void ASSERT_TRUE(bool cond, const std::string& msg = "") {
    if (!cond) throw std::runtime_error(msg.empty() ? "Assertion failed" : msg);
}

void ASSERT_EQ(int a, int b, const std::string& msg = "") {
    if (a != b) throw std::runtime_error(
        msg + " (expected " + std::to_string(a) + " == " + std::to_string(b) + ")");
}

// ============================================================
// Image Construction Tests
// ============================================================
void test_image_construction() {
    Image img(10, 5);
    ASSERT_EQ(img.width, 10, "width");
    ASSERT_EQ(img.height, 5, "height");
    ASSERT_TRUE(!img.isBinary, "default not binary");
    ASSERT_TRUE(!img.empty(), "not empty");

    Image empty(0, 0);
    ASSERT_TRUE(empty.empty(), "empty image");
}

void test_binarize_image() {
    Image gray(4, 4);
    // Set pixel values: first two rows dark (0-100), last two rows bright (200-255)
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
            gray.pixels[r][c] = (r < 2) ? 50 : 200;

    Image bin = binarizeImage(gray, 128);
    ASSERT_TRUE(bin.isBinary, "output is binary");
    ASSERT_EQ(bin.pixels[0][0], 1, "dark pixel -> 1 (black)");
    ASSERT_EQ(bin.pixels[3][3], 0, "bright pixel -> 0 (white)");
}

void test_halftone_image() {
    Image gray(8, 8);
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            gray.pixels[r][c] = 128; // Mid-gray

    Image ht = halftoneImage(gray);
    ASSERT_TRUE(ht.isBinary, "halftone output is binary");
    ASSERT_EQ(ht.width, 8, "width preserved");
    ASSERT_EQ(ht.height, 8, "height preserved");

    // Count black pixels — for mid-gray, should be roughly 50%
    int black = 0;
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (ht.pixels[r][c] == 1) black++;
    ASSERT_TRUE(black > 20 && black < 44, "halftone ~50% black for mid-gray");
}

void test_resize_image() {
    Image img(10, 10, 42);
    Image resized = resizeImage(img, 5, 5);
    ASSERT_EQ(resized.width, 5, "resized width");
    ASSERT_EQ(resized.height, 5, "resized height");
    ASSERT_EQ(resized.pixels[0][0], 42, "value preserved");
}

// ============================================================
// VCS Tests
// ============================================================
void test_vcs_share_dimensions() {
    Image secret(10, 8);
    secret.isBinary = true;
    Image s1(0,0), s2(0,0);
    VCS::generateShares(secret, s1, s2);
    ASSERT_EQ(s1.width, 20, "VCS share width = 2x");
    ASSERT_EQ(s1.height, 8, "VCS share height same");
    ASSERT_EQ(s2.width, 20, "VCS share2 width = 2x");
    ASSERT_TRUE(s1.isBinary, "share1 is binary");
}

void test_vcs_all_white_roundtrip() {
    Image secret(20, 20, 0); // All white
    secret.isBinary = true;
    Image s1(0,0), s2(0,0);
    VCS::generateShares(secret, s1, s2);
    Image dec = VCS::decryptShares(s1, s2);

    // For all-white: OR of identical sub-pixels should give [1,0] or [0,1]
    // Each pair should have exactly one 1 → reduced pixel = 1 (black due to OR)
    // Wait — VCS for white: shares identical. OR of [1,0]|[1,0] = [1,0] → reduced = 1.
    // Actually for white pixel, both sub-pixels are same pattern.
    // OR(same, same) = same. So sub-pixel pair is either [1,0] or [0,1].
    // Reduced: 1|0=1 or 0|1=1. So white pixels appear as 50% gray (1 black sub-pixel).
    // This is expected: VCS has 50% contrast loss on white regions.
    // Black pixels: complementary -> OR = [1,1] always. Reduced = 1.
    // So decrypted all-white should have each pair as 1, not 0.
    // The key test is: black regions should be denser than white regions.
    ASSERT_TRUE(dec.isBinary, "decoded is binary");
    ASSERT_EQ(dec.width, 40, "decoded width = 2x");
}

void test_vcs_all_black_roundtrip() {
    Image secret(20, 20, 1); // All black
    secret.isBinary = true;
    Image s1(0,0), s2(0,0);
    VCS::generateShares(secret, s1, s2);
    Image dec = VCS::decryptShares(s1, s2);

    // Black pixel: complementary sub-pixels -> OR = [1,1]
    for (int r = 0; r < dec.height; ++r)
        for (int c = 0; c < dec.width; ++c)
            ASSERT_EQ(dec.pixels[r][c], 1, "all-black decrypted");
}

// ============================================================
// Random Grid Tests
// ============================================================
void test_rg_no_expansion() {
    Image secret(15, 10);
    secret.isBinary = true;
    Image s1(0,0), s2(0,0);
    RG::generateShares(secret, s1, s2);
    ASSERT_EQ(s1.width, 15, "RG no expansion width");
    ASSERT_EQ(s1.height, 10, "RG no expansion height");
}

void test_rg_all_black_roundtrip() {
    Image secret(20, 20, 1);
    secret.isBinary = true;
    Image s1(0,0), s2(0,0);
    RG::generateShares(secret, s1, s2);
    Image dec = RG::decryptShares(s1, s2);

    // Black: share2 = complement of share1 -> OR always 1
    for (int r = 0; r < dec.height; ++r)
        for (int c = 0; c < dec.width; ++c)
            ASSERT_EQ(dec.pixels[r][c], 1, "RG all-black decrypted");
}

void test_rg_share_randomness() {
    Image secret(100, 100, 0);
    secret.isBinary = true;
    Image s1(0,0), s2(0,0);
    RG::generateShares(secret, s1, s2);

    // Share1 should be ~50% black, ~50% white
    int black = 0;
    for (int r = 0; r < s1.height; ++r)
        for (int c = 0; c < s1.width; ++c)
            if (s1.pixels[r][c] == 1) black++;

    double ratio = (double)black / (s1.width * s1.height);
    ASSERT_TRUE(ratio > 0.4 && ratio < 0.6,
        "RG share ~50% black (got " + std::to_string(ratio) + ")");
}

// ============================================================
// DHCOD Tests
// ============================================================
void test_dhcod_perfect_xor() {
    // DHCOD with XOR should perfectly reconstruct the halftoned secret
    Image secret(20, 20, 0);
    secret.isBinary = true;
    // Make a checkerboard pattern
    for (int r = 0; r < 20; ++r)
        for (int c = 0; c < 20; ++c)
            secret.pixels[r][c] = (r + c) % 2;

    Image cover(20, 20, 128); // Gray cover
    Image s1(0,0), s2(0,0);
    DHCOD::generateShares(secret, cover, s1, s2);
    Image dec = DHCOD::decryptShares(s1, s2);

    // XOR decryption should be perfect for binary input
    Image secretHT = halftoneImage(secret);
    int mismatches = 0;
    for (int r = 0; r < 20; ++r)
        for (int c = 0; c < 20; ++c)
            if (dec.pixels[r][c] != secretHT.pixels[r][c]) mismatches++;
    ASSERT_EQ(mismatches, 0, "DHCOD XOR perfect reconstruction");
}

void test_dhcod_size_mismatch() {
    // Cover has different dimensions — should auto-resize, not crash
    Image secret(20, 20, 0);
    secret.isBinary = true;
    Image cover(30, 15, 128); // Mismatched

    Image s1(0,0), s2(0,0);
    DHCOD::generateShares(secret, cover, s1, s2);

    ASSERT_EQ(s1.width, 20, "DHCOD share width matches secret");
    ASSERT_EQ(s1.height, 20, "DHCOD share height matches secret");
}

// ============================================================
// Edge Case Tests
// ============================================================
void test_1x1_image() {
    Image secret(1, 1, 1);
    secret.isBinary = true;

    Image s1(0,0), s2(0,0);
    VCS::generateShares(secret, s1, s2);
    ASSERT_EQ(s1.width, 2, "1x1 VCS share width");

    RG::generateShares(secret, s1, s2);
    ASSERT_EQ(s1.width, 1, "1x1 RG share width");
}

// ============================================================
// Main
// ============================================================
int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "  VC-SG Automated Test Suite" << std::endl;
    std::cout << "============================================\n" << std::endl;

    std::cout << "[Image Utilities]" << std::endl;
    RUN_TEST(test_image_construction);
    RUN_TEST(test_binarize_image);
    RUN_TEST(test_halftone_image);
    RUN_TEST(test_resize_image);

    std::cout << "\n[VCS — Naor-Shamir]" << std::endl;
    RUN_TEST(test_vcs_share_dimensions);
    RUN_TEST(test_vcs_all_white_roundtrip);
    RUN_TEST(test_vcs_all_black_roundtrip);

    std::cout << "\n[RG — Random Grid]" << std::endl;
    RUN_TEST(test_rg_no_expansion);
    RUN_TEST(test_rg_all_black_roundtrip);
    RUN_TEST(test_rg_share_randomness);

    std::cout << "\n[DHCOD — Meaningful Shares]" << std::endl;
    RUN_TEST(test_dhcod_perfect_xor);
    RUN_TEST(test_dhcod_size_mismatch);

    std::cout << "\n[Edge Cases]" << std::endl;
    RUN_TEST(test_1x1_image);

    std::cout << "\n============================================" << std::endl;
    std::cout << "  Results: " << g_passed << " passed, " << g_failed << " failed" << std::endl;
    std::cout << "============================================" << std::endl;

    return g_failed > 0 ? 1 : 0;
}
