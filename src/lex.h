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
#include <vector>
#include <string>
#include <sstream>
#include <optional>
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
    _char,
    _bool,
    _assign,
    _apply,             // for FP
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
};

enum class GroundValueType {
    // _unit,
    _bool,
    _int_32,
};


struct Token {
    ExpType ty;
    size_t row = 0, col = 0;
    std::optional<std::string> val {};
};

inline std::optional<int> alu_c(const ExpType type) {
    switch (type) {
        case ExpType::_add:
        case ExpType::_sub:
            return 0;
        case ExpType::_mul:
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

    Token classify(const std::string& buffer) {
        std::string filter;
        bool comment_flag = false;
        for (int i = 0; i = buffer.size(); i++) {
            // This is the filter
            // this is potentially dangerous
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
        std::string buf;

        // bool comment_flag = false;
        bool string_flag = false;

        std::vector<Token> tokens;
        Token *tok = new Token();
        

        while (peek().has_value()) {
            char c = peek().value();
            if (c == ' ' || c == '\n' || c == ';') {
                // End of a term
                tokens.emplace_back(&tok);
                tok = new Token();
                if (c == '\n') {
                    row++; col = 0;
                } else {
                    col++;
                }
                continue;
            }
            // Redundant
            // if (comment_flag) {
            //     // This indicates that the scanner is in the middle of a comment,
            //     // the parse should ignore these section until comment reaches end
            //     while (peek().value() != '\n') coom();
            //     continue;
            // }
            if (string_flag) {
                // This indicates that the scanner is amongst a string,
                // the parser would render everything in a string as one token
                while (peek().value() != '"') {
                    buf.push_back(coom());
                } 

            }
            switch (c) {
                case '(':
                    // start of expression parenthesis
                    tokens.push_back({ ExpType::_lpar, row, col });
                    continue;
                case '[':
                    // array index
                    tokens.push_back({ ExpType::_lbra, row, col });
                    continue;
                case ':':
                    /**
                     * could be
                     *  - `::`  type definition for function
                     *  - `:`   ternary `else` operator
                     */
                    coom();
                    if (peek().value() == ':')
                        tokens.push_back({ ExpType::_tdef, row, col });
                    else
                        tokens.push_back({ ExpType::_sdef, row, col });
                    continue;
                /*
                case '{':
                    // start of brackets

                    break;
                */
                case '=':
                    /**
                     * could be
                     *  - `::`  type definition for function
                     *  - `:`   ternary `else` operator
                     */
                    tokens.push_back({ ExpType::_assign, row, col });
                    
                    buf.clear();
                    continue;
                case '-':
                    /**
                     * could be
                     *  - `-`   negation OR minus arith operator
                     *  - `--`  decrement arith operator
                     *  - `->`  type map definition for function
                     */
                    coom();
                    if (peek().value() == '>')
                        tokens.push_back({ ExpType::_map, row, col });
                    else if (peek().value() == '-')
                        tokens.push_back({ ExpType::_decr, row, col });
                    else
                        tokens.push_back({ ExpType::_sub, row, col });
                    continue;
                case '+':
                    /**
                     * could be
                     *  - `+`   plus arith operator
                     *  - `++`  increment arith operator
                     */
                    coom();
                    if (peek().value() == '+') {
                        tokens.push_back({ ExpType::_incr, row, col });
                        coom();
                    } else
                        tokens.push_back({ ExpType::_add, row, col });
                    continue;
                case '*':
                    coom();
                    tokens.push_back({ ExpType::_mul, row, col });
                    continue;
                case '/':
                    // quotient arith operator
                    coom();
                    tokens.push_back({ ExpType::_quot, row, col });
                    continue;
                case '%':
                    // remainder arith operator
                    coom();
                    tokens.push_back({ ExpType::_rem, row, col });
                    continue;
                case '?':
                    // ternary logical operator
                    // this can be treated as an `if`
                    coom();
                    tokens.push_back({ ExpType::_if, row, col });
                    continue;
                case '~':
                    // ternary logical operator
                    // this can be treated as an `else`
                    coom();
                    tokens.push_back({ ExpType::_else, row, col });
                    continue;
                case '&':
                    // `and` logical operator
                    coom();
                    tokens.push_back({ ExpType::_and, row, col });
                    continue;
                case '|':
                    // `or` logical operator
                    coom();
                    tokens.push_back({ ExpType::_or, row, col });
                    continue;
                case '>':
                    /**
                     * could be
                     *  - `>`   `gt` 
                     *  - `>=`  `ge`
                     */
                    coom();
                    if (peek().value() == '=')
                        tokens.push_back({ ExpType::_ge, row, col });
                    else
                        tokens.push_back({ ExpType::_gt, row, col });
                    continue;
                case '<':
                    /**
                     * could be
                     *  - `<`   `lt`
                     *  - `<=`  'le`
                     */
                    coom();
                    if (peek().value() == '=')
                        tokens.push_back({ ExpType::_le, row, col });
                    else
                        tokens.push_back({ ExpType::_lt, row, col });
                    continue;
                case ';':
                    // inline break
                    // i.e. ` a++; b++; c++ `
                    coom();
                    if (peek().value() != ' ')
                        std::cerr << "lex -> tokenize : symbol - expecting space, ';' should be used for multiple statements in a line at row: " \
                            << row << " | col: " << col << std::endl;
                        exit(EXIT_FAILURE); 
                    break;
                case '"':
                    // '"' indicates head of string
                    // ...:
                    if (!string_flag) {
                        // string begins
                        string_flag = true;
                    } else {
                        string_flag = false;
                        tokens.push_back({ ExpType::_char, row, col });
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
                //     continue;
                default:
                    // pass-through statement

                    break;
            }
            if (std::isalpha(c)) {
                
                buf.push_back(coom());
                while (peek().has_value() && std::isalnum(peek().value())) {
                    buf.push_back(coom());
                }
                // ============ keyword ============ 
                if (buf == "return")
                    tokens.push_back({ ExpType::_return, row, col });
                else if (buf == "let")
                    tokens.push_back({ ExpType::_let, row, col });
                else if (buf == "for")
                    tokens.push_back({ ExpType::_for, row, col });
                else if (buf == "if")
                    tokens.push_back({ ExpType::_if, row, col });
                else if (buf == "elif")
                    tokens.push_back({ ExpType::_elif, row, col });
                else if (buf == "else")
                    tokens.push_back({ ExpType::_else, row, col });
                // ============ variable ============ 
                else {
                    if (env.find(buf) == env.end()) {
                        // variable declaration
                        env.insert(buf);
                    }
                    tokens.push_back({ ExpType::_var, row, col });
                }
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
                        continue;
                    }
                }
                tokens.push_back({ (float_flag) ? ExpType::_int : ExpType::_float, row, col, buf });
                // Wait this is the job of the evaluator wtf
                // tok->type = ExpType::_number;

                buf.clear();
            }
            switch (c) {
                
                default:
                    // unknown term
                    std::cerr << "lex -> tokenize : term - unknown term at row: " \
                        << row << " | col: " << col << std::endl;
                    break;
            }
        }

        // while ((next = input.find(' ', last)) != std::string::npos) {
        //     // std::cout << input.substr(last, next-last) << "\n";
        //     Token classified = classify(input.substr(last, next - last));

        //     tokens.emplace_back(buf);
            
        //     last = next + 1;
        // }
        // std::cout << input.substr(last) << "\n";
      
      return tokens;
    }

    /**
     * @brief builds parse tree off the tokens
     * 
     * @param tokens 
     * @return std::vector<std::string> 
     */
    std::vector<std::string> tree(std::vector<std::string> tokens) {
      return nullptr;
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
     * @brief move pointer on content one char forward
     * 
     */
    inline char coom() {col++; return m_src.at(m_idx++);}
};

#endif
