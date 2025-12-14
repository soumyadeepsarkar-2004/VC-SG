# Makefile for Visual Cryptography Project
# Reorganized folder structure

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

# Directories
SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build

# Source files (with paths)
MAIN_SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/image_utils.cpp $(SRC_DIR)/vcs.cpp $(SRC_DIR)/rg.cpp $(SRC_DIR)/dhcod.cpp
ANALYZE_SOURCES = $(SRC_DIR)/analyze.cpp $(SRC_DIR)/image_utils.cpp $(SRC_DIR)/vcs.cpp $(SRC_DIR)/rg.cpp $(SRC_DIR)/dhcod.cpp

# Target executables
MAIN_TARGET = $(BIN_DIR)/vc_program
ANALYZE_TARGET = $(BIN_DIR)/analyze

# Default target
all: $(MAIN_TARGET) $(ANALYZE_TARGET)

# Build main program
$(MAIN_TARGET): $(MAIN_SOURCES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(MAIN_TARGET) $(MAIN_SOURCES)
	@echo "✓ Main program built: $(MAIN_TARGET)"

# Build analyze program
$(ANALYZE_TARGET): $(ANALYZE_SOURCES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(ANALYZE_TARGET) $(ANALYZE_SOURCES)
	@echo "✓ Analysis tool built: $(ANALYZE_TARGET)"

# Clean build artifacts
clean:
	rm -f $(BIN_DIR)/*
	@echo "Cleaned executables"

# Clean output images
clean-output:
	rm -f output/*.pgm output/vcs/*.pgm output/rg/*.pgm output/dhcod/*.pgm
	@echo "Cleaned output files"

# Clean everything
clean-all: clean clean-output
	@echo "Cleaned everything"

# Run the main program
run: $(MAIN_TARGET)
	cd src && ../$(MAIN_TARGET)

# Run analysis
analyze: $(ANALYZE_TARGET)
	cd src && ../$(ANALYZE_TARGET)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: clean all
	@echo "Debug build complete"

# Help
help:
	@echo "Visual Cryptography Project - Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  make           - Build all programs"
	@echo "  make all       - Same as make"
	@echo "  make clean     - Remove executables"
	@echo "  make clean-output - Remove output PGM files"
	@echo "  make clean-all - Remove all generated files"
	@echo "  make run       - Build and run main program"
	@echo "  make analyze   - Build and run analysis tool"
	@echo "  make debug     - Build with debug symbols"
	@echo "  make help      - Show this help"

# Phony targets
.PHONY: all clean clean-output clean-all run analyze debug help
