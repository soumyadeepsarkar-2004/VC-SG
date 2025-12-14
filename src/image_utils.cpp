#include "image_utils.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

// Helper to skip comments in PGM
void ignoreComments(std::ifstream& file) {
    char c = file.peek();
    while (c == '#' || c == '\n' || c == '\r' || c == '\t' || c == ' ') {
        if (c == '#') {
            std::string line;
            std::getline(file, line);
        } else {
            file.get(); // consume whitespace
        }
        c = file.peek();
    }
}

Image loadPGM(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return Image(0, 0);
    }

    std::string format;
    file >> format;
    if (format != "P2" && format != "P5") {
        std::cerr << "Error: Unsupported PGM format " << format << " (Only P2/P5 supported)" << std::endl;
        return Image(0, 0);
    }

    ignoreComments(file);

    int w, h, maxVal;
    file >> w >> h >> maxVal;
    
    ignoreComments(file);

    Image img(w, h);

    if (format == "P2") {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                file >> img.pixels[i][j];
            }
        }
    } else { // P5
        // P5 is binary. Needs to read a single whitespace char after header before data.
        // We already did ignoreComments, but we might have consumed start of data if not careful.
        // Actually, ignoreComments peaks. 
        // Let's rely on standard extraction skipping whitespace for P2.
        // For P5, we need to read raw bytes.
        // Re-opening or careful handling is needed. 
        // For simplicity in this project, let's assume P2 (ASCII) for inputs if possible, 
        // or just handle P5 basic byte read.
        file.get(); // consume single whitespace after maxVal usually
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                unsigned char c = file.get();
                img.pixels[i][j] = static_cast<int>(c);
            }
        }
    }

    return img;
}

bool savePGM(const std::string& filename, const Image& img) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    // Writing P2 (ASCII) for simplicity
    file << "P2\n";
    file << "# Created by VC_RG_Comparator\n";
    file << img.width << " " << img.height << "\n";
    file << "255\n"; // Max val

    for (int i = 0; i < img.height; ++i) {
        for (int j = 0; j < img.width; ++j) {
            // Internal 1 = Black, 0 = White.
            // PGM 0 = Black, 255 = White.
            // So if internal is 1 (Black), write 0. If 0 (White), write 255.
            int val = (img.pixels[i][j] == 1) ? 0 : 255;
            
            // If the image is not strictly binary (just loaded grayscale), write as is?
            // But we want to strictly support our logic.
            // Let's check max val. If > 1, assume it's normal grayscale.
            if (img.pixels[i][j] > 1) val = img.pixels[i][j];

            file << val << " ";
        }
        file << "\n";
    }

    return true;
}

Image binarizeImage(const Image& input, int threshold) {
    Image res(input.width, input.height);
    for (int i = 0; i < input.height; ++i) {
        for (int j = 0; j < input.width; ++j) {
            // Standard: val < threshold -> Black (0 in PGM, 1 in Internal)
            // val >= threshold -> White (255 in PGM, 0 in Internal)
            res.pixels[i][j] = (input.pixels[i][j] < threshold) ? 1 : 0;
        }
    }
    return res;
}

Image halftoneImage(const Image& input) {
    // 4x4 Bayer Matrix
    // Values scaled to 0-255 range conceptually (Bayer is 0-15).
    // Threshold = (M[y%4][x%4] + 0.5) * (255/16)
    static const int bayer[4][4] = {
        { 0,  8,  2, 10},
        {12,  4, 14,  6},
        { 3, 11,  1,  9},
        {15,  7, 13,  5}
    };

    Image res(input.width, input.height);
    for (int y = 0; y < input.height; ++y) {
        for (int x = 0; x < input.width; ++x) {
            int val = input.pixels[y][x];
            // Since input might be already 0/1, check range.
            // If input is 0-1, we assume it's already binary-ish, but let's assume 0-255 for grayscale.
            // Internal 1=Black, 0=White... wait. 
            // Standard Grayscale PGM: 0=Black, 255=White.
            // Internal Image: We store raw values from PGM (0-255).
            // BUT our `binarizeImage` produces 0 or 1.
            // If we are halftoning, we expect Grayscale inputs (0-255).
            
            // Map Bayer (0-15) to 0-255 threshold.
            int threshold = (bayer[y % 4][x % 4] * 17); // 16 * 16 = 256 approx. 17 * 15 = 255.
            
            // If Pixel > Threshold -> White (0 Internal)
            // If Pixel <= Threshold -> Black (1 Internal)
            res.pixels[y][x] = (val > threshold) ? 0 : 1;
        }
    }
    return res;
}
