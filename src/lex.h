// Lexer for Polyphen

// The layout of the lexer is inspired by MinHs
// Which is a minimal implementation of Haskell using Haskell
// A project of the Comp3161 deputy which I attended and worked through

// Hence the pattern-matching-like expressions below
// This language will not, in the near future, support FP paradigm
// OOP is planned further up as an extension to Polyphen
// Barebone, Polyphen will be a imperative programming language

// ===========================================================

// any of the three major c++ compiler can identify variable errors
// at parse level.

// so it might be better to have a variable environment for the parser
// already.

#ifndef _LEX
#define _LEX

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <sstream>
#include <optional>
#include <variant>
#include <functional>
#include <algorithm>
#include <any>
#include <unordered_map>
#include <unordered_set>

// compiler features that is supported
const bool IMPLICIT_TYPE = false;
const bool OOP = false;
const bool FP = false;


enum class KeywordType {
};


enum class ExpType {
    _var,
    _op,
    _con,
    _int,               // future support for `0b`, `0x`
    _float,
    _str,
    _bool,
    _assign,
    _app,             // for FP
    _let,               // for FP
    _fun,               // for FP
    _case,              // for FP
    _map,
    _tdef,              // function type definition
    _sdef,              // function logic definition
    _main,              // maybe redundant
    _return,
    _input,
    _for,
    _while,
    _if,
    _elif,
    _else,
    _construct,         // for FP
    _semi,              // redundant
    _lpar,
    _rpar,              // maybe redundant
    _lbra,
    _rbra,              // maybe redundant

    // arithmetic operator
    _add,
    _incr,              // `++` or `+=`
    _sub,
    _decr,              // `--` or `-=`
    _mul,
    _div,               // floating point
    _exp,
    _quot,
    _rem,

    // numeric operator
    _neg,

    // logical operator
    _gt,
    _ge,
    _lt,
    _le,
    _eq,
    _ne,
    _and,
    _or,
    _not,
    _xor,
    // _tern,
    // _or,

    // (a + b)
    // type unification (for FP)
    _fst,
    _snd,

    // (a, b)
    // type product (pair)
    _inl,
    _inr,


    // Not coming soon...

    // idk maybe give this a symbol of `==>`
    _recapp,            // equivalent to Haskell `map`

    // `@>`
    _foldl,             // foldl

    // `>@`
    _foldr,             // foldr
    

};

enum class GroundValueType {
    // _unit,
    _char,
    _bool,
    _int,
    _float,
};



struct Token {
    ExpType ty;
    size_t row = 0, col = 0;
    std::optional<std::string> val {};
};

struct NodeExpr;

struct NodeIdent {
    Token ident;
};

struct NodeIntLit {
    Token int_lit;
};

// =================================
// ======== Recursive Nodes ========
// =================================

// ======== Arithmetic Nodes =========

struct NodeParen {
    NodeExpr *l, *r;
};

struct NodeArithExpr;

struct NodeArithNeg {
    NodeArithExpr *l;
};

struct NodeArithAdd {
    NodeArithExpr *l, *r;
};

struct NodeArithMul {
    NodeArithExpr *l, *r;
};

struct NodeArithSub {
    NodeArithExpr *l, *r;
};

struct NodeArithDiv {
    NodeArithExpr *l, *r;
};

struct NodeArithQuot {
    NodeArithExpr *l, *r;
};

struct NodeArithRem {
    NodeArithExpr *l, *r;
};

// NOTE:
// incr or decr operations techincally falls under control flow
// as of now they will not be implemented

struct NodeArithExpo {
    NodeArithExpr *l, *r;
};

// Binary arithmetic operators, defaulted to fundamental operations
struct NodeArithBin {
    std::variant<NodeArithAdd*, NodeArithSub*, NodeArithMul*, NodeArithDiv*, \
        NodeArithQuot*, NodeArithRem*, NodeArithExpo*> binop;
};

// Arithmetic wrappers for tree descents, defaulting to paren, int lit, idents 
/*
struct NodeArithTerm {
    std::variant<NodeParen*, NodeIdent*, NodeIntLit*> term;
};
*/

// Arithmetic expr wrapper
/*
struct NodeArithExpr {
    std::variant<NodeArithTerm*, NodeArithBin*> expr;
};
*/

// ======== Logical Nodes =========
struct NodeBoolLit {
    Token bool_lit;
};

struct NodeLogicGt {
    NodeExpr *l, *r;
};

struct NodeLogicGe {
    NodeExpr *l, *r;
};

struct NodeLogicLt {
    NodeExpr *l, *r;
};

struct NodeLogicLe {
    NodeExpr *l, *r;
};

struct NodeLogicEq {
    NodeExpr *l, *r;
};

struct NodeLogicNe {
    NodeExpr *l, *r;
};

struct NodeLogicAnd {
    NodeExpr *l, *r;
};

struct NodeLogicOr {
    NodeExpr *l, *r;
};

struct NodeLogicNot {
    NodeExpr *l, *r;
};

struct NodeLogicXor {
    NodeExpr *l, *r;
};

// Logical shift right
// left: num ro-be-shifted
// right: pos to-be-shifted
struct NodeLogicSrl {
    NodeExpr *l, *r;
};

// Logical shift left
// left: num ro-be-shifted
// right: pos to-be-shifted
struct NodeLogicSll {
    NodeExpr *l, *r;
};

// Binary logical operators, defaulted to fundamental operations
struct NodeLogicBin {
    std::variant<NodeLogicGt*, NodeLogicGe*, NodeLogicLt*, NodeLogicLe*, \
        NodeLogicEq*, NodeLogicNe*, NodeLogicAnd*, NodeLogicOr*, \
        NodeLogicNot*, NodeLogicXor*, NodeLogicSrl*, NodeLogicSll*> binop;
};

// ======== Summed terms and expressions ========
//
struct NodeTerm {
    std::variant<NodeParen*, NodeIdent*, \
        NodeBoolLit*, NodeIntLit*> term;
};

struct NodeExpr {
    std::variant<NodeArithBin*, NodeLogicBin*, \
        NodeTerm*> expr;
};

// ======== Statements and Control Flows ========
//
struct NodeStmt;

// Contains: statement exit steps
struct NodeStmtEnd {
    NodeExpr *expr;
};

struct NodeScope {
    std::vector<NodeStmt*> stmts;
};

struct NodeStmtLet {
    Token ident;
    NodeExpr *expr {};
};

struct PredIf;

struct PredIfElif {
    NodeExpr *expr {};
    NodeScope *scope {};
    std::optional<PredIf*> pred;
};

struct PredIfElse {
    NodeScope *scope;
};

// Else If predicates
struct PredIf {
    std::variant<PredIfElif*, PredIfElse*> expr;
};


struct NodeStmtIf {
    NodeExpr *expr {};
    NodeScope *scope {};
    std::optional<PredIf*> pred;
};

struct NodeStmtAssign {
    Token ident;
    NodeExpr* expr {};
};

struct NodeStmt {
    std::variant<NodeStmtEnd*, NodeStmtLet*, NodeScope*, NodeStmtIf*, NodeStmtAssign*> var;
};

struct NodeProgram {
    std::vector<NodeStmt*> stmts;
};

// ======== Define general binary operations (currying) ========

struct NodeBin {
    NodeExpr *l, *r;
};

struct NodeBinFunc {
    std::vector<NodeStmt*> stmts;
    std::queue<Token> args;
    NodeScope *scope {};
};

struct NodeFuncApp {
    NodeBinFunc *func;
    Token *arg;
};

// Abstract constructors
struct NodeExprCon;

struct NodeMap {
    NodeBinFunc *func;
    NodeExprCon *list;
};

// Constructors should theretically be universal
// However for the current implementation it will be limited to arithmetic objects
// Left expr, right con, hence foldl applicable
struct NodeExprCon {
    NodeExpr* expr;
    NodeExprCon* con;
};





/**
 * @brief ExpType token conversion table
 *
 */
static std::unordered_map<std::string, const ExpType> tokTable = {
    { "return", ExpType::_return },
    { "if", ExpType::_if },
    // { "let", ExpType::_let },
    { "elif", ExpType::_elif },
    { "else", ExpType::_else },
    { "while", ExpType::_while },
    { "for", ExpType::_for },
    { "(", ExpType::_lpar },
    { "[", ExpType::_lbra },
    { "::", ExpType::_tdef },
    { ":", ExpType::_sdef },
    { "==", ExpType::_eq },
    { "=", ExpType::_assign },
    { "->", ExpType::_map },
    { "--", ExpType::_decr },
    { "-", ExpType::_sub },
    { "++", ExpType::_incr },
    { "+", ExpType::_add },
    { "*", ExpType::_mul },
    { "/", ExpType::_quot },
    { "%", ExpType::_rem },
    { "?", ExpType::_if },
    { "~", ExpType::_else },
    { "&", ExpType::_and },
    { "|", ExpType::_or },
    { "^", ExpType::_xor },
    { ">=", ExpType::_ge },
    { ">", ExpType::_gt },
    { "<=", ExpType::_le },
    { "<", ExpType::_lt },
};


/**
 * @brief GroundValueType token conversion table
 *
 */
static std::unordered_map<std::string, const GroundValueType> tyTable = {
    {"char", GroundValueType::_char},
    {"bool", GroundValueType::_bool},
    {"int", GroundValueType::_int},
    {"float", GroundValueType::_float},
};


/**
 * @brief (deprecated) Classify level-of-operation of arith
 * 
 * @param type 
 * @return std::optional<int> 
 */
inline std::optional<int> alu_c(const ExpType type) {
    switch (type) {
        case ExpType::_add:
        case ExpType::_sub:
            return 0;
        case ExpType::_mul:
        case ExpType::_div:
        case ExpType::_quot:
        case ExpType::_rem:
            return 1;
        case ExpType::_exp:
            return 2;
        default:
            return {};
    }
}



class Tokenizer {
public:
    explicit Tokenizer(std::string input)
        : m_src(std::move(input))
    {
    }

    
    Token classify(const std::string &type, const std::string &buffer, const std::string &content) {
        if (tokTable.find(buffer) == tokTable.end()) {
            // variable
            env.insert(buffer);
            
        }
    }

    /**
     * @brief splits given code content into tokens
     * 
     * @param input 
     * @return std::vector<Token> 
     */
    std::vector<Token> tokenize() {
        // OOP currently not supported
        // size_t last = 0, curr = 0;

        // ty -> explicit type declaration, currently not available
        // buf -> variable buffer
        // cnt -> value content
        std::string ty, buf, cnt;
        size_t q_row = 0, q_col = 0;

        // bool comment_flag = false;
        bool string_flag = false;
        std::vector<Token> tokens;

        while (char c = peek().has_value()) {
            if (c == ' ' || c == '\n' || c == ';') {
                // End of a term
                tokens.emplace_back(classify(ty, buf, cnt));
                if (c == '\n') {
                    row++; col = 0;
                } else {
                    col++;
                }
                continue;
            }
            if (string_flag) {
                // This indicates that the scanner is amongst a string,
                // the parser would render everything in a string as one token
                while ((peek(-1).value() == '\\' && peek().value() == '\"') || peek().value() != '\"') {
                    cnt.push_back(coom());
                } 
                string_flag = false;
                continue;
            }
            
            // Special character map
            switch (c) {
                case '=':
                    // pass `==` through to map conversion
                    if (peek(1).value() == '=') break;
                    pad(); 
                    // assignment operator
                    char d;
                    while (peek().has_value() && \
                          (peek().value() != ' ' || peek().value() != '\n' || peek().value() != ';')) {
                        // extract string
                        cnt.push_back(coom());
                    }
                    tokens.emplace_back();
                    continue;
                case ';':
                    // inline break
                    // i.e. ` a++; b++; c++ `
                    pad();
                    if (!std::isalpha(peek().value())) {
                        std::cerr << "lex -> tokenize : symbol - expecting valid expression, \
                                     ';' should be used for multiple statements in a line at row: " \
                                  << row << " | col: " << col << std::endl;
                        exit(EXIT_FAILURE); 
                    }
                    continue;
                case '\"':
                    // '"' indicates head of string
                    // ...:
                    if (!string_flag) {
                        // string begins
                        string_flag = true;
                    } else {
                        std::cerr << "lex -> tokenize : symbol - missing \" at end of string, \
                                     starting \" at row: " \
                                  << row << " | col: " << col << std::endl;
                        exit(EXIT_FAILURE); 
                    }
                    coom();
                    continue;
                case '#':
                    // '#' backslash indicates comments
                    // skip until end of comment
                    while (peek().value() != '\n') coom();
                    coom();
                    continue;
                // case '\\':
                //     // non-string '\\' allows statemtn across lines
                //     coom();
                //     break;
                default:
                    // pass-through statement
                    break;
            }

            if (std::isalpha(c)) {
                
                buf.push_back(coom());
                while (peek().has_value() && std::isalnum(peek().value())) {
                    buf.push_back(coom());
                }
                if (env.find(buf) == env.end()) {
                    // variable declaration
                    env.insert(buf);
                }
                tokens.push_back({ ExpType::_var, row, col });
                // The above may be ditched and replaced by actual
                // eval / tyinf steps
                buf.clear();
                continue;
            }
            if (std::isdigit(c)) {
                // numeric value
                buf.push_back(coom());
                bool float_flag = false;
                while (peek().has_value()) {
                    if (std::isdigit(peek().value()) || peek().value() == '.') {
                        buf.push_back(coom());
                        if (float_flag && peek().value() == '.') {
                            std::cerr << "lex -> tokenize : numerical - multiple `.` in numerical value at row: " \
                                << row << " | col: " << col << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        float_flag = peek().value() == '.';
                    } else {
                        std::cerr << "lex -> tokenize : numerical - hostile symbol / alphabet in number at row: " \
                                  << row << " | col: " << col << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
                tokens.push_back({ (float_flag) ? ExpType::_float: ExpType::_int, row, col, buf });
                // Wait this is the job of the evaluator wtf
                // tok->type = ExpType::_number;

                buf.clear();
                continue;
            }
            switch (c) {
                default:
                    // unknown term
                    std::cerr << "lex -> tokenize : term - unknown term at row: " \
                        << row << " | col: " << col << std::endl;
                    break;
            }
        }

      return tokens;
    }

    /**
     * @brief substitute value to variable before `=`
     * 
     * @param 
     * @return std::string 
     */
    std::string valsub() {
        pad();
        char c = peek().value();
        switch (c) {
            case '\"':
                // string
                return crop();
            case '\'':
                // char
                return crop();
            default:
                break;
        }
        if (std::isdigit(c)) {
            // coom();
            // if (peek().value() == 'x') {
            //     // hex
            // } else if (peek().value() == 'b') {
            //     // bin
            // } else
            return crop();
        }
        if (std::isalpha(c)) {
            // varsub
            return crop();
        }
    }

    

    /**
     * @brief builds parse tree off the tokens
     * 
     * @param tokens 
     * @return std::vector<std::string> 
     */
    std::vector<std::string> tree(std::vector<std::string> tokens) {
        // builds a parse tree
      return {0};
    }

private:
    const std::string m_src;                // content
    size_t m_idx = 0;                       // padding index
    std::unordered_set<std::string> env;    // var environment

    // variable suffix substitution - may be redundant
    std::unordered_map<std::string, std::string> delta; 
    size_t row = 0, col = 0;                // scanner pos

    /**
     * @brief more convenient `peek()`
     * 
     * @param offset=0
     * @return std::optional<char> 
     */
    [[nodiscard]] std::optional<char> peek(const size_t offset = 0) const {
        if (m_idx + offset >= m_src.length()) return {};
        return m_src.at(m_idx + offset);
    }

    /**
     * @brief move pointer until it is not a space
     * 
     */
    void pad() {
        coom();
        while (peek().value() == ' ')
            coom();
    }

    /**
     * @brief crops a term
     *  helper function for valsub() and tokenize()
     * 
     * @return std::string 
     */
    std::string crop() {
        std::string buffer;
        while (peek().value() != ' ')
            buffer.push_back(coom());
        return buffer;
    }

    /**
     * @brief move pointer on content one char forward
     * 
     */
    inline char coom() {col++; return m_src.at(m_idx++);}
};

#endif
