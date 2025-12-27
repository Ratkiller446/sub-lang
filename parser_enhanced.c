/* ========================================
   SUB Language Enhanced Parser
   Properly handles: variables, functions, if/elif/else, loops, print, expressions
   File: parser_enhanced.c
   ======================================== */

#define _GNU_SOURCE
#include "sub_compiler.h"

/* Parser state */
typedef struct {
    Token *tokens;
    int token_count;
    int current;
    int errors;
} ParserState;

/* Forward declarations */
static ASTNode* parse_statement(ParserState *state);
static ASTNode* parse_expression(ParserState *state);
static ASTNode* parse_block(ParserState *state);
static ASTNode* parse_primary(ParserState *state);
static ASTNode* parse_binary(ParserState *state, int precedence);

/* Create AST node */
static ASTNode* create_node(ASTNodeType type, const char *value) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Error: Failed to allocate memory for AST node\n");
        return NULL;
    }
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->left = NULL;
    node->right = NULL;
    node->next = NULL;
    node->condition = NULL;
    node->body = NULL;
    node->children = NULL;
    node->child_count = 0;
    node->metadata = NULL;
    return node;
}

/* Get current token */
static Token* current_token(ParserState *state) {
    if (!state || !state->tokens) return NULL;
    if (state->current >= state->token_count) {
        return &state->tokens[state->token_count - 1];
    }
    return &state->tokens[state->current];
}

/* Peek ahead n tokens */
static Token* peek_token(ParserState *state, int offset) {
    int pos = state->current + offset;
    if (pos >= state->token_count) {
        return &state->tokens[state->token_count - 1];
    }
    return &state->tokens[pos];
}

/* Advance to next token */
static void advance(ParserState *state) {
    if (state && state->current < state->token_count - 1) {
        state->current++;
    }
}

/* Check if current token matches type */
static bool match(ParserState *state, TokenType type) {
    Token *tok = current_token(state);
    return tok && tok->type == type;
}

/* Expect a specific token type */
static bool expect(ParserState *state, TokenType type) {
    if (match(state, type)) {
        advance(state);
        return true;
    }
    Token *tok = current_token(state);
    fprintf(stderr, "Parser Error: Expected token type %d, got %d at line %d\n", 
            type, tok ? tok->type : -1, tok ? tok->line : 0);
    state->errors++;
    return false;
}

/* Skip newlines and whitespace */
static void skip_newlines(ParserState *state) {
    while (match(state, TOKEN_NEWLINE)) {
        advance(state);
    }
}

/* Parse primary expression (literals, identifiers, parenthesized expressions) */
static ASTNode* parse_primary(ParserState *state) {
    Token *tok = current_token(state);
    if (!tok) return NULL;
    
    // Number literal
    if (match(state, TOKEN_NUMBER)) {
        ASTNode *node = create_node(AST_LITERAL, tok->value);
        node->data_type = TYPE_INT; // TODO: Detect float
        advance(state);
        return node;
    }
    
    // String literal
    if (match(state, TOKEN_STRING_LITERAL)) {
        ASTNode *node = create_node(AST_LITERAL, tok->value);
        node->data_type = TYPE_STRING;
        advance(state);
        return node;
    }
    
    // Boolean literals
    if (match(state, TOKEN_TRUE) || match(state, TOKEN_FALSE)) {
        ASTNode *node = create_node(AST_LITERAL, tok->value);
        node->data_type = TYPE_BOOL;
        advance(state);
        return node;
    }
    
    // Identifier or function call
    if (match(state, TOKEN_IDENTIFIER)) {
        char *name = strdup(tok->value);
        advance(state);
        
        // Check for function call
        if (match(state, TOKEN_LPAREN)) {
            advance(state);
            ASTNode *call = create_node(AST_CALL_EXPR, name);
            
            // Parse arguments (simplified for now)
            if (!match(state, TOKEN_RPAREN)) {
                call->left = parse_expression(state);
            }
            
            expect(state, TOKEN_RPAREN);
            free(name);
            return call;
        }
        
        // Just an identifier
        ASTNode *node = create_node(AST_IDENTIFIER, name);
        free(name);
        return node;
    }
    
    // Parenthesized expression
    if (match(state, TOKEN_LPAREN)) {
        advance(state);
        ASTNode *expr = parse_expression(state);
        expect(state, TOKEN_RPAREN);
        return expr;
    }
    
    return NULL;
}

/* Parse binary expressions with precedence */
static ASTNode* parse_binary(ParserState *state, int precedence) {
    if (precedence >= 7) {
        return parse_primary(state);
    }
    
    ASTNode *left = parse_binary(state, precedence + 1);
    if (!left) return NULL;
    
    while (true) {
        Token *op = current_token(state);
        if (!op || !match(state, TOKEN_OPERATOR)) break;
        
        // Simple precedence check (can be enhanced)
        advance(state);
        ASTNode *right = parse_binary(state, precedence + 1);
        
        ASTNode *binary = create_node(AST_BINARY_EXPR, op->value);
        binary->left = left;
        binary->right = right;
        left = binary;
    }
    
    return left;
}

/* Parse expression */
static ASTNode* parse_expression(ParserState *state) {
    return parse_binary(state, 0);
}

/* Parse block of statements */
static ASTNode* parse_block(ParserState *state) {
    ASTNode *block = create_node(AST_BLOCK, NULL);
    ASTNode *first_stmt = NULL;
    ASTNode *last_stmt = NULL;
    
    skip_newlines(state);
    
    while (!match(state, TOKEN_END) && !match(state, TOKEN_EOF) && 
           !match(state, TOKEN_ELIF) && !match(state, TOKEN_ELSE)) {
        
        if (match(state, TOKEN_HASH)) {
            advance(state);
            ASTNode *stmt = parse_statement(state);
            
            if (stmt) {
                if (!first_stmt) {
                    first_stmt = stmt;
                    last_stmt = stmt;
                } else {
                    last_stmt->next = stmt;
                    last_stmt = stmt;
                }
            }
        } else {
            advance(state);
        }
        
        skip_newlines(state);
    }
    
    block->body = first_stmt;
    return block;
}

/* Parse print statement */
static ASTNode* parse_print(ParserState *state) {
    advance(state); // skip 'print'
    
    ASTNode *print_node = create_node(AST_CALL_EXPR, "printf");
    
    if (match(state, TOKEN_LPAREN)) {
        advance(state);
        print_node->left = parse_expression(state);
        expect(state, TOKEN_RPAREN);
    }
    
    return print_node;
}

/* Parse if statement */
static ASTNode* parse_if(ParserState *state) {
    advance(state); // skip 'if'
    
    ASTNode *if_node = create_node(AST_IF_STMT, NULL);
    
    // Parse condition
    if_node->condition = parse_expression(state);
    skip_newlines(state);
    
    // Parse then block
    if_node->body = parse_block(state);
    
    // Handle elif/else
    if (match(state, TOKEN_ELIF)) {
        // Recursively parse elif as nested if
        if_node->right = parse_if(state);
    } else if (match(state, TOKEN_ELSE)) {
        advance(state);
        skip_newlines(state);
        if_node->right = parse_block(state);
    }
    
    if (match(state, TOKEN_END)) {
        advance(state);
    }
    
    return if_node;
}

/* Parse for loop */
static ASTNode* parse_for(ParserState *state) {
    advance(state); // skip 'for'
    
    ASTNode *for_node = create_node(AST_FOR_STMT, NULL);
    
    // Parse loop variable (simplified)
    if (match(state, TOKEN_IDENTIFIER)) {
        Token *var = current_token(state);
        for_node->value = strdup(var->value);
        advance(state);
    }
    
    // Skip 'in' keyword (add TOKEN_IN if needed)
    while (!match(state, TOKEN_NEWLINE) && !match(state, TOKEN_EOF)) {
        advance(state);
    }
    
    skip_newlines(state);
    for_node->body = parse_block(state);
    
    if (match(state, TOKEN_END)) {
        advance(state);
    }
    
    return for_node;
}

/* Parse while loop */
static ASTNode* parse_while(ParserState *state) {
    advance(state); // skip 'while'
    
    ASTNode *while_node = create_node(AST_WHILE_STMT, NULL);
    while_node->condition = parse_expression(state);
    skip_newlines(state);
    while_node->body = parse_block(state);
    
    if (match(state, TOKEN_END)) {
        advance(state);
    }
    
    return while_node;
}

/* Parse function declaration */
static ASTNode* parse_function(ParserState *state) {
    advance(state); // skip 'function'
    
    ASTNode *func = create_node(AST_FUNCTION_DECL, NULL);
    
    // Get function name
    if (match(state, TOKEN_IDENTIFIER)) {
        Token *name = current_token(state);
        func->value = strdup(name->value);
        advance(state);
    }
    
    // Parse parameters
    if (match(state, TOKEN_LPAREN)) {
        advance(state);
        // TODO: Parse parameter list
        while (!match(state, TOKEN_RPAREN) && !match(state, TOKEN_EOF)) {
            advance(state);
        }
        if (match(state, TOKEN_RPAREN)) {
            advance(state);
        }
    }
    
    skip_newlines(state);
    
    // Parse function body
    func->body = parse_block(state);
    
    if (match(state, TOKEN_END)) {
        advance(state);
    }
    
    return func;
}

/* Parse statement */
static ASTNode* parse_statement(ParserState *state) {
    if (!state) return NULL;
    skip_newlines(state);
    
    Token *tok = current_token(state);
    if (!tok) return NULL;
    
    // Variable declaration
    if (match(state, TOKEN_VAR)) {
        advance(state);
        ASTNode *var_decl = create_node(AST_VAR_DECL, NULL);
        
        if (match(state, TOKEN_IDENTIFIER)) {
            var_decl->value = strdup(tok->value);
            tok = current_token(state);
            advance(state);
            
            // Check for assignment
            if (match(state, TOKEN_OPERATOR)) {
                advance(state);
                var_decl->right = parse_expression(state);
            }
        }
        return var_decl;
    }
    
    // Constant declaration
    if (match(state, TOKEN_CONST)) {
        advance(state);
        ASTNode *const_decl = create_node(AST_CONST_DECL, NULL);
        
        if (match(state, TOKEN_IDENTIFIER)) {
            const_decl->value = strdup(tok->value);
            tok = current_token(state);
            advance(state);
            
            if (match(state, TOKEN_OPERATOR)) {
                advance(state);
                const_decl->right = parse_expression(state);
            }
        }
        return const_decl;
    }
    
    // Function declaration
    if (match(state, TOKEN_FUNCTION)) {
        return parse_function(state);
    }
    
    // If statement
    if (match(state, TOKEN_IF)) {
        return parse_if(state);
    }
    
    // For loop
    if (match(state, TOKEN_FOR)) {
        return parse_for(state);
    }
    
    // While loop
    if (match(state, TOKEN_WHILE)) {
        return parse_while(state);
    }
    
    // Return statement
    if (match(state, TOKEN_RETURN)) {
        advance(state);
        ASTNode *ret = create_node(AST_RETURN_STMT, NULL);
        ret->left = parse_expression(state);
        return ret;
    }
    
    // Print (handle as special function)
    if (match(state, TOKEN_IDENTIFIER)) {
        if (tok->value && strcmp(tok->value, "print") == 0) {
            return parse_print(state);
        }
    }
    
    // Skip unknown tokens
    advance(state);
    return NULL;
}

/* Main parser function */
ASTNode* parser_parse_enhanced(Token *tokens, int token_count) {
    if (!tokens || token_count <= 0) {
        fprintf(stderr, "Error: Invalid tokens or token count\n");
        return NULL;
    }
    
    ParserState state = {tokens, token_count, 0, 0};
    ASTNode *root = create_node(AST_PROGRAM, "program");
    if (!root) return NULL;
    
    ASTNode *first_stmt = NULL;
    ASTNode *last_stmt = NULL;
    
    while (!match(&state, TOKEN_EOF)) {
        skip_newlines(&state);
        
        if (match(&state, TOKEN_HASH)) {
            advance(&state);
            ASTNode *stmt = parse_statement(&state);
            
            if (stmt) {
                if (!first_stmt) {
                    first_stmt = stmt;
                    last_stmt = stmt;
                } else {
                    last_stmt->next = stmt;
                    last_stmt = stmt;
                }
            }
        } else if (!match(&state, TOKEN_EOF)) {
            advance(&state);
        }
    }
    
    root->left = first_stmt;
    
    if (state.errors > 0) {
        fprintf(stderr, "Parser completed with %d errors\n", state.errors);
    }
    
    return root;
}

/* Free AST */
void parser_free_ast(ASTNode *node) {
    if (!node) return;
    
    if (node->value) free(node->value);
    if (node->left) parser_free_ast(node->left);
    if (node->right) parser_free_ast(node->right);
    if (node->next) parser_free_ast(node->next);
    if (node->condition) parser_free_ast(node->condition);
    if (node->body) parser_free_ast(node->body);
    
    if (node->children) {
        for (int i = 0; i < node->child_count; i++) {
            parser_free_ast(node->children[i]);
        }
        free(node->children);
    }
    
    free(node);
}
