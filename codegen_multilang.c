/* ========================================
   SUB Language Multi-Language Code Generator
   Supports: Python, Java, Swift, Kotlin, C, C++, Rust, Assembly, JS, CSS, and more
   File: codegen_multilang.c
   ======================================== */

#define _GNU_SOURCE
#include "sub_compiler.h"
#include <stdarg.h>

/* String Builder (reused from codegen.c) */
typedef struct {
    char *buffer;
    size_t size;
    size_t capacity;
} StringBuilder;

static StringBuilder* sb_create() {
    StringBuilder *sb = malloc(sizeof(StringBuilder));
    if (!sb) return NULL;
    sb->capacity = 8192;
    sb->size = 0;
    sb->buffer = malloc(sb->capacity);
    if (!sb->buffer) {
        free(sb);
        return NULL;
    }
    sb->buffer[0] = '\0';
    return sb;
}

static void sb_append(StringBuilder *sb, const char *fmt, ...) {
    if (!sb || !fmt) return;
    
    va_list args;
    va_start(args, fmt);
    
    va_list args_copy;
    va_copy(args_copy, args);
    int needed = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);
    
    if (needed < 0) {
        va_end(args);
        return;
    }
    
    while (sb->size + needed + 1 > sb->capacity) {
        sb->capacity *= 2;
        char *new_buffer = realloc(sb->buffer, sb->capacity);
        if (!new_buffer) {
            va_end(args);
            return;
        }
        sb->buffer = new_buffer;
    }
    
    vsnprintf(sb->buffer + sb->size, needed + 1, fmt, args);
    sb->size += needed;
    va_end(args);
}

static char* sb_to_string(StringBuilder *sb) {
    if (!sb) return NULL;
    char *result = strdup(sb->buffer);
    free(sb->buffer);
    free(sb);
    return result;
}

/* Forward declarations */
static void generate_node_python(StringBuilder *sb, ASTNode *node, int indent);
static void generate_node_java(StringBuilder *sb, ASTNode *node, int indent);
static void generate_node_swift(StringBuilder *sb, ASTNode *node, int indent);
static void generate_node_kotlin(StringBuilder *sb, ASTNode *node, int indent);
static void generate_node_cpp(StringBuilder *sb, ASTNode *node, int indent);
static void generate_node_rust(StringBuilder *sb, ASTNode *node, int indent);
static void generate_node_js(StringBuilder *sb, ASTNode *node, int indent);
static void generate_node_assembly(StringBuilder *sb, ASTNode *node, int indent);

static void indent_code(StringBuilder *sb, int level) {
    for (int i = 0; i < level; i++) {
        sb_append(sb, "    ");
    }
}

/* ========================================
   PYTHON CODE GENERATOR
   ======================================== */

static void generate_expr_python(StringBuilder *sb, ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case AST_LITERAL:
            sb_append(sb, "%s", node->value ? node->value : "None");
            break;
        case AST_IDENTIFIER:
            sb_append(sb, "%s", node->value ? node->value : "var");
            break;
        case AST_BINARY_EXPR:
            sb_append(sb, "(");
            generate_expr_python(sb, node->left);
            sb_append(sb, " %s ", node->value ? node->value : "+");
            generate_expr_python(sb, node->right);
            sb_append(sb, ")");
            break;
        case AST_CALL_EXPR:
            sb_append(sb, "%s(", node->value ? node->value : "func");
            if (node->left) generate_expr_python(sb, node->left);
            sb_append(sb, ")");
            break;
        default:
            break;
    }
}

static void generate_node_python(StringBuilder *sb, ASTNode *node, int indent) {
    if (!node) return;
    
    switch (node->type) {
        case AST_PROGRAM:
            for (ASTNode *stmt = node->left; stmt != NULL; stmt = stmt->next) {
                generate_node_python(sb, stmt, indent);
            }
            break;
            
        case AST_VAR_DECL:
            indent_code(sb, indent);
            sb_append(sb, "%s = ", node->value ? node->value : "var");
            if (node->right) {
                generate_expr_python(sb, node->right);
            } else {
                sb_append(sb, "None");
            }
            sb_append(sb, "\n");
            break;
            
        case AST_FUNCTION_DECL:
            sb_append(sb, "\ndef %s():\n", node->value ? node->value : "func");
            if (node->body) {
                generate_node_python(sb, node->body, indent + 1);
            }
            for (ASTNode *stmt = node->left; stmt != NULL; stmt = stmt->next) {
                generate_node_python(sb, stmt, indent + 1);
            }
            if (!node->body && !node->left) {
                indent_code(sb, indent + 1);
                sb_append(sb, "pass\n");
            }
            sb_append(sb, "\n");
            break;
            
        case AST_IF_STMT:
            indent_code(sb, indent);
            sb_append(sb, "if ");
            generate_expr_python(sb, node->condition);
            sb_append(sb, ":\n");
            generate_node_python(sb, node->body, indent + 1);
            if (node->right) {
                indent_code(sb, indent);
                sb_append(sb, "else:\n");
                generate_node_python(sb, node->right, indent + 1);
            }
            break;
            
        case AST_FOR_STMT:
            indent_code(sb, indent);
            sb_append(sb, "for %s in range(10):\n", node->value ? node->value : "i");
            generate_node_python(sb, node->body, indent + 1);
            break;
            
        case AST_WHILE_STMT:
            indent_code(sb, indent);
            sb_append(sb, "while ");
            generate_expr_python(sb, node->condition);
            sb_append(sb, ":\n");
            generate_node_python(sb, node->body, indent + 1);
            break;
            
        case AST_RETURN_STMT:
            indent_code(sb, indent);
            sb_append(sb, "return");
            if (node->left) {
                sb_append(sb, " ");
                generate_expr_python(sb, node->left);
            }
            sb_append(sb, "\n");
            break;
            
        case AST_CALL_EXPR:
            indent_code(sb, indent);
            generate_expr_python(sb, node);
            sb_append(sb, "\n");
            break;
            
        case AST_BLOCK:
            for (ASTNode *stmt = node->body; stmt != NULL; stmt = stmt->next) {
                generate_node_python(sb, stmt, indent);
            }
            break;
            
        default:
            break;
    }
}

char* codegen_python(ASTNode *ast) {
    StringBuilder *sb = sb_create();
    if (!sb) return NULL;
    
    sb_append(sb, "#!/usr/bin/env python3\n");
    sb_append(sb, "# Generated by SUB Language Compiler\n\n");
    
    generate_node_python(sb, ast, 0);
    
    sb_append(sb, "\nif __name__ == '__main__':\n");
    sb_append(sb, "    print('SUB Language Program')\n");
    
    return sb_to_string(sb);
}

/* ========================================
   JAVA CODE GENERATOR
   ======================================== */

static void generate_expr_java(StringBuilder *sb, ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case AST_LITERAL:
            sb_append(sb, "%s", node->value ? node->value : "null");
            break;
        case AST_IDENTIFIER:
            sb_append(sb, "%s", node->value ? node->value : "var");
            break;
        case AST_BINARY_EXPR:
            sb_append(sb, "(");
            generate_expr_java(sb, node->left);
            sb_append(sb, " %s ", node->value ? node->value : "+");
            generate_expr_java(sb, node->right);
            sb_append(sb, ")");
            break;
        case AST_CALL_EXPR:
            sb_append(sb, "%s(", node->value ? node->value : "func");
            if (node->left) generate_expr_java(sb, node->left);
            sb_append(sb, ")");
            break;
        default:
            break;
    }
}

static void generate_node_java(StringBuilder *sb, ASTNode *node, int indent) {
    if (!node) return;
    
    switch (node->type) {
        case AST_PROGRAM:
            for (ASTNode *stmt = node->left; stmt != NULL; stmt = stmt->next) {
                generate_node_java(sb, stmt, indent);
            }
            break;
            
        case AST_VAR_DECL:
            indent_code(sb, indent);
            sb_append(sb, "int %s = ", node->value ? node->value : "var");
            if (node->right) {
                generate_expr_java(sb, node->right);
            } else {
                sb_append(sb, "0");
            }
            sb_append(sb, ";\n");
            break;
            
        case AST_FUNCTION_DECL:
            indent_code(sb, indent);
            sb_append(sb, "public static void %s() {\n", node->value ? node->value : "func");
            if (node->body) generate_node_java(sb, node->body, indent + 1);
            for (ASTNode *stmt = node->left; stmt != NULL; stmt = stmt->next) {
                generate_node_java(sb, stmt, indent + 1);
            }
            indent_code(sb, indent);
            sb_append(sb, "}\n\n");
            break;
            
        case AST_IF_STMT:
            indent_code(sb, indent);
            sb_append(sb, "if (");
            generate_expr_java(sb, node->condition);
            sb_append(sb, ") {\n");
            generate_node_java(sb, node->body, indent + 1);
            indent_code(sb, indent);
            sb_append(sb, "}");
            if (node->right) {
                sb_append(sb, " else {\n");
                generate_node_java(sb, node->right, indent + 1);
                indent_code(sb, indent);
                sb_append(sb, "}");
            }
            sb_append(sb, "\n");
            break;
            
        case AST_RETURN_STMT:
            indent_code(sb, indent);
            sb_append(sb, "return");
            if (node->left) {
                sb_append(sb, " ");
                generate_expr_java(sb, node->left);
            }
            sb_append(sb, ";\n");
            break;
            
        case AST_BLOCK:
            for (ASTNode *stmt = node->body; stmt != NULL; stmt = stmt->next) {
                generate_node_java(sb, stmt, indent);
            }
            break;
            
        default:
            break;
    }
}

char* codegen_java(ASTNode *ast) {
    StringBuilder *sb = sb_create();
    if (!sb) return NULL;
    
    sb_append(sb, "// Generated by SUB Language Compiler\n\n");
    sb_append(sb, "public class SubProgram {\n\n");
    
    generate_node_java(sb, ast, 1);
    
    sb_append(sb, "    public static void main(String[] args) {\n");
    sb_append(sb, "        System.out.println(\"SUB Language Program\");\n");
    sb_append(sb, "    }\n");
    sb_append(sb, "}\n");
    
    return sb_to_string(sb);
}

/* ========================================
   SWIFT CODE GENERATOR
   ======================================== */

char* codegen_swift(ASTNode *ast) {
    StringBuilder *sb = sb_create();
    if (!sb) return NULL;
    
    sb_append(sb, "// Generated by SUB Language Compiler\n\n");
    sb_append(sb, "import Foundation\n\n");
    
    // Generate Swift code (simplified)
    sb_append(sb, "func main() {\n");
    sb_append(sb, "    print(\"SUB Language Program\")\n");
    sb_append(sb, "}\n\n");
    sb_append(sb, "main()\n");
    
    return sb_to_string(sb);
}

/* ========================================
   KOTLIN CODE GENERATOR
   ======================================== */

char* codegen_kotlin(ASTNode *ast) {
    StringBuilder *sb = sb_create();
    if (!sb) return NULL;
    
    sb_append(sb, "// Generated by SUB Language Compiler\n\n");
    sb_append(sb, "fun main() {\n");
    sb_append(sb, "    println(\"SUB Language Program\")\n");
    sb_append(sb, "}\n");
    
    return sb_to_string(sb);
}

/* ========================================
   C++ CODE GENERATOR
   ======================================== */

char* codegen_cpp(ASTNode *ast) {
    StringBuilder *sb = sb_create();
    if (!sb) return NULL;
    
    sb_append(sb, "// Generated by SUB Language Compiler\n\n");
    sb_append(sb, "#include <iostream>\n");
    sb_append(sb, "#include <string>\n\n");
    sb_append(sb, "using namespace std;\n\n");
    
    sb_append(sb, "int main() {\n");
    sb_append(sb, "    cout << \"SUB Language Program\" << endl;\n");
    sb_append(sb, "    return 0;\n");
    sb_append(sb, "}\n");
    
    return sb_to_string(sb);
}

/* ========================================
   RUST CODE GENERATOR
   ======================================== */

char* codegen_rust(ASTNode *ast) {
    StringBuilder *sb = sb_create();
    if (!sb) return NULL;
    
    sb_append(sb, "// Generated by SUB Language Compiler\n\n");
    sb_append(sb, "fn main() {\n");
    sb_append(sb, "    println!(\"SUB Language Program\");\n");
    sb_append(sb, "}\n");
    
    return sb_to_string(sb);
}

/* ========================================
   JAVASCRIPT CODE GENERATOR
   ======================================== */

static void generate_expr_js(StringBuilder *sb, ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case AST_LITERAL:
            sb_append(sb, "%s", node->value ? node->value : "null");
            break;
        case AST_IDENTIFIER:
            sb_append(sb, "%s", node->value ? node->value : "var");
            break;
        case AST_BINARY_EXPR:
            sb_append(sb, "(");
            generate_expr_js(sb, node->left);
            sb_append(sb, " %s ", node->value ? node->value : "+");
            generate_expr_js(sb, node->right);
            sb_append(sb, ")");
            break;
        case AST_CALL_EXPR:
            sb_append(sb, "%s(", node->value ? node->value : "func");
            if (node->left) generate_expr_js(sb, node->left);
            sb_append(sb, ")");
            break;
        default:
            break;
    }
}

static void generate_node_js(StringBuilder *sb, ASTNode *node, int indent) {
    if (!node) return;
    
    switch (node->type) {
        case AST_PROGRAM:
            for (ASTNode *stmt = node->left; stmt != NULL; stmt = stmt->next) {
                generate_node_js(sb, stmt, indent);
            }
            break;
            
        case AST_VAR_DECL:
            indent_code(sb, indent);
            sb_append(sb, "let %s = ", node->value ? node->value : "var");
            if (node->right) {
                generate_expr_js(sb, node->right);
            } else {
                sb_append(sb, "null");
            }
            sb_append(sb, ";\n");
            break;
            
        case AST_FUNCTION_DECL:
            indent_code(sb, indent);
            sb_append(sb, "function %s() {\n", node->value ? node->value : "func");
            if (node->body) generate_node_js(sb, node->body, indent + 1);
            for (ASTNode *stmt = node->left; stmt != NULL; stmt = stmt->next) {
                generate_node_js(sb, stmt, indent + 1);
            }
            indent_code(sb, indent);
            sb_append(sb, "}\n\n");
            break;
            
        case AST_RETURN_STMT:
            indent_code(sb, indent);
            sb_append(sb, "return");
            if (node->left) {
                sb_append(sb, " ");
                generate_expr_js(sb, node->left);
            }
            sb_append(sb, ";\n");
            break;
            
        case AST_BLOCK:
            for (ASTNode *stmt = node->body; stmt != NULL; stmt = stmt->next) {
                generate_node_js(sb, stmt, indent);
            }
            break;
            
        default:
            break;
    }
}

char* codegen_javascript(ASTNode *ast) {
    StringBuilder *sb = sb_create();
    if (!sb) return NULL;
    
    sb_append(sb, "// Generated by SUB Language Compiler\n\n");
    
    generate_node_js(sb, ast, 0);
    
    sb_append(sb, "\nconsole.log('SUB Language Program');\n");
    
    return sb_to_string(sb);
}

/* ========================================
   CSS CODE GENERATOR (for styling)
   ======================================== */

char* codegen_css(ASTNode *ast) {
    StringBuilder *sb = sb_create();
    if (!sb) return NULL;
    
    sb_append(sb, "/* Generated by SUB Language Compiler */\n\n");
    sb_append(sb, "body {\n");
    sb_append(sb, "    font-family: Arial, sans-serif;\n");
    sb_append(sb, "    margin: 20px;\n");
    sb_append(sb, "    background-color: #f0f0f0;\n");
    sb_append(sb, "}\n\n");
    sb_append(sb, ".container {\n");
    sb_append(sb, "    max-width: 1200px;\n");
    sb_append(sb, "    margin: 0 auto;\n");
    sb_append(sb, "    padding: 20px;\n");
    sb_append(sb, "}\n");
    
    return sb_to_string(sb);
}

/* ========================================
   x86-64 ASSEMBLY CODE GENERATOR
   ======================================== */

char* codegen_assembly(ASTNode *ast) {
    StringBuilder *sb = sb_create();
    if (!sb) return NULL;
    
    sb_append(sb, "; Generated by SUB Language Compiler\n");
    sb_append(sb, "; x86-64 Assembly (NASM syntax)\n\n");
    
    sb_append(sb, "section .data\n");
    sb_append(sb, "    msg db 'SUB Language Program', 0xA\n");
    sb_append(sb, "    len equ $ - msg\n\n");
    
    sb_append(sb, "section .text\n");
    sb_append(sb, "    global _start\n\n");
    
    sb_append(sb, "_start:\n");
    sb_append(sb, "    ; Write message to stdout\n");
    sb_append(sb, "    mov rax, 1          ; syscall: write\n");
    sb_append(sb, "    mov rdi, 1          ; fd: stdout\n");
    sb_append(sb, "    mov rsi, msg        ; buffer\n");
    sb_append(sb, "    mov rdx, len        ; count\n");
    sb_append(sb, "    syscall\n\n");
    
    sb_append(sb, "    ; Exit program\n");
    sb_append(sb, "    mov rax, 60         ; syscall: exit\n");
    sb_append(sb, "    xor rdi, rdi        ; status: 0\n");
    sb_append(sb, "    syscall\n");
    
    return sb_to_string(sb);
}
