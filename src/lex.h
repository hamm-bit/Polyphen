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
    _char,
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
    _str,
    _bool,
    _int_32,
};



struct Token {
    ExpType ty;
    size_t row = 0, col = 0;
    std::optional<std::string> val {};
};


std::unordered_map<std::string, std::function<Token(int, int)>> tokTable = {
    { "return", [](int row, int col){ Token b={ ExpType::_return, row, col}; return b; } },
    { "if", [](int row, int col){ Token b={ ExpType::_if, row, col}; return b; } },
    { "let", [] (int row, int col){ Token b={ ExpType::_let, row, col}; return b;} },
    { "elif", [] (int row, int col){ Token b={ ExpType::_elif, row, col}; return b;} },
    { "else", [] (int row, int col){ Token b={ ExpType::_else, row, col}; return b;} },
    { "(", [] (int row, int col){ Token b={ ExpType::_lpar, row, col}; return b;} },
    { "[", [] (int row, int col){ Token b={ ExpType::_lbra, row, col}; return b;} },
    { "::", [] (int row, int col){ Token b={ ExpType::_tdef, row, col}; return b;} },
    { ":", [] (int row, int col){ Token b={ ExpType::_sdef, row, col}; return b;} },
    { "==", [] (int row, int col){ Token b={ ExpType::_eq, row, col}; return b;} },
    { "=", [] (int row, int col){ Token b={ ExpType::_assign, row, col}; return b;} },
    { "->", [] (int row, int col){ Token b={ ExpType::_map, row, col}; return b;} },
    { "--", [] (int row, int col){ Token b={ ExpType::_decr, row, col}; return b;} },
    { "-", [] (int row, int col){ Token b={ ExpType::_sub, row, col}; return b;} },
    { "++", [] (int row, int col){ Token b={ ExpType::_incr, row, col}; return b;} },
    { "+", [] (int row, int col){ Token b={ ExpType::_add, row, col}; return b;} },
    { "*", [] (int row, int col){ Token b={ ExpType::_mul, row, col}; return b;} },
    { "/", [] (int row, int col){ Token b={ ExpType::_quot, row, col}; return b;} },
    { "%", [] (int row, int col){ Token b={ ExpType::_rem, row, col}; return b;} },
    { "?", [] (int row, int col){ Token b={ ExpType::_if, row, col}; return b;} },
    { "~", [] (int row, int col){ Token b={ ExpType::_else, row, col}; return b;} },
    { "&", [] (int row, int col){ Token b={ ExpType::_and, row, col}; return b;} },
    { "|", [] (int row, int col){ Token b={ ExpType::_or, row, col}; return b;} },
    { "^", [] (int row, int col){ Token b={ ExpType::_xor, row, col}; return b;} },
    { ">=", [] (int row, int col){ Token b={ ExpType::_ge, row, col}; return b;} },
    { ">", [] (int row, int col){ Token b={ ExpType::_gt, row, col}; return b;} },
    { "<=", [] (int row, int col){ Token b={ ExpType::_le, row, col}; return b;} },
    { "<", [] (int row, int col){ Token b={ ExpType::_lt, row, col}; return b;} },
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

            // LUT for op and keyword
            if (tokTable.find(buf) != tokTable.end()) {
                tokens.push_back(tokTable[buf](row, col));
                buf.clear(); 
            }

            switch (c) {
                case ';':
                    // inline break
                    // i.e. ` a++; b++; c++ `
                    pad();
                    if (!std::isalpha(peek().value()))
                        std::cerr << "lex -> tokenize : symbol - expecting valid expression, \
                                     ';' should be used for multiple statements in a line at row: " \
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
                    break;
                case '#':
                    // '#' backslash indicates comments
                    // skip until end of comment
                    while (peek().value() != '\n') coom();
                    coom();
                    break;
                // case '\\':
                //     // non-string '\\' allows statemtn across lines
                //     coom();
                //     break;
                default:
                    // pass-through statement
                    break;
            }
            buf.clear();
            continue;

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
                        continue;
                    }
                    std::cerr << "lex -> tokenize : numerical - hostile symbol / alphabet in number at row: " \
                        << row << " | col: " << col << std::endl;
                    exit(EXIT_FAILURE);
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
<<<<<<< HEAD
      return nullptr;
=======
        // builds a parse tree
>>>>>>> refs/remotes/origin/main
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
