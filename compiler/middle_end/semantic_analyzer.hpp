/**
 * SUB Language Middle-End - C++ Implementation
 * Semantic Analysis and Optimization Engine
 */

#ifndef SUB_SEMANTIC_ANALYZER_HPP
#define SUB_SEMANTIC_ANALYZER_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <optional>
#include <variant>

extern "C" {
    #include "../frontend/frontend_ffi.h"
}

namespace sub {
namespace middle_end {

enum class DataType {
    Unknown,
    Int, Float, String, Bool,
    Array, Object, Function,
    Null, Auto, Void
};

enum class ASTNodeType {
    Program, VarDecl, FunctionDecl,
    IfStmt, ForStmt, WhileStmt,
    BinaryExpr, UnaryExpr, CallExpr,
    Identifier, Literal, Block
};

struct ASTNode {
    ASTNodeType type;
    std::string value;
    DataType data_type;
    int line;
    int column;
    std::vector<std::unique_ptr<ASTNode>> children;
    std::unordered_map<std::string, std::string> attributes;
};

class SymbolTable {
public:
    struct Symbol {
        std::string name;
        DataType type;
        int scope_level;
        bool is_const;
        bool is_initialized;
        int line_declared;
    };

private:
    std::vector<std::unordered_map<std::string, Symbol>> scopes_;
    int current_scope_level_;

public:
    SymbolTable() : current_scope_level_(0) {
        scopes_.push_back({});
    }

    void enter_scope() {
        current_scope_level_++;
        scopes_.push_back({});
    }

    void exit_scope() {
        if (current_scope_level_ > 0) {
            scopes_.pop_back();
            current_scope_level_--;
        }
    }

    bool insert(const std::string& name, DataType type, bool is_const, int line) {
        if (scopes_[current_scope_level_].count(name)) {
            return false; // Already declared in current scope
        }
        scopes_[current_scope_level_][name] = {
            name, type, current_scope_level_, is_const, true, line
        };
        return true;
    }

    std::optional<Symbol> lookup(const std::string& name) const {
        for (int i = current_scope_level_; i >= 0; i--) {
            auto it = scopes_[i].find(name);
            if (it != scopes_[i].end()) {
                return it->second;
            }
        }
        return std::nullopt;
    }

    int current_scope_level() const { return current_scope_level_; }
};

class SemanticAnalyzer {
private:
    SymbolTable symbol_table_;
    std::vector<std::string> errors_;
    std::vector<std::string> warnings_;
    bool has_errors_;

    void report_error(const std::string& message, int line, int column) {
        errors_.push_back("Error [" + std::to_string(line) + ":" + 
                         std::to_string(column) + "]: " + message);
        has_errors_ = true;
    }

    void report_warning(const std::string& message, int line, int column) {
        warnings_.push_back("Warning [" + std::to_string(line) + ":" + 
                           std::to_string(column) + "]: " + message);
    }

    DataType infer_type(const ASTNode* node);
    bool check_type_compatibility(DataType expected, DataType actual);
    void analyze_node(ASTNode* node);
    void analyze_var_decl(ASTNode* node);
    void analyze_function_decl(ASTNode* node);
    void analyze_binary_expr(ASTNode* node);
    void analyze_call_expr(ASTNode* node);

public:
    SemanticAnalyzer() : has_errors_(false) {}

    bool analyze(ASTNode* root) {
        has_errors_ = false;
        errors_.clear();
        warnings_.clear();
        analyze_node(root);
        return !has_errors_;
    }

    const std::vector<std::string>& get_errors() const { return errors_; }
    const std::vector<std::string>& get_warnings() const { return warnings_; }
    bool has_errors() const { return has_errors_; }
};

template<typename T>
class OptimizationPass {
public:
    virtual ~OptimizationPass() = default;
    virtual void run(ASTNode* root, T& context) = 0;
    virtual const char* name() const = 0;
};

class ConstantFoldingPass : public OptimizationPass<int> {
public:
    void run(ASTNode* root, int& level) override;
    const char* name() const override { return "Constant Folding"; }
};

class DeadCodeEliminationPass : public OptimizationPass<int> {
public:
    void run(ASTNode* root, int& level) override;
    const char* name() const override { return "Dead Code Elimination"; }
};

class InlineExpansionPass : public OptimizationPass<int> {
public:
    void run(ASTNode* root, int& level) override;
    const char* name() const override { return "Inline Expansion"; }
};

class Optimizer {
private:
    std::vector<std::unique_ptr<OptimizationPass<int>>> passes_;
    int optimization_level_;

public:
    explicit Optimizer(int level) : optimization_level_(level) {
        if (level >= 1) {
            passes_.push_back(std::make_unique<ConstantFoldingPass>());
        }
        if (level >= 2) {
            passes_.push_back(std::make_unique<DeadCodeEliminationPass>());
        }
        if (level >= 3) {
            passes_.push_back(std::make_unique<InlineExpansionPass>());
        }
    }

    void optimize(ASTNode* root) {
        for (auto& pass : passes_) {
            pass->run(root, optimization_level_);
        }
    }
};

} // namespace middle_end
} // namespace sub

extern "C" {
    void* sub_middle_end_create_analyzer();
    int sub_middle_end_analyze(void* analyzer, void* ast_root);
    void sub_middle_end_optimize(void* ast_root, int level);
    void sub_middle_end_free_analyzer(void* analyzer);
    const char* sub_middle_end_get_errors(void* analyzer);
}

#endif // SUB_SEMANTIC_ANALYZER_HPP
