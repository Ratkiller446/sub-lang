/* ========================================
   SUB Language Semantic Analyzer
   File: semantic.c
   ======================================== */

#include "sub_compiler.h"

// Symbol table entry
typedef struct SymbolEntry {
    char *name;
    char *type;
    int scope_level;
    struct SymbolEntry *next;
} SymbolEntry;

// Symbol table
typedef struct {
    SymbolEntry *head;
    int current_scope;
} SymbolTable;

// Create symbol table
static SymbolTable* create_symbol_table() {
    SymbolTable *table = malloc(sizeof(SymbolTable));
    table->head = NULL;
    table->current_scope = 0;
    return table;
}

// Add symbol to table
static void add_symbol(SymbolTable *table, const char *name, const char *type) {
    SymbolEntry *entry = malloc(sizeof(SymbolEntry));
    entry->name = strdup(name);
    entry->type = strdup(type);
    entry->scope_level = table->current_scope;
    entry->next = table->head;
    table->head = entry;
}

// Lookup symbol in table
static SymbolEntry* lookup_symbol(SymbolTable *table, const char *name) {
    SymbolEntry *current = table->head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Free symbol table
static void free_symbol_table(SymbolTable *table) {
    SymbolEntry *current = table->head;
    while (current) {
        SymbolEntry *next = current->next;
        free(current->name);
        free(current->type);
        free(current);
        current = next;
    }
    free(table);
}

// Analyze AST node
static int analyze_node(ASTNode *node, SymbolTable *table) {
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
    
    SymbolTable *table = create_symbol_table();
    int result = analyze_node(ast, table);
    free_symbol_table(table);
    
    return result;
}
