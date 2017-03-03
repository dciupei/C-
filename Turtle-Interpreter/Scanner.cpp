#include "Scanner.h"
#include <locale>
#include <sstream>
#include <map>

Token Scanner::nextToken(Attribute& attr, int& lineno) {
    int c;
    lineno_ = 1;
    //
    // Eat whitespace and comments
    //
top:
    do {
        c = in_.get();
    } while (std::isspace(c));
    
    if (c == '#') {
        do {
            c = in_.get();
            if (c == EOF) return Token::EOT;
        } while (c != '\n');
        lineno_++;
        goto top;
    }
    
    if (c == EOF) return Token::EOT;
    if (c == '+') return Token::PLUS;
    if (c == '-') return Token::MINUS;
    if (c == '*') return Token::MULT;
    if (c == '/') return Token::DIV;
    if (c == '(') return Token::LPAREN;
    if (c == ')') return Token::RPAREN;
    if (c == '=') return Token::EQ;
    if (c == '\n') lineno_++;
    
    if (c == ':') {  // assign :=
        c = in_.get();
        if (c != '=')
            throw std::runtime_error("Unknown lexeme");
        return Token::ASSIGN;
    }
    
    if (c == '>') {                     //greater or equal
        c = in_.get();
        if (c != '='){
            in_.unget();
            return Token::GT;
        }
        return Token::GE;
    }
    
    if (c == '<') {                     //less then, Not equal, less then equal
        c = in_.get();
        if (c == '>'){
            return Token::NE;
        } else if (c == '='){
            return Token::LE;
        } else {
            in_.unget();
            return Token::LT;
        }
    }
    
    //
    // REAL
    //
    if (std::isdigit(c)) {
        std::string buf = "";
        do {
            buf.push_back(c);
            c = in_.get();
        } while (std::isdigit(c));
        if (c == '.') {
            buf.push_back(c);
            c = in_.get();
            while (std::isdigit(c)) {
                buf.push_back(c);
                c = in_.get();
            }
        }
        in_.unget();
        attr.f = std::stod(buf);
        return Token::REAL;
    }
    
    //
    // IDENT or a reserved word
    //
    if (std::isalpha(c) || c == '_') {
        std::string buf = "";
        do {
            buf.push_back(c);
            c = in_.get();
        } while (std::isalnum(c) || c == '_');
        in_.unget();
        attr.s = buf;
        Token T = stringToToken(buf);
        return T;
    }
    throw std::runtime_error("Unknown lexeme");
}

Token stringToToken(std::string str) {           //checks the string returns token
    static std::map<std::string,Token> tokenMap = {
        {"UNKNOWN", Token::UNKNOWN },
        {"EOT", Token::EOT},
        {"IDENT", Token::IDENT},
        {"REAL", Token::REAL},
        {"ASSIGN", Token::ASSIGN},
        {"EQ", Token::EQ},
        {"NE", Token::NE},
        {"LE", Token::LE},
        {"LT", Token::LT},
        {"GE", Token::GE},
        {"GT", Token::GT},
        {"LPAREN", Token::LPAREN},
        {"RPAREN", Token::RPAREN},
        {"OR", Token::OR},
        {"AND", Token::AND},
        {"NOT", Token::NOT},
        {"PLUS", Token::PLUS},
        {"MINUS", Token::MINUS},
        {"MULT", Token::MULT},
        {"DIV", Token::DIV},
        {"IF", Token::IF},
        {"THEN", Token::THEN},
        {"ELSIF", Token::ELSIF},
        {"ELSE", Token::ELSE},
        {"FI", Token::FI},
        {"WHILE", Token::WHILE},
        {"DO", Token::DO},
        {"OD", Token::OD},
        {"HOME", Token::HOME},
        {"PENUP", Token::PENUP},
        {"PENDOWN", Token::PENDOWN},
        {"FORWARD", Token::FORWARD},
        {"LEFT", Token::LEFT},
        {"RIGHT", Token::RIGHT},
        {"PUSHSTATE", Token::PUSHSTATE},
        {"POPSTATE", Token::POPSTATE}
    };
    auto p = tokenMap.find(str);
    if (p != tokenMap.end())
        return p->second;
    return Token::IDENT;
    
}


std::string tokenToString(Token token) {
    static std::map<Token,std::string> tokenMap = {
        {Token::UNKNOWN, "UNKNOWN"},
        {Token::EOT,"EOT"},
        {Token::IDENT, "IDENT"},
        {Token::REAL, "REAL"},
        {Token::ASSIGN, "ASSIGN"},
        {Token::EQ, "EQ"},
        {Token::NE, "NE"},
        {Token::LE, "LE"},
        {Token::LT, "LT"},
        {Token::GE, "GE"},
        {Token::GT,"GT"},
        {Token::LPAREN, "LPAREN"},
        {Token::RPAREN, "RPAREN"},
        {Token::OR, "OR"},
        {Token::AND, "AND"},
        {Token::NOT, "NOT"},
        {Token::PLUS, "PLUS"},
        {Token::MINUS, "MINUS"},
        {Token::MULT, "MULT"},
        {Token::DIV,"DIV"},
        {Token::IF, "IF"},
        {Token::THEN, "THEN"},
        {Token::ELSIF, "ELSIF"},
        {Token::ELSE, "ELSE"},
        {Token::FI,"FI"},
        {Token::WHILE, "WHILE"},
        {Token::DO, "DO"},
        {Token::OD,"OD"},
        {Token::HOME, "HOME"},
        {Token::PENUP, "PENUP"},
        {Token::PENDOWN, "PENDOWN"},
        {Token::FORWARD, "FORWARD"},
        {Token::LEFT, "LEFT"},
        {Token::RIGHT, "RIGHT"},
        {Token::PUSHSTATE, "PUSHSTATE"},
        {Token::POPSTATE,"POPSTATE"}
    };
    auto p = tokenMap.find(token);
    if (p != tokenMap.end())
        return p->second;
    return "?";
    
}

