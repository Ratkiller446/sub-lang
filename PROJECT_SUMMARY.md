# SUB Programming Language - Project Summary

## Overview

SUB is a revolutionary cross-platform programming language designed to be the world's easiest language to learn and use. It combines:

✓ **Blockchain-inspired syntax** with # operators for visual clarity
✓ **C-based compiler** for maximum performance  
✓ **Cross-platform compilation** (Android, iOS, Web, Windows, macOS, Linux)
✓ **Multi-language embedding** support (Python, JavaScript, etc.)
✓ **Intuitive and readable** code structure

## Repository Structure

```
sub-lang/
├── sub_compiler.h        # Header with type definitions
├── sub.c                 # Main compiler driver
├── lexer.c               # Lexical analyzer
├── parser.c              # Syntax parser and AST builder
├── semantic.c            # Semantic analyzer
├── codegen.c             # Multi-platform code generator
├── utils.c               # Utility functions
├── Makefile              # Build system
├── example.sb            # Sample SUB program
├── LICENSE               # MIT License
├── .gitignore            # Git ignore rules
├── README.md             # Main documentation
├── CONTRIBUTING.md       # Contribution guidelines
├── LANGUAGE_SPEC.md      # Language specification
└── .github/
    └── workflows/
        └── ci.yml        # CI/CD workflow
```

## Quick Start

### 1. Clone the Repository
```bash
git clone https://github.com/subhobhai943/sub-lang.git
cd sub-lang
```

### 2. Build the Compiler
```bash
make
```

### 3. Test with Example
```bash
./sub example.sb web
```

## Key Features

### Blockchain Method (#)
Every statement starts with #, creating a visual "chain":
```sub
#var name = "Alice"
#function greet()
    #print("Hello")
#end
```

### Cross-Platform Compilation
```bash
./sub myapp.sb android    # Compile for Android
./sub myapp.sb ios        # Compile for iOS  
./sub myapp.sb web        # Compile for Web
./sub myapp.sb windows    # Compile for Windows
./sub myapp.sb macos      # Compile for macOS
./sub myapp.sb linux      # Compile for Linux
```

### Multi-Language Embedding
```sub
#embed python
    import numpy as np
    data = np.array([1,2,3])
#endembed

#embed javascript
    const items = [1, 2, 3];
    console.log(items);
#endembed
```

## Compiler Architecture

1. **Lexical Analysis** (lexer.c) - Tokenize source code
2. **Syntax Analysis** (parser.c) - Build AST
3. **Semantic Analysis** (semantic.c) - Type checking
4. **Optimization** - Code optimization passes
5. **Code Generation** (codegen.c) - Platform-specific output

## Platform Targets

| Platform | Output Language | Framework |
|----------|----------------|-----------|  
| Android | Java/Kotlin | Android SDK |
| iOS | Swift | UIKit |
| Web | JavaScript | HTML5 |
| Windows | C/C++ | Win32 API |
| macOS | Swift/ObjC | Cocoa |
| Linux | C | GTK |

## Development

### Prerequisites
- GCC or Clang compiler
- Make build system
- Git

### Building from Source
```bash
make clean
make
```

### Running Tests
```bash
make test
```

### Installation
```bash
sudo make install
```

## Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for:
- How to report bugs
- How to suggest features
- Coding standards
- Pull request process

## Documentation

- [README.md](README.md) - Main project documentation
- [LANGUAGE_SPEC.md](LANGUAGE_SPEC.md) - Complete language specification  
- [CONTRIBUTING.md](CONTRIBUTING.md) - Contribution guidelines

## License

MIT License - See [LICENSE](LICENSE) file

## Contact

- **GitHub**: https://github.com/subhobhai943/sub-lang
- **Issues**: https://github.com/subhobhai943/sub-lang/issues

## Project Statistics

- **Source Files**: 7 C files + 1 header
- **Documentation**: 4 markdown files
- **Example Programs**: 1 .sb file
- **Build System**: Makefile
- **CI/CD**: GitHub Actions
- **License**: MIT
- **Target Platforms**: 6
- **File Extension**: .sb

## Version

**Current Version**: 1.0  
**Release Date**: 2025  
**Status**: Active Development

---

**Built with ❤️ for developers who want simplicity and power**

*Write Once, Run Everywhere with Maximum Simplicity*
