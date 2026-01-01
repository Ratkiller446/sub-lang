# OpenCode GLM 4.7 Prompts for Windows Fixes

This file contains specialized prompts to be used with **OpenCode GLM 4.7** via the shell AI tool. Each instance is designed to solve a specific part of the Windows build failure.

---

### Instance 1: MSVC Attribute Compatibility
**Role**: C Portability Specialist
**Task**: Fix `__attribute__((unused))` errors in MSVC.
**Prompt**:
```text
Context: The SUB Language project is failing to build on Windows using MSVC (cl.exe).
Error: `parser_enhanced.c(56): error C2061: syntax error: identifier '__attribute__'`

Problem: MSVC does not support the GCC-style `__attribute__((unused))` syntax.

Task:
1. Modify `src/include/windows_compat.h` to define a macro that handles attributes cross-platform:
   - If `_MSC_VER` is defined, `#define UNUSED` to be empty or `__pragma(warning(suppress: 4100/4101))`.
   - If `__GNUC__` is defined, `#define UNUSED __attribute__((unused))`.
2. Update `src/core/parser_enhanced.c` to use the new `UNUSED` macro instead of the direct `__attribute__` syntax.
3. Ensure `windows_compat.h` is included in `parser_enhanced.c`.
```

---

### Instance 2: Standard Format Strings for int64_t
**Role**: Systems Programmer
**Task**: Fix `printf` format string warnings/errors in `ir.c`.
**Prompt**:
```text
Context: MSVC (cl.exe) warns about `int64_t` being printed with `%ld` in `src/ir/ir.c`.
Error: `ir.c(539): warning C4477: 'printf' : format string '%ld' requires an argument of type 'long', but variadic argument 1 has type 'int64_t'`

Problem: `int64_t` platform-specific size makes `%ld` unsafe. Standard C99 provides `<inttypes.h>`.

Task:
1. In `src/ir/ir.c`, include `<inttypes.h>`.
2. Replace `printf("...%ld", ...)` calls where the value is `int64_t` with the proper macro: `printf("..." PRIi64, ...)`.
3. Specifically fix lines 539 (CONST_INT) and 565 (CALL args) in `src/ir/ir.c`.
```

---

### Instance 3: Unified Windows Compatibility Header Update
**Role**: Software Architect
**Task**: Clean up `src/include/windows_compat.h` to handle all current portability shims.
**Prompt**:
```text
Context: We need a central place to handle all MSVC vs GCC/Clang differences.
Task:
Update `src/include/windows_compat.h` to include:
1. The `UNUSED` macro shim.
2. Inclusion of `<inttypes.h>` for portable printing of fixed-width integers.
3. Ensuring `strcasecmp` and `strdup` mappings are active for all Windows compilers (MinGW AND MSVC).
4. Providing a `__builtin_expect` shim if used, as MSVC lacks it (define as empty).

Deliverable: The complete code for a robust `windows_compat.h`.
```

---

### Instance 4: CI/CD Workflow Debugging (MSVC Entry Point)
**Role**: Build Engineer
**Task**: Fix the GitHub Actions entry point for the Windows build.
**Prompt**:
```text
Context: The current GitHub Actions log shows the build is trying to compile files from the root directory:
`cl /I. sub_native_compiler.c parser_enhanced.c lexer.c semantic.c ir.c codegen_x64.c /Fe:subc-native-windows.exe`

Problem: The project has been reorganized into a `src/` directory structure. The files are now in `src/compilers/`, `src/core/`, etc.

Task:
1. Update the `.github/workflows/release.yml` file.
2. Find the Windows build step and update the `cl` command to use the correct paths:
   `cl /Isrc/include /Isrc/core /Isrc/codegen /Isrc/ir src/compilers/sub_native_compiler.c src/core/parser_enhanced.c src/core/lexer.c src/core/semantic.c src/ir/ir.c src/codegen/codegen_x64.c /Fe:subc-native-windows.exe`
3. Ensure all include paths (`/I`) are correctly mapped to the new structure.
```
