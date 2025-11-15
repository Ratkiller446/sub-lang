# Contributing to SUB Language

Thank you for your interest in contributing to SUB Language! This document provides guidelines for contributing to the project.

## How to Contribute

### Reporting Bugs
- Use GitHub Issues to report bugs
- Include SUB version, OS, and steps to reproduce
- Provide sample .sb code that demonstrates the issue

### Suggesting Features
- Open a GitHub Issue with the "enhancement" label
- Describe the feature and its use case
- Explain how it improves the language

### Code Contributions

1. **Fork the Repository**
   ```bash
   git clone https://github.com/subhobhai943/sub-lang.git
   cd sub-lang
   ```

2. **Create a Branch**
   ```bash
   git checkout -b feature/my-feature
   ```

3. **Make Changes**
   - Follow the coding style
   - Add tests for new features
   - Update documentation

4. **Test Your Changes**
   ```bash
   make clean
   make
   make test
   ```

5. **Commit and Push**
   ```bash
   git add .
   git commit -m "Add: Description of changes"
   git push origin feature/my-feature
   ```

6. **Create Pull Request**
   - Describe your changes
   - Reference related issues
   - Wait for review

## Coding Standards

### C Code Style
- Use 4 spaces for indentation
- Follow K&R style bracing
- Add comments for complex logic
- Keep functions under 50 lines when possible

### Naming Conventions
- Functions: `snake_case`
- Structs: `PascalCase`  
- Constants: `UPPER_SNAKE_CASE`
- Variables: `snake_case`

### Documentation
- Update README.md for user-facing changes
- Add inline comments for complex algorithms
- Update language specification for syntax changes

## Development Setup

### Prerequisites
- GCC or Clang compiler
- Make build system
- Git

### Building
```bash
make clean
make
```

### Testing
```bash
./sub example.sb web
```

## Language Design Principles

When contributing to SUB, keep these principles in mind:

1. **Simplicity First** - Code should be easy to read and write
2. **Blockchain Method** - Use # for visual clarity
3. **Cross-Platform** - Support all target platforms
4. **Performance** - Optimize for speed
5. **Compatibility** - Maintain backward compatibility

## Questions?

Feel free to open an issue or contact the maintainers.

Thank you for contributing! 🚀
