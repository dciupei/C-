#include "Parser.h"
#include <sstream>
#include <stdexcept>

void Parser::match(Token tok) {
    if (tok != lookahead_) {
        std::stringstream ss;
        ss << "Unexpected token '" << tokenToString(lookahead_) << "', ";
        ss << "Expecting '" << tokenToString(tok) << "'";
        throw std::runtime_error(ss.str());
    }
    lookahead_ = scanner_.nextToken(attribute_, lineno_);
}

void Parser::parse() {
    lookahead_ = scanner_.nextToken(attribute_, lineno_);
    try {
        prog();
    } catch(const std::exception& error) {
        std::stringstream ss;
        ss << lineno_ << ": " << error.what();
        throw std::runtime_error(ss.str());
    }
}

void Parser::prog() {
    stmt_seq();
    match(Token::EOT);
}

void Parser::stmt_seq() {
    while (lookahead_ != Token::EOT) {
        Stmt *s = stmt();
        AST_.push_back(s);
    }
}

Stmt *Parser::stmt() {
    if (lookahead_ == Token::IDENT)
        return assign();
    else if (lookahead_ == Token::IF)
        return if_stmt();
    else if (lookahead_ == Token::WHILE)
        return while_stmt();
    else
        return action();
}

Stmt *Parser::assign() {
    std::string name = attribute_.s;
    match(Token::IDENT);
    match(Token::ASSIGN);
    Expr *e = expr();
    return new AssignStmt(name, e);
}

Stmt *Parser::block() {             //stmt {stmts}
    Stmt *s = stmt();
    std::vector<Stmt*> s_;
    s_.push_back(s);
    while(lookahead_ != Token::ELSE && lookahead_ != Token::ELSIF && lookahead_ != Token::FI && lookahead_ != Token::OD) {
        s = stmt();
        s_.push_back(s);
    }
    return new BlockStmt(s_);
}

Stmt *Parser::while_stmt() {
    match(Token::WHILE);
    Expr *c = bool_();
    match(Token::DO);
    Stmt *b = block();
    match(Token::OD);
    return new WhileStmt(c, b);
}

Stmt *Parser::elsePart() {
    if(lookahead_==Token::ELSIF){
            match(Token::ELSIF);
            Expr *c = bool_();
            match(Token::THEN);
            Stmt *b = block();
            Stmt *e = elsePart();
            return new IfStmt(c, b, e);
        }
    if(lookahead_==Token::ELSE){
            match(Token::ELSE);
            Stmt *b = block();
            match(Token::FI);
            return b;
    }
    match(Token::FI);
    return NULL;
    
}

Stmt *Parser::if_stmt() {
    match(Token::IF);
    Expr *c = bool_();
    match(Token::THEN);
    Stmt *b = block();
    Stmt *e = elsePart();
    return new IfStmt(c, b, e);
}

Stmt *Parser::action() {
    switch(lookahead_) {
        case Token::HOME:    match(Token::HOME); return new HomeStmt();
        case Token::PENUP:   match(Token::PENUP); return new PenUpStmt();
        case Token::PENDOWN: match(Token::PENDOWN); return new PenDownStmt();
        case Token::FORWARD: match(Token::FORWARD); return new ForwardStmt(expr());
        case Token::LEFT:    match(Token::LEFT); return new LeftStmt(expr());
        case Token::RIGHT:   match(Token::RIGHT); return new RightStmt(expr());
        case Token::PUSHSTATE:
            match(Token::PUSHSTATE); return new PushStateStmt();
        case Token::POPSTATE:
            match(Token::POPSTATE); return new PopStateStmt();
        default:
            throw std::runtime_error("Expecting turtle action statement!");
    }
}

Expr *Parser::expr() {
    Expr *e = term();
    while (lookahead_ == Token::PLUS ||
           lookahead_ == Token::MINUS) {
        const auto op = lookahead_;
        match(lookahead_);
        Expr *t = term();
        if (op == Token::PLUS)
            e = new AddExpr(e, t);
        else
            e = new SubExpr(e, t);
    }
    return e;
}

Expr *Parser::term() {
    Expr *e = factor();
    while (lookahead_ == Token::MULT ||
           lookahead_ == Token::DIV) {
        Token op = lookahead_;
        match(lookahead_);
        Expr *t = factor();
        if (op == Token::MULT)
            e = new MulExpr(e, t);
        else
            e = new DivExpr(e, t);
    }
    return e;
}

Expr *Parser::factor() {
    switch(lookahead_) {
        case Token::PLUS:   match(Token::PLUS); return factor();
        case Token::MINUS:  match(Token::MINUS); return new NegExpr(factor());
        case Token::LPAREN:
        {
            match(Token::LPAREN);
            Expr *e = expr();
            match(Token::RPAREN);
            return e;
        }
        case Token::IDENT:
        {
            const std::string name = attribute_.s;
            match(Token::IDENT);
            return new VarExpr(name);
        }
        case Token::REAL:
        {
            const float val = attribute_.f;
            match(Token::REAL);
            return new ConstExpr(val);
        }
        default:
            throw std::runtime_error("Expecting factor!");
    }
}

Expr *Parser::bool_() {
    Expr *b = bool_term();
    while(lookahead_ == Token::OR) {
        match(Token::OR);
        Expr *b1 = bool_term();
        b = new BoolTerm(b, b1);
    }
   return b;
}


Expr *Parser::bool_term() {
    Expr *b = bool_factor();
    while(lookahead_ == Token::AND){
            match(Token::AND);
            Expr *b1 = bool_factor();
            b = new BoolFactor(b, b1);
    }
    return b;
}

Expr *Parser::bool_factor() {
    if(lookahead_ == Token::NOT){
        match(Token::NOT);
        Expr *b = bool_factor();
        return b;
    }else if(lookahead_ == Token::LPAREN){
        match(Token::LPAREN);
        Expr *b1 = bool_();
        match(Token::RPAREN);
        return b1;
    }
    return cmp();
}

Expr *Parser::cmp() {
    Expr *exp = expr();
    switch(lookahead_) {
        case Token::NE:
            match(Token::NE);
            return new CmpNE(exp, expr());
        case Token::EQ:
            match(Token::EQ);
            return new CmpEQ(exp, expr());
        case Token::LT:
            match(Token::LT);
            return new CmpLT(exp, expr());
        case Token::LE:
            match(Token::LE);
            return new CmpLE(exp, expr());
        case Token::GT:
            match(Token::GT);
            return new CmpGT(exp, expr());
        case Token::GE:
            match(Token::GE);
            return new CmpGE(exp, expr());
    
        default: throw std::runtime_error("Error!");
    }
}


