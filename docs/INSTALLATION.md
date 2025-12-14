# 📦 Installation Guide

## Visual Cryptography Project - Setup Instructions

This guide will help you set up and run the Visual Cryptography project on your system.

---

## 📋 Prerequisites

### Minimum Requirements
- **C++ Compiler** with C++11 support
- **Operating System**: Windows, Linux, or macOS
- **RAM**: 256 MB minimum
- **Disk Space**: 50 MB for project + outputs
- **No external libraries required** - Pure C++ standard library

### Recommended Compilers

| Platform | Compiler | Version | Command |
|----------|----------|---------|---------|
| Windows | MinGW (g++) | 6.3.0+ | `g++ --version` |
| Windows | MSVC | 2015+ | `cl` |
| Linux | GCC | 4.8+ | `g++ --version` |
| macOS | Clang | 3.3+ | `clang++ --version` |

---

## 🚀 Quick Installation

### Option 1: Windows (Recommended)

1. **Download/Clone the project**
   ```cmd
   cd C:\
   git clone <repository-url> VC_NS
   cd VC_NS
   ```

2. **Verify compiler**
   ```cmd
   g++ --version
   ```
   If not installed, download MinGW from: https://sourceforge.net/projects/mingw/

3. **Build the project**
   ```cmd
   build.bat
   ```

4. **Run the program**
   ```cmd
   bin\vc_program.exe
   bin\analyze.exe
   ```

### Option 2: Linux/macOS

1. **Download/Clone the project**
   ```bash
   cd ~
   git clone <repository-url> VC_NS
   cd VC_NS
   ```

2. **Verify compiler**
   ```bash
   g++ --version
   ```
   If not installed:
   - **Ubuntu/Debian**: `sudo apt-get install g++`
   - **Fedora**: `sudo dnf install gcc-c++`
   - **macOS**: `xcode-select --install`

3. **Build the project**
   ```bash
   chmod +x build.sh
   ./build.sh
   ```

4. **Run the program**
   ```bash
   ./bin/vc_program
   ./bin/analyze
   ```

### Option 3: Using Makefile (Universal)

```bash
make          # Build all
make run      # Build and run main program
make analyze  # Build and run analysis tool
make clean    # Clean executables
make help     # Show all targets
```

---

## 🔧 Detailed Installation

### Step 1: Install C++ Compiler

#### Windows Users

**Option A: MinGW (Recommended)**
1. Download MinGW installer: https://sourceforge.net/projects/mingw/
2. Run installer and select "mingw32-gcc-g++" package
3. Add to PATH: `C:\MinGW\bin`
4. Verify: Open CMD and run `g++ --version`

**Option B: Visual Studio**
1. Download Visual Studio Community: https://visualstudio.microsoft.com/
2. During installation, select "Desktop development with C++"
3. Verify: Open "Developer Command Prompt" and run `cl`

#### Linux Users

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential
g++ --version
```

**Fedora/RHEL:**
```bash
sudo dnf install gcc-c++
g++ --version
```

**Arch Linux:**
```bash
sudo pacman -S gcc
g++ --version
```

#### macOS Users

**Using Xcode Command Line Tools:**
```bash
xcode-select --install
g++ --version
```

**Using Homebrew:**
```bash
brew install gcc
g++ --version
```

---

### Step 2: Download Project

#### Option A: Git Clone (Recommended)
```bash
git clone <repository-url> VC_NS
cd VC_NS
```

#### Option B: Download ZIP
1. Download ZIP from repository
2. Extract to desired location
3. Navigate to extracted folder

---

### Step 3: Build Project

The project provides three build methods. Choose one:

#### Method 1: Automated Build Scripts

**Windows:**
```cmd
build.bat
```

**Linux/macOS:**
```bash
chmod +x build.sh
./build.sh
```

#### Method 2: Makefile
```bash
make
```

#### Method 3: Manual Compilation

**Windows:**
```cmd
g++ -std=c++11 -o bin/vc_program.exe src/main.cpp src/image_utils.cpp src/vcs.cpp src/rg.cpp src/dhcod.cpp
g++ -std=c++11 -o bin/analyze.exe src/analyze.cpp src/image_utils.cpp src/vcs.cpp src/rg.cpp src/dhcod.cpp
```

**Linux/macOS:**
```bash
mkdir -p bin
g++ -std=c++11 -o bin/vc_program src/main.cpp src/image_utils.cpp src/vcs.cpp src/rg.cpp src/dhcod.cpp
g++ -std=c++11 -o bin/analyze src/analyze.cpp src/image_utils.cpp src/vcs.cpp src/rg.cpp src/dhcod.cpp
```

---

### Step 4: Verify Installation

#### Test Compilation

You should see output like:
```
✓ Compiling image_utils.cpp...
✓ Compiling vcs.cpp...
✓ Compiling rg.cpp...
✓ Compiling dhcod.cpp...
✓ Compiling main.cpp...
✓ Linking vc_program...
✓ Build complete!
```

#### Test Execution

**Windows:**
```cmd
bin\vc_program.exe
```

**Linux/macOS:**
```bash
./bin/vc_program
```

Expected output:
```
Visual Cryptography Schemes - Implementation

Creating sample images...
✓ Created input/input.pgm
✓ Created input/cover.pgm

Running VCS (Naor-Shamir)...
✓ Generated VCS shares
✓ Decrypted VCS secret

Running Random Grid...
✓ Generated RG shares
✓ Decrypted RG secret

Running DHCOD...
✓ Generated DHCOD shares
✓ Decrypted DHCOD secret

All schemes completed successfully!
Check output/ directory for results.
```

---

## 📁 Post-Installation

After successful installation, your directory should look like:

```
VC_NS/
├── bin/
│   ├── vc_program.exe      # Main executable
│   └── analyze.exe          # Analysis tool
├── input/
│   ├── input.pgm            # Sample secret image
│   └── cover.pgm            # Sample cover image
└── output/
    ├── vcs/                 # VCS outputs
    ├── rg/                  # RG outputs
    └── dhcod/               # DHCOD outputs
```

---

## ❓ Troubleshooting

### Issue: "g++ is not recognized"

**Solution:**
- Windows: Add MinGW to PATH (`C:\MinGW\bin`)
- Linux/macOS: Install GCC (`sudo apt-get install g++`)

### Issue: "Permission denied"

**Solution (Linux/macOS):**
```bash
chmod +x build.sh
chmod +x bin/vc_program
```

### Issue: "C++11 not supported"

**Solution:**
- Update your compiler to a newer version
- Minimum: GCC 4.8+, Clang 3.3+, MSVC 2015+

### Issue: Build fails with errors

**Solution:**
1. Check compiler version: `g++ --version`
2. Ensure all source files are present in `src/`
3. Try manual compilation (see Method 3)
4. Check error messages carefully

### Issue: "Cannot find -lstdc++"

**Solution (Linux):**
```bash
sudo apt-get install libstdc++6
```

### Issue: Output images not created

**Solution:**
1. Ensure directories exist: `input/`, `output/`, `output/vcs/`, `output/rg/`, `output/dhcod/`
2. Check write permissions
3. Run from project root directory

---

## 🔄 Updating

To update the project:

```bash
git pull origin main
make clean
make
```

---

## 🧹 Uninstallation

To remove the project:

**Windows:**
```cmd
cd ..
rmdir /s /q VC_NS
```

**Linux/macOS:**
```bash
cd ..
rm -rf VC_NS
```

---

## 📚 Next Steps

After installation:

1. ✅ **Run the program**: Generate sample outputs
2. 📖 **Read documentation**: Check `docs/README.md`
3. 🔬 **Run analysis**: Execute `analyze` tool
4. 🎨 **Try custom images**: Replace `input/input.pgm`
5. 🔧 **Modify code**: Experiment with parameters

---

## 💡 Tips

- **Performance**: Compilation takes ~5-10 seconds on modern systems
- **IDE Support**: Project works with VS Code, Visual Studio, CLion, etc.
- **Debugging**: Add `-g` flag for debug symbols
- **Optimization**: Add `-O2` or `-O3` for faster execution

---

## 📞 Support

For installation issues:
- 📖 Check [QUICK_REFERENCE.md](docs/QUICK_REFERENCE.md)
- 📚 Read [README.md](README.md)
- 🔧 See [TECHNICAL_GUIDE.md](docs/TECHNICAL_GUIDE.md)

---

**✅ Installation Complete! Ready to explore visual cryptography!**
