#!/bin/bash
# Source Code Reorganization Script
# Moves all source files to src/ directory with proper structure

set -e  # Exit on error

echo "=== SUB Language Source Reorganization ==="
echo

# Create src directory structure
echo "[1/5] Creating directory structure..."
mkdir -p src/core
mkdir -p src/codegen
mkdir -p src/ir
mkdir -p src/compilers
mkdir -p src/include
echo "✓ Directories created"

# Batch 1: Core Compiler Components
echo
echo "[2/5] Moving core compiler files..."
git mv lexer.c src/core/ 2>/dev/null || mv lexer.c src/core/
git mv parser.c src/core/ 2>/dev/null || mv parser.c src/core/
git mv parser_enhanced.c src/core/ 2>/dev/null || mv parser_enhanced.c src/core/
git mv semantic.c src/core/ 2>/dev/null || mv semantic.c src/core/
git mv type_system.c src/core/ 2>/dev/null || mv type_system.c src/core/
git mv type_system.h src/core/ 2>/dev/null || mv type_system.h src/core/
git mv error_handler.c src/core/ 2>/dev/null || mv error_handler.c src/core/
git mv error_handler.h src/core/ 2>/dev/null || mv error_handler.h src/core/
git mv utils.c src/core/ 2>/dev/null || mv utils.c src/core/
echo "✓ Core files moved (9 files)"

# Batch 2: Code Generation
echo
echo "[3/5] Moving code generation files..."
git mv codegen.c src/codegen/ 2>/dev/null || mv codegen.c src/codegen/
git mv codegen_cpp.c src/codegen/ 2>/dev/null || mv codegen_cpp.c src/codegen/
git mv codegen_cpp.h src/codegen/ 2>/dev/null || mv codegen_cpp.h src/codegen/
git mv codegen_multilang.c src/codegen/ 2>/dev/null || mv codegen_multilang.c src/codegen/
git mv codegen_native.c src/codegen/ 2>/dev/null || mv codegen_native.c src/codegen/
git mv codegen_native.h src/codegen/ 2>/dev/null || mv codegen_native.h src/codegen/
git mv codegen_rust.c src/codegen/ 2>/dev/null || mv codegen_rust.c src/codegen/
git mv codegen_rust.h src/codegen/ 2>/dev/null || mv codegen_rust.h src/codegen/
git mv codegen_x64.c src/codegen/ 2>/dev/null || mv codegen_x64.c src/codegen/
git mv codegen_x64.h src/codegen/ 2>/dev/null || mv codegen_x64.h src/codegen/
git mv targets.c src/codegen/ 2>/dev/null || mv targets.c src/codegen/
git mv targets.h src/codegen/ 2>/dev/null || mv targets.h src/codegen/
echo "✓ Codegen files moved (12 files)"

# Batch 3: IR and Headers
echo
echo "[4/5] Moving IR and header files..."
git mv ir.c src/ir/ 2>/dev/null || mv ir.c src/ir/
git mv ir.h src/ir/ 2>/dev/null || mv ir.h src/ir/
git mv sub_compiler.h src/include/ 2>/dev/null || mv sub_compiler.h src/include/
git mv windows_compat.h src/include/ 2>/dev/null || mv windows_compat.h src/include/
echo "✓ IR and headers moved (4 files)"

# Batch 4: Main Compilers
echo
echo "[5/5] Moving main compiler files..."
git mv sub.c src/compilers/ 2>/dev/null || mv sub.c src/compilers/
git mv sub_multilang.c src/compilers/ 2>/dev/null || mv sub_multilang.c src/compilers/
git mv sub_native.c src/compilers/ 2>/dev/null || mv sub_native.c src/compilers/
git mv sub_native_compiler.c src/compilers/ 2>/dev/null || mv sub_native_compiler.c src/compilers/
echo "✓ Compiler files moved (4 files)"

echo
echo "=== Reorganization Complete! ==="
echo "Total: 29 files moved to src/"
echo
echo "Next steps:"
echo "  1. Update Makefile paths"
echo "  2. Update CMakeLists.txt"
echo "  3. Update #include paths in source files"
echo "  4. Test compilation: make clean && make"
