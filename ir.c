/* ========================================
   SUB Language - IR Implementation
   Converts AST to intermediate representation
   File: ir.c
   ======================================== */

#define _GNU_SOURCE
#include "ir.h"
#include "sub_compiler.h"
#include "windows_compat.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Create IR module */
IRModule* ir_module_create(void) {
    IRModule *module = calloc(1, sizeof(IRModule));
    module->entry_point = strdup("main");
    return module;
}

/* Free IR module */
void ir_module_free(IRModule *module) {
    if (!module) return;
    
    // Free functions
    IRFunction *func = module->functions;
    while (func) {
        IRFunction *next = func->next;
        
        // Free instructions
        IRInstruction *instr = func->instructions;
        while (instr) {
            IRInstruction *next_instr = instr->next;
            if (instr->dest) free(instr->dest);
            if (instr->src1) free(instr->src1);
            if (instr->src2) free(instr->src2);
            free(instr);
            instr = next_instr;
        }
        
        free(func->name);
        free(func);
        func = next;
    }
    
    // Free string literals
    for (int i = 0; i < module->string_count; i++) {
        free(module->string_literals[i]);
    }
    free(module->string_literals);
    free(module->entry_point);
    free(module);
}

/* Create IR function */
IRFunction* ir_function_create(const char *name, IRType return_type) {
    IRFunction *func = calloc(1, sizeof(IRFunction));
    func->name = strdup(name);
    func->return_type = return_type;
    func->params = NULL;
    func->param_count = 0;
    func->instructions = NULL;
    func->local_count = 0;
    func->reg_count = 0;
    return func;
}

/* Add parameter to function */
void ir_function_add_param(IRFunction *func, IRValue *param) {
    func->params = realloc(func->params, sizeof(IRValue*) * (func->param_count + 1));
    func->params[func->param_count++] = param;
}

/* Add instruction to function */
void ir_function_add_instruction(IRFunction *func, IRInstruction *instr) {
    if (!func->instructions) {
        func->instructions = instr;
    } else {
        IRInstruction *last = func->instructions;
        while (last->next) {
            last = last->next;
        }
        last->next = instr;
    }
}

/* Create IR instruction */
IRInstruction* ir_instruction_create(IROpcode opcode) {
    IRInstruction *instr = calloc(1, sizeof(IRInstruction));
    instr->opcode = opcode;
    return instr;
}

/* Create IR values */
IRValue* ir_value_create_int(int64_t value) {
    IRValue *val = calloc(1, sizeof(IRValue));
    val->type = IR_TYPE_INT;
    val->data.int_val = value;
    return val;
}

IRValue* ir_value_create_float(double value) {
    IRValue *val = calloc(1, sizeof(IRValue));
    val->type = IR_TYPE_FLOAT;
    val->data.float_val = value;
    return val;
}

IRValue* ir_value_create_string(const char *value) {
    IRValue *val = calloc(1, sizeof(IRValue));
    val->type = IR_TYPE_STRING;
    val->data.string_val = strdup(value);
    return val;
}

IRValue* ir_value_create_reg(int reg_num, IRType type) {
    IRValue *val = calloc(1, sizeof(IRValue));
    val->type = type;
    val->data.reg_num = reg_num;
    return val;
}

IRValue* ir_value_create_label(const char *label) {
    IRValue *val = calloc(1, sizeof(IRValue));
    val->type = IR_TYPE_LABEL;
    val->data.label = strdup(label);
    return val;
}

/* Convert AST to IR */
static void ir_generate_from_ast_node(IRFunction *func, ASTNode *node);

IRModule* ir_generate_from_ast(void *ast_root) {
    if (!ast_root) return NULL;
    
    IRModule *module = ir_module_create();
    
    // Create main function
    IRFunction *main_func = ir_function_create("main", IR_TYPE_INT);
    
    // Generate IR from AST
    ASTNode *root = (ASTNode*)ast_root;
    ir_generate_from_ast_node(main_func, root);
    
    // Add return 0 if not present
    IRInstruction *ret_instr = ir_instruction_create(IR_RETURN);
    ret_instr->src1 = ir_value_create_int(0);
    ret_instr->comment = strdup("main");
    ir_function_add_instruction(main_func, ret_instr);
    
    module->functions = main_func;
    
    return module;
}

/* Generate IR from AST node (recursive) */
static void ir_generate_from_ast_node(IRFunction *func, ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case AST_PROGRAM:
            // Process all children
            for (int i = 0; i < node->child_count; i++) {
                ir_generate_from_ast_node(func, node->children[i]);
            }
            break;
            
        case AST_VAR_DECL: {
            // Variable declaration: allocate space
            IRInstruction *alloc = ir_instruction_create(IR_ALLOC);
            alloc->dest = ir_value_create_reg(func->local_count++, IR_TYPE_INT);
            alloc->dest->name = node->value ? strdup(node->value) : NULL;
            ir_function_add_instruction(func, alloc);
            
            // If there's an initializer, store it
            if (node->child_count > 0) {
                ir_generate_from_ast_node(func, node->children[0]);
                
                IRInstruction *store = ir_instruction_create(IR_STORE);
                store->dest = alloc->dest;
                ir_function_add_instruction(func, store);
            }
            break;
        }
            
        case AST_CALL_EXPR:
            if (node->value && strcmp(node->value, "print") == 0) {
                // Generate code for arguments
                for (int i = 0; i < node->child_count; i++) {
                    ir_generate_from_ast_node(func, node->children[i]);
                }
                
                IRInstruction *print = ir_instruction_create(IR_PRINT);
                ir_function_add_instruction(func, print);
            }
            break;
            
        case AST_BINARY_EXPR: {
            // Generate left and right operands
            if (node->left) ir_generate_from_ast_node(func, node->left);
            if (node->right) ir_generate_from_ast_node(func, node->right);
            
            // Determine operation
            IROpcode op = IR_ADD;
            if (node->value) {
                if (strcmp(node->value, "+") == 0) op = IR_ADD;
                else if (strcmp(node->value, "-") == 0) op = IR_SUB;
                else if (strcmp(node->value, "*") == 0) op = IR_MUL;
                else if (strcmp(node->value, "/") == 0) op = IR_DIV;
            }
            
            IRInstruction *bin_op = ir_instruction_create(op);
            bin_op->dest = ir_value_create_reg(func->reg_count++, IR_TYPE_INT);
            ir_function_add_instruction(func, bin_op);
            break;
        }
            
        case AST_LITERAL: {
            // Load constant
            IRInstruction *load_const = ir_instruction_create(IR_CONST_INT);
            load_const->dest = ir_value_create_reg(func->reg_count++, IR_TYPE_INT);
            if (node->value) {
                load_const->src1 = ir_value_create_int(atoi(node->value));
            } else {
                load_const->src1 = ir_value_create_int(0);
            }
            ir_function_add_instruction(func, load_const);
            break;
        }
            
        default:
            // Recurse on children
            for (int i = 0; i < node->child_count; i++) {
                ir_generate_from_ast_node(func, node->children[i]);
            }
            break;
    }
}

/* Optimize IR (placeholder) */
void ir_optimize(IRModule *module) {
    // TODO: Implement optimizations
    // - Dead code elimination
    // - Constant folding
    // - Common subexpression elimination
    (void)module;
}

/* Print IR for debugging */
void ir_print(IRModule *module) {
    if (!module) return;
    
    printf("\n=== IR Module ===\n");
    printf("Entry point: %s\n\n", module->entry_point);
    
    IRFunction *func = module->functions;
    while (func) {
        printf("Function: %s\n", func->name);
        printf("  Locals: %d\n", func->local_count);
        printf("  Registers: %d\n", func->reg_count);
        printf("  Instructions:\n");
        
        IRInstruction *instr = func->instructions;
        while (instr) {
            printf("    ");
            switch (instr->opcode) {
                case IR_ADD: printf("ADD"); break;
                case IR_SUB: printf("SUB"); break;
                case IR_MUL: printf("MUL"); break;
                case IR_DIV: printf("DIV"); break;
                case IR_CONST_INT: 
                    printf("CONST_INT %ld", instr->src1->data.int_val); 
                    break;
                case IR_PRINT: printf("PRINT"); break;
                case IR_RETURN: printf("RETURN"); break;
                default: printf("UNKNOWN"); break;
            }
            printf("\n");
            instr = instr->next;
        }
        
        func = func->next;
    }
}
