# Makefile for Visual Cryptography Project (VC-SG)
# Supports: main program, analysis tool, test suite

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -Wpedantic -O2

# Directories
SRC_DIR = src
BIN_DIR = bin

# Common source files
COMMON = $(SRC_DIR)/image_utils.cpp $(SRC_DIR)/vcs.cpp $(SRC_DIR)/rg.cpp $(SRC_DIR)/dhcod.cpp

# Source files per target
MAIN_SOURCES    = $(SRC_DIR)/main.cpp $(COMMON)
ANALYZE_SOURCES = $(SRC_DIR)/analyze.cpp $(COMMON)
TEST_SOURCES    = $(SRC_DIR)/test.cpp $(COMMON)

# Target executables
MAIN_TARGET    = $(BIN_DIR)/vc_program
ANALYZE_TARGET = $(BIN_DIR)/analyze
TEST_TARGET    = $(BIN_DIR)/vc_test

# Default target
all: $(MAIN_TARGET) $(ANALYZE_TARGET) $(TEST_TARGET)

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

# Build test suite
$(TEST_TARGET): $(TEST_SOURCES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_SOURCES)
	@echo "✓ Test suite built: $(TEST_TARGET)"

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
	./$(MAIN_TARGET)

# Run analysis
analyze: $(ANALYZE_TARGET)
	./$(ANALYZE_TARGET)

# Run tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: clean all
	@echo "Debug build complete"

# Help
help:
	@echo "Visual Cryptography Project (VC-SG) — Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  make           - Build all programs"
	@echo "  make all       - Same as make"
	@echo "  make clean     - Remove executables"
	@echo "  make clean-output - Remove output PGM files"
	@echo "  make clean-all - Remove all generated files"
	@echo "  make run       - Build and run main program"
	@echo "  make analyze   - Build and run analysis tool"
	@echo "  make test      - Build and run test suite"
	@echo "  make debug     - Build with debug symbols"
	@echo "  make help      - Show this help"

# Phony targets
.PHONY: all clean clean-output clean-all run analyze test debug help
