/* ========================================
   SUB Language Semantic Analyzer
   File: semantic.c
   ======================================== */

#define _GNU_SOURCE
#include "sub_compiler.h"
#include "windows_compat.h"

// Symbol table entry (local)
typedef struct LocalSymbolEntry {
    char *name;
    char *type;
    int scope_level;
    struct LocalSymbolEntry *next;
} LocalSymbolEntry;

// Symbol table (local)
typedef struct {
    LocalSymbolEntry *head;
    int current_scope;
} LocalSymbolTable;

// Create symbol table
static LocalSymbolTable* create_symbol_table() {
    LocalSymbolTable *table = malloc(sizeof(LocalSymbolTable));
    table->head = NULL;
    table->current_scope = 0;
    return table;
}

// Add symbol to table
static void add_symbol(LocalSymbolTable *table, const char *name, const char *type) {
    LocalSymbolEntry *entry = malloc(sizeof(LocalSymbolEntry));
    entry->name = strdup(name);
    entry->type = strdup(type);
    entry->scope_level = table->current_scope;
    entry->next = table->head;
    table->head = entry;
}

// Lookup symbol in table
static LocalSymbolEntry* lookup_symbol(LocalSymbolTable *table, const char *name) {
    LocalSymbolEntry *current = table->head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Free symbol table
static void free_symbol_table(LocalSymbolTable *table) {
    LocalSymbolEntry *current = table->head;
    while (current) {
        LocalSymbolEntry *next = current->next;
        free(current->name);
        free(current->type);
        free(current);
        current = next;
    }
    free(table);
}

// Analyze AST node
static int analyze_node(ASTNode *node, LocalSymbolTable *table) {
    if (!node) return 1;
    
    switch (node->type) {
        case AST_VAR_DECL:
            if (node->value) {
                if (lookup_symbol(table, node->value)) {
                    fprintf(stderr, "Semantic error: Variable '%s' already declared\n", node->value);
                    return 0;
                }
                add_symbol(table, node->value, "var");
            }
            if (node->right) {
                return analyze_node(node->right, table);
            }
            break;
            
        case AST_IDENTIFIER:
            if (node->value) {
                if (!lookup_symbol(table, node->value)) {
                    fprintf(stderr, "Semantic error: Undefined variable '%s'\n", node->value);
                    return 0;
                }
            }
            break;
            
        case AST_FUNCTION_DECL:
            if (node->value) {
                add_symbol(table, node->value, "function");
            }
            table->current_scope++;
            if (node->left) {
                if (!analyze_node(node->left, table)) return 0;
            }
            table->current_scope--;
            break;
            
        default:
            if (node->left && !analyze_node(node->left, table)) return 0;
            if (node->right && !analyze_node(node->right, table)) return 0;
            if (node->next && !analyze_node(node->next, table)) return 0;
            break;
    }
    
    return 1;
}

// Main semantic analysis function
int semantic_analyze(ASTNode *ast) {
    if (!ast) {
        fprintf(stderr, "Semantic error: NULL AST\n");
        return 0;
    }
    
    LocalSymbolTable *table = create_symbol_table();
    int result = analyze_node(ast, table);
    free_symbol_table(table);
    
    return result;
}
