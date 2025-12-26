# SUB Programming Language Compiler

## Overview
SUB is a cross-platform programming language with "blockchain-style" hash (#) syntax. This project contains the compiler that translates SUB source code (.sb files) into various target platforms.

## Project Structure
```
├── sub.c             # Main compiler driver
├── lexer.c           # Lexical analysis (tokenizer)
├── parser.c          # Syntax parsing (AST builder)
├── semantic.c        # Semantic analysis
├── codegen.c         # Code generation for multiple platforms
├── utils.c           # Utility functions
├── sub_compiler.h    # Main header with types and declarations
├── error_handler.h   # Error handling utilities
├── example.sb        # Example SUB program
├── Makefile          # Build configuration
└── compiler/         # Advanced multi-language compiler (Rust/C++/C)
```

## Building
The project uses a standard C Makefile build:
```bash
make          # Build the compiler
make clean    # Clean build artifacts
./sub <file.sb> <platform>  # Run compiler
```

## Supported Platforms
- `web` - HTML/JavaScript output
- `android` - Java/Kotlin output
- `ios` - Swift output
- `windows` - Win32 C output
- `macos` - Cocoa output
- `linux` - C output

## Usage Example
```bash
./sub example.sb web
```

## Language Features
- Hash (#) prefixed statements for visual clarity
- Variables: `#var name = "value"`
- Functions: `#function name() ... #end`
- Conditionals: `#if`, `#elif`, `#else`, `#end`
- Loops: `#for`, `#while`
- Embedded code: `#embed python/javascript/c/cpp ... #endembed`
- UI components: `#ui.window()`, `#ui.button()`, etc.

## Recent Changes
- 2025-12-26: Initial import and setup for Replit environment
  - Fixed function signature mismatches in header
  - Added _GNU_SOURCE for POSIX compatibility
  - Resolved type naming conflicts in semantic analyzer
