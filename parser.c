/* ========================================
   SUB Language Parser Implementation
   File: parser.c
   ======================================== */

#include "sub_compiler.h"

// Parser state
typedef struct {
    Token *tokens;
    int token_count;
    int current;
} ParserState;

// Forward declarations
static ASTNode* parse_statement(ParserState *state);
static ASTNode* parse_expression(ParserState *state);

// Create AST node
static ASTNode* create_node(ASTNodeType type, const char *value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->left = NULL;
    node->right = NULL;
    node->next = NULL;
    node->children = NULL;
    node->child_count = 0;
    node->metadata = NULL;
    return node;
}

// Get current token
static Token* current_token(ParserState *state) {
    if (state->current >= state->token_count) {
        return &state->tokens[state->token_count - 1];
    }
    return &state->tokens[state->current];
}

// Advance to next token
static void advance(ParserState *state) {
    if (state->current < state->token_count - 1) {
        state->current++;
    }
}

// Check if current token matches type
static int match(ParserState *state, TokenType type) {
    return current_token(state)->type == type;
}

// Skip newlines
static void skip_newlines(ParserState *state) {
    while (match(state, TOKEN_NEWLINE)) {
        advance(state);
    }
}

// Parse expression (simplified)
static ASTNode* parse_expression(ParserState *state) {
    if (match(state, TOKEN_NUMBER)) {
        ASTNode *node = create_node(AST_LITERAL, current_token(state)->value);
        advance(state);
        return node;
    }
    
    if (match(state, TOKEN_STRING)) {
        ASTNode *node = create_node(AST_LITERAL, current_token(state)->value);
        advance(state);
        return node;
    }
    
    if (match(state, TOKEN_IDENTIFIER)) {
        ASTNode *node = create_node(AST_IDENTIFIER, current_token(state)->value);
        advance(state);
        return node;
    }
    
    return NULL;
}

// Parse statement
static ASTNode* parse_statement(ParserState *state) {
    skip_newlines(state);
    
    if (match(state, TOKEN_VAR)) {
        advance(state);
        ASTNode *node = create_node(AST_VAR_DECL, NULL);
        if (match(state, TOKEN_IDENTIFIER)) {
            node->value = strdup(current_token(state)->value);
            advance(state);
        }
        if (match(state, TOKEN_OPERATOR)) {
            advance(state);
            node->right = parse_expression(state);
        }
        return node;
    }
    
    if (match(state, TOKEN_FUNCTION)) {
        advance(state);
        ASTNode *node = create_node(AST_FUNCTION_DECL, NULL);
        if (match(state, TOKEN_IDENTIFIER)) {
            node->value = strdup(current_token(state)->value);
            advance(state);
        }
        return node;
    }
    
    if (match(state, TOKEN_RETURN)) {
        advance(state);
        ASTNode *node = create_node(AST_RETURN_STMT, NULL);
        node->left = parse_expression(state);
        return node;
    }
    
    return NULL;
}

// Main parser function
ASTNode* parser_parse(Token *tokens, int token_count) {
    ParserState state = {tokens, token_count, 0};
    ASTNode *root = create_node(AST_PROGRAM, "program");
    
    while (!match(&state, TOKEN_EOF)) {
        if (match(&state, TOKEN_HASH)) {
            advance(&state);
            ASTNode *stmt = parse_statement(&state);
            if (stmt) {
                if (!root->left) {
                    root->left = stmt;
                } else {
                    ASTNode *current = root->left;
                    while (current->next) current = current->next;
                    current->next = stmt;
                }
            }
        } else {
            advance(&state);
        }
    }
    
    return root;
}

// Free AST
void parser_free_ast(ASTNode *node) {
    if (!node) return;
    
    if (node->value) free(node->value);
    if (node->left) parser_free_ast(node->left);
    if (node->right) parser_free_ast(node->right);
    if (node->next) parser_free_ast(node->next);
    
    if (node->children) {
        for (int i = 0; i < node->child_count; i++) {
            parser_free_ast(node->children[i]);
        }
        free(node->children);
    }
    
    free(node);
}
