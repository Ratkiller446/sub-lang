# Contribution Guide - SUB Language

Welcome to the SUB Language project! This guide will walk you through the contribution process step-by-step.

## Quick Start

| Step | Action | Command/Link |
|------|--------|--------------|
| 1 | Fork the repository | [Fork on GitHub](https://github.com/subhobhai943/sub-lang/fork) |
| 2 | Clone your fork | `git clone https://github.com/YOUR_USERNAME/sub-lang.git` |
| 3 | Create a branch | `git checkout -b feature/your-feature-name` |
| 4 | Make changes | Edit files in your preferred editor |
| 5 | Test changes | `make clean && make` |
| 6 | Commit changes | `git commit -am "description"` |
| 7 | Push to fork | `git push origin feature/your-feature-name` |
| 8 | Create Pull Request | [Create PR](https://github.com/subhobhai943/sub-lang/compare) |

## Detailed Step-by-Step Guide

### Step 1: Fork the Repository

1. Visit the [SUB Language repository](https://github.com/subhobhai943/sub-lang)
2. Click the **Fork** button in the top-right corner
3. Wait for GitHub to create your fork

### Step 2: Clone Your Fork

```bash
# Clone your forked repository
git clone https://github.com/YOUR_USERNAME/sub-lang.git

# Navigate to the directory
cd sub-lang

# Add upstream remote to stay updated
git remote add upstream https://github.com/subhobhai943/sub-lang.git
```

### Step 3: Create a Feature Branch

```bash
# Update your main branch
git checkout main
git pull upstream main

# Create and switch to a new branch
git checkout -b feature/your-feature-name
```

**Branch Naming Conventions:**

| Type | Pattern | Example |
|------|---------|---------|
| New Feature | `feature/description` | `feature/add-python-support` |
| Bug Fix | `fix/description` | `fix/memory-leak-parser` |
| Documentation | `docs/description` | `docs/update-readme` |
| Performance | `perf/description` | `perf/optimize-codegen` |
| Refactoring | `refactor/description` | `refactor/clean-headers` |

### Step 4: Make Your Changes

#### For Adding New Language Support

| Task | Files to Modify | What to Add |
|------|----------------|-------------|
| Core Infrastructure | `sub_multilang.c` | Add `LANG_YOURNAME` enum, language info, parse case |
| Code Generator | `codegen_multilang.c` | Implement `codegen_yourname()` function |
| Documentation | `README.md` | Add language to supported list |
| Language Guide | `MULTILANG_GUIDE.md` | Add usage examples |
| Specification | `LANGUAGE_SPEC.md` | Document syntax mappings |
| Tests | `tests/test_yourname_*.sb` | Create test cases (min 5) |

#### For Bug Fixes

1. **Identify the issue**: Reproduce the bug locally
2. **Locate the code**: Find the relevant source file
3. **Fix the bug**: Make minimal necessary changes
4. **Add test**: Create a test case to prevent regression
5. **Document**: Add comments explaining the fix

#### For Documentation

1. **Check grammar**: Use proper spelling and grammar
2. **Add examples**: Include code examples where helpful
3. **Update TOC**: Ensure table of contents is current
4. **Cross-reference**: Link to related documentation

### Step 5: Test Your Changes

| Test Type | Command | Expected Result |
|-----------|---------|----------------|
| Build Test | `make clean && make` | No warnings or errors |
| Syntax Check | `./sublang example.sb language` | Generates valid output |
| Language Validation | `language-tool -c output.file` | "Syntax OK" |
| Regression Test | Test other languages | All still work correctly |
| Edge Cases | Test empty/nested/special cases | Handles gracefully |

**Testing Checklist:**

```bash
# Build the project
make clean && make

# Test your new feature
./sublang tests/your_test.sb language > output.ext

# Verify output syntax (example for Python)
python -m py_compile output.py

# Run existing tests to ensure no regressions
make test  # if test suite exists

# Test edge cases
./sublang tests/edge_case_1.sb language
./sublang tests/edge_case_2.sb language
```

### Step 6: Commit Your Changes

```bash
# Stage your changes
git add .

# Commit with descriptive message
git commit -m "type: brief description

Detailed explanation of what changed and why.

- Bullet point 1
- Bullet point 2
"
```

**Commit Message Format:**

| Prefix | Usage | Example |
|--------|-------|---------|
| `feat:` | New feature | `feat: Add Ruby language support` |
| `fix:` | Bug fix | `fix: Resolve memory leak in parser` |
| `docs:` | Documentation | `docs: Update contribution guide` |
| `test:` | Adding tests | `test: Add edge case tests for loops` |
| `refactor:` | Code refactoring | `refactor: Simplify codegen logic` |
| `perf:` | Performance improvement | `perf: Optimize string concatenation` |
| `style:` | Code style changes | `style: Format with clang-format` |
| `chore:` | Maintenance | `chore: Update dependencies` |

### Step 7: Push to Your Fork

```bash
# Push your branch to your fork
git push origin feature/your-feature-name
```

If you need to make additional changes after pushing:

```bash
# Make changes
git add .
git commit -m "fix: address review feedback"
git push origin feature/your-feature-name
```

### Step 8: Create a Pull Request

1. Visit your fork on GitHub
2. Click **"Compare & pull request"** button
3. Fill out the PR template:

**PR Title Format:** `[Type] Brief description`

**PR Description Should Include:**

| Section | What to Include |
|---------|----------------|
| Summary | What does this PR do? |
| Changes | List of modifications |
| Motivation | Why is this needed? |
| Testing | How did you test? |
| Screenshots | Visual changes (if applicable) |
| Breaking Changes | Any compatibility issues? |

**Example PR Description:**

```markdown
## Summary
Adds Ruby as a new target language for the SUB compiler.

## Changes
- Added LANG_RUBY enum and language info
- Implemented codegen_ruby() with ~170 lines
- Updated README.md, MULTILANG_GUIDE.md, LANGUAGE_SPEC.md
- Added 8 test files for edge cases
- Fixed pre-existing bug in Swift codegen

## Motivation
Ruby's `end` keyword naturally aligns with SUB's `#end` syntax.

## Testing
- [x] `make clean && make` - builds without warnings
- [x] `./sublang example.sb ruby` - generates valid Ruby
- [x] `ruby -c output.rb` - syntax check passes
- [x] Regression tests - all other languages work
- [x] Edge case tests pass

## Breaking Changes
None
```

### Step 9: Respond to Review Feedback

| Scenario | Action |
|----------|--------|
| Changes requested | Make updates, commit, and push to same branch |
| Questions asked | Reply to comments promptly |
| CI fails | Fix issues and push again |
| Approved | Wait for maintainer to merge |
| Merge conflicts | Rebase on latest main and resolve |

**Handling Merge Conflicts:**

```bash
# Fetch latest changes
git fetch upstream

# Rebase your branch
git rebase upstream/main

# Resolve conflicts in editor
# After resolving:
git add .
git rebase --continue

# Force push to your branch
git push origin feature/your-feature-name --force
```

## What to Contribute

### High Priority

| Category | Examples |
|----------|----------|
| Language Support | Add new target languages (Go, Rust, Kotlin, etc.) |
| Bug Fixes | Fix compiler crashes, incorrect output |
| Documentation | Improve guides, add examples |
| Test Coverage | Add more test cases |

### Medium Priority

| Category | Examples |
|----------|----------|
| Performance | Optimize codegen, reduce memory usage |
| Features | Add new SUB syntax features |
| Tooling | Build IDE plugins, syntax highlighters |
| Examples | Create sample projects |

### Welcome Contributions

| Type | Description |
|------|-------------|
| First-time contributors | Look for "good first issue" label |
| Documentation improvements | Fix typos, clarify explanations |
| Test additions | Add edge case tests |
| Code cleanup | Improve code readability |

## Code Style Guidelines

### C Code Standards

| Rule | Example |
|------|---------|
| Indentation | 4 spaces (no tabs) |
| Braces | Opening brace on same line |
| Naming | `snake_case` for functions/variables |
| Comments | Use `//` for single line, `/* */` for blocks |
| Line length | Max 100 characters |

### Example:

```c
// Good
void generate_code(Language lang, AST* tree) {
    if (lang == LANG_PYTHON) {
        codegen_python(tree);
    }
}

// Bad
void GenerateCode(Language lang,AST* tree){
if(lang==LANG_PYTHON){codegen_python(tree);}}
```

## Getting Help

| Resource | Link |
|----------|------|
| Issues | [Open an issue](https://github.com/subhobhai943/sub-lang/issues) |
| Discussions | [Join discussion](https://github.com/subhobhai943/sub-lang/discussions) |
| Documentation | See README.md |
| Examples | Check `examples/` directory |

## Code of Conduct

1. **Be respectful** - Treat everyone with respect
2. **Be constructive** - Provide helpful feedback
3. **Be patient** - Maintainers review in their free time
4. **Be collaborative** - Work together on solutions

## Recognition

All contributors will be:
- Listed in the project contributors
- Mentioned in release notes for significant contributions
- Given credit in documentation updates

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

Thank you for contributing to SUB Language! 🚀

**Questions?** Open an issue or start a discussion.
