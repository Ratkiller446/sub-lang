/* ========================================
   SUB Language Compiler - Main Components
   Implemented in C for performance
   File: sub_compiler.h
   ======================================== */

#ifndef SUB_COMPILER_H
#define SUB_COMPILER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Token Types for Lexical Analysis */
typedef enum {
    TOKEN_HASH,           // #
    TOKEN_VAR,            // var
    TOKEN_FUNCTION,       // function
    TOKEN_IF,             // if
    TOKEN_ELIF,           // elif
    TOKEN_ELSE,           // else
    TOKEN_FOR,            // for
    TOKEN_WHILE,          // while
    TOKEN_RETURN,         // return
    TOKEN_END,            // end
    TOKEN_EMBED,          // embed
    TOKEN_ENDEMBED,       // endembed
    TOKEN_UI,             // ui
    TOKEN_IDENTIFIER,     // variable names
    TOKEN_NUMBER,         // numeric literals
    TOKEN_STRING,         // string literals
    TOKEN_OPERATOR,       // +, -, *, /, =, etc.
    TOKEN_LPAREN,         // (
    TOKEN_RPAREN,         // )
    TOKEN_LBRACE,         // {
    TOKEN_RBRACE,         // }
    TOKEN_DOT,            // .
    TOKEN_COMMA,          // ,
    TOKEN_NEWLINE,        // \n
    TOKEN_EOF             // End of file
} TokenType;

/* Token Structure */
typedef struct {
    TokenType type;
    char *value;
    int line;
    int column;
} Token;

/* Abstract Syntax Tree Node Types */
typedef enum {
    AST_PROGRAM,
    AST_VAR_DECL,
    AST_FUNCTION_DECL,
    AST_IF_STMT,
    AST_FOR_STMT,
    AST_WHILE_STMT,
    AST_RETURN_STMT,
    AST_ASSIGN_STMT,
    AST_CALL_EXPR,
    AST_BINARY_EXPR,
    AST_IDENTIFIER,
    AST_LITERAL,
    AST_BLOCK,
    AST_UI_COMPONENT,
    AST_EMBED_CODE
} ASTNodeType;

/* AST Node Structure */
typedef struct ASTNode {
    ASTNodeType type;
    char *value;
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *next;
    struct ASTNode **children;
    int child_count;
    void *metadata;
} ASTNode;

/* Target Platform Enum */
typedef enum {
    PLATFORM_ANDROID,
    PLATFORM_IOS,
    PLATFORM_WINDOWS,
    PLATFORM_MACOS,
    PLATFORM_LINUX,
    PLATFORM_WEB
} Platform;

/* Compiler Context */
typedef struct {
    Token *tokens;
    int token_count;
    int current_token;
    ASTNode *ast;
    Platform target_platform;
    char *output_path;
} CompilerContext;

/* Function Declarations */

// Lexical Analysis
Token* lexer_tokenize(const char *source, int *token_count);
void lexer_free_tokens(Token *tokens, int count);

// Parser
ASTNode* parser_parse(Token *tokens, int token_count);
void parser_free_ast(ASTNode *node);

// Semantic Analysis
int semantic_analyze(ASTNode *ast);

// Code Generation
char* codegen_generate(ASTNode *ast, Platform platform);

// Utility Functions
char* read_file(const char *filename);
void write_file(const char *filename, const char *content);
void compile_error(const char *message, int line);

#endif /* SUB_COMPILER_H */
