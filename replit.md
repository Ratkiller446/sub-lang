# SUB Programming Language Compiler

## Overview
SUB is a programming language designed for cross-platform development with a "blockchain-style" hash syntax. The compiler translates `.sb` source files into code for various platforms (web, Android, iOS, Windows, macOS, Linux).

## Project Structure
```
.
├── sub.c               # Main compiler entry point
├── lexer.c             # Lexical analyzer (tokenizer)
├── parser.c            # Parser (AST generation)
├── semantic.c          # Semantic analyzer
├── codegen.c           # Code generator
├── utils.c             # Utility functions
├── sub_compiler.h      # Main header file
├── error_handler.h     # Error handling header
├── example.sb          # Example SUB program
├── Makefile            # Build configuration
├── CMakeLists.txt      # CMake build (for full multi-language build)
├── build.sh            # Full build script (Rust + C++ + C)
├── compiler/           # Multi-language compiler components
│   ├── frontend/       # Rust frontend (lexer/parser)
│   ├── middle_end/     # C++ middle-end (semantic analysis)
│   ├── backend/        # C backend (code generation)
│   └── main.c          # Entry point for heavy compiler
└── compiler_rust/      # Standalone Rust compiler
```

## Build & Run
The project uses a Makefile for the C-only build:

```bash
make          # Build the compiler
make clean    # Clean build artifacts
./sub <file>.sb <platform>  # Compile a SUB program
```

Supported platforms: web, android, ios, windows, macos, linux, native

## Current State
- C-only compiler is functional and builds successfully
- Compiles SUB source files to target platform code
- Full multi-language build (Rust + C++ + C) requires additional setup

## Languages & Tools
- C11 (primary compiler implementation)
- Rust (optional frontend)
- C++17 (optional middle-end)
- GCC for compilation
- CMake for full build

## Recent Changes
- 2025-12-26: Initial setup in Replit environment
