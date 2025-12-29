# Contributing to SUB Language

Thank you for your interest in contributing to SUB Language! This guide will help you contribute to the project step by step.

## 🌍 Language Integration

**Any contributor can integrate their favorite language into SUB!** Whether it's Python, JavaScript, Java, or any other language, you can help expand SUB's cross-platform capabilities. Follow the steps below to get started.

## 📋 Step-by-Step Contribution Guide

### Step 1: Fork and Clone
```bash
# Fork the repository on GitHub, then clone your fork
git clone https://github.com/YOUR_USERNAME/sub-lang.git
cd sub-lang
```

### Step 2: Create a Feature Branch
```bash
git checkout -b feature/your-feature-name
# Example: git checkout -b feature/add-python-backend
```

### Step 3: Set Up Development Environment
```bash
# Install prerequisites (GCC/Clang, Make)
make clean
make
```

### Step 4: Make Your Changes
- Add your language backend in appropriate files
- Follow the coding standards (see below)
- Test thoroughly on your platform

### Step 5: Test Your Changes
```bash
make clean
make
./sub example.sb [your-target]
```

### Step 6: Commit and Push
```bash
git add .
git commit -m "Add: Brief description of your changes"
git push origin feature/your-feature-name
```

### Step 7: Create Pull Request
- Go to your fork on GitHub
- Click "New Pull Request"
- Describe your changes in detail
- Reference any related issues
- Wait for review and address feedback

## ⚠️ Known Issues

### Windows Compatibility (v1.0.3)

**Current Status:** v1.0.3 does not support Windows due to POSIX-specific functions.

#### Issues:

| Issue        | Location          | Problem                  |
|--------------|-------------------|--------------------------|  
| `strdup()`   | 20+ files         | MSVC wants `_strdup()`   |
| `<strings.h>`| sub_multilang.c:9 | Doesn't exist on Windows |
| `strcasecmp()`| sub_multilang.c  | Windows uses `_stricmp()`|

#### The Fix for v1.0.4:

**Contributors can fix this in v1.0.4!** Add this at the top of affected files:

```c
#ifdef _WIN32
#define strdup _strdup
#define strcasecmp _stricmp
#endif
```

And replace `<strings.h>` include:

```c
// From:
#include <strings.h>

// To:
#ifndef _WIN32
#include <strings.h>
#endif
```

**Affected Files:** Check all C files using these functions (20+ files need updates).

## 🎯 Roadmap & Future Targets

Our next major goals are:

1. **Rust Compiler Architecture Integration** - Add Rust as a compilation target
2. **C++ Compiler Architecture Integration** - Enhanced C++ backend support  
3. **C Compiler Architecture Integration** - Improved C code generation
4. **Full Windows Support (v1.0.4)** - Fix POSIX compatibility issues

Want to work on these? Open an issue or submit a PR!

## 🐛 Reporting Bugs

- Use GitHub Issues to report bugs
- Include:
  - SUB version (e.g., v1.0.3)
  - Operating System (Windows/macOS/Linux)
  - Steps to reproduce
  - Sample .sb code demonstrating the issue

## 💡 Suggesting Features

- Open a GitHub Issue with the "enhancement" label
- Describe the feature and its use case
- Explain how it improves SUB Language

## 📝 Coding Standards

### C Code Style
- **Indentation:** 4 spaces (no tabs)
- **Bracing:** K&R style
- **Comments:** Add for complex logic
- **Function Length:** Keep under 50 lines when possible

### Naming Conventions
- **Functions:** `snake_case` (e.g., `compile_to_web`)
- **Structs:** `PascalCase` (e.g., `ASTNode`)
- **Constants:** `UPPER_SNAKE_CASE` (e.g., `MAX_BUFFER_SIZE`)
- **Variables:** `snake_case` (e.g., `token_count`)

### Cross-Platform Considerations
- Always test on multiple platforms when possible
- Use `#ifdef` for platform-specific code
- Avoid POSIX-only functions (see Windows compatibility above)
- Document platform limitations

### Documentation
- Update README.md for user-facing changes
- Add inline comments for complex algorithms
- Update language specification for syntax changes
- Add examples for new features

## 🏗️ Development Setup

### Prerequisites
- **Compiler:** GCC or Clang
- **Build System:** Make
- **Version Control:** Git
- **OS:** Linux/macOS (Windows support coming in v1.0.4)

### Building from Source
```bash
make clean
make
```

### Running Examples
```bash
# Compile to web
./sub example.sb web

# Compile to Android
./sub example.sb android

# Compile to iOS
./sub example.sb ios
```

## 🎨 Language Design Principles

When contributing to SUB, keep these principles in mind:

1. **Simplicity First** - Code should be intuitive and easy to read
2. **Blockchain Method** - Use `#` syntax for visual clarity
3. **Cross-Platform** - Support all target platforms equally
4. **Performance** - Optimize for speed without sacrificing readability
5. **Compatibility** - Maintain backward compatibility when possible
6. **Accessibility** - Make it the "world's easiest" language to use

## ❓ Questions or Help?

Feel free to:
- Open a GitHub Issue for questions
- Start a GitHub Discussion
- Contact the maintainers

---

**Thank you for contributing to SUB Language! Together we're building the world's easiest cross-platform programming language.** 🚀
