#ifndef TURTLE_AST_H
#define TURTLE_AST_H

#include <string>
#include <iostream>
#include <vector>
#include <iomanip>
#include "Env.h"

//
// Abstract base class for all expressions.
//
class Expr {
public:
    virtual ~Expr() {}
    virtual float eval(Env& env) const = 0;
};

//
// Abstract base class for all statements.
//
class Stmt {
public:
    virtual ~Stmt() {};
    virtual void execute(Env& env) = 0;
};

//
// AST's expressions and statements go here.
//
class AssignStmt : public Stmt {
protected:
    const std::string _name;  //l-value
    Expr *_expr; // r-value
public:
    AssignStmt(const std::string& n, Expr *e) : _name{n}, _expr{e} {}
    virtual void execute(Env& env) {
        env.put(_name, _expr->eval(env));
    }
};

class WhileStmt : public Stmt {
protected:
    Expr *_expr;
    Stmt *_stmt;
public:
    WhileStmt(Expr *cond, Stmt *body) : _expr{cond}, _stmt{body} {}
    virtual void execute(Env &env) {
        while(_expr->eval(env)) {
            _stmt->execute(env);
        }
    }
};

class IfStmt : public Stmt {
protected:
    Expr *_c;
    Stmt *_b, *_e;
public:
    IfStmt(Expr *c, Stmt *b, Stmt *e) : _c{c}, _b{b}, _e{e} {}
    virtual void execute(Env &env) {
        if(_c->eval(env)) {                  //if condition true
            _b->execute(env);                //evaluate the body
        }
        else if(_e != NULL) {                //if else not null
            _e->execute(env);                //evaluate the else
        }
    }
};

class BlockStmt : public Stmt {
protected:
    std::vector<Stmt*> _stmts;
    Stmt *_s;
public:
    BlockStmt(std::vector<Stmt*> stmts) : _stmts{stmts} {}
    virtual void execute(Env &env) {
        for(auto const& _s: _stmts)             //iterates the vector
            _s->execute(env);
    }
};

class HomeStmt : public Stmt {
public:
    virtual void execute(Env& env) {
        std::cout << "H" << std::endl;
    }
};

class PenUpStmt : public Stmt {
public:
    virtual void execute(Env& env) {
        std::cout << "U" << std::endl;
    }
};

class PenDownStmt : public Stmt {
public:
    virtual void execute(Env& env) {
        std::cout << "D" << std::endl;
    }
};

class PushStateStmt : public Stmt {
public:
    virtual void execute(Env& env) {
        std::cout << "[" << std::endl;
    }
};

class PopStateStmt : public Stmt {
public:
    virtual void execute(Env& env) {
        std::cout << "]" << std::endl;
    }
};

class ForwardStmt : public Stmt {
protected:
    Expr *_dist;
public:
    ForwardStmt(Expr *e) : _dist{e} {}
    virtual void execute(Env& env) {
        const float d = _dist->eval(env);
        std::cout << "M " << d << std::endl;
    }
};

class RightStmt : public Stmt {
protected:
    Expr *_angle;
public:
    RightStmt(Expr *e) : _angle{e} {}
    virtual void execute(Env& env) {
        const float a = _angle->eval(env);
        std::cout << "R " << -a << std::endl;
    }
};

class LeftStmt : public Stmt {
protected:
    Expr *_angle;
public:
    LeftStmt(Expr *e) : _angle{e} {}
    virtual void execute(Env& env) {
        const float a = _angle->eval(env);
        std::cout << "R " << a << std::endl;
    }
};

class VarExpr : public Expr {
protected:
    const std::string _name;
public:
    VarExpr(const std::string& n) : _name{n} {}
    virtual float eval(Env& env) const {
        return env.get(_name);
    }
};

class ConstExpr : public Expr {
protected:
    const float _val;
public:
    ConstExpr(float v) : _val{v} {}
    virtual float eval(Env& env) const {
        return _val;
    }
};

class UnaryExpr : public Expr {
protected:
    Expr *_expr;
public:
    UnaryExpr(Expr *e) : _expr{e} {}
};

class NegExpr : public UnaryExpr {
public:
    NegExpr(Expr *e) : UnaryExpr(e) {}
    virtual float eval(Env& env) const {
        return -_expr->eval(env);
    }
};

class BinaryExpr : public Expr {
protected:
    Expr *_left, *_right;
public:
    BinaryExpr(Expr *l, Expr *r) : _left{l}, _right{r} {}
};

class AddExpr : public BinaryExpr {
public:
    AddExpr(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env& env) const {
        return _left->eval(env) + _right->eval(env);
    }
};

class SubExpr : public BinaryExpr {
public:
    SubExpr(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env& env) const {
        return _left->eval(env) - _right->eval(env);
    }
};

class MulExpr : public BinaryExpr {
public:
    MulExpr(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env& env) const {
        return _left->eval(env) * _right->eval(env);
    }
};

class DivExpr : public BinaryExpr {
public:
    DivExpr(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env& env) const {
        return _left->eval(env) / _right->eval(env);
    }
};

class BoolTerm : public BinaryExpr {
public:
    BoolTerm(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env& env) const {
        return _left->eval(env) || _right->eval(env);
    }
};

class BoolFactor : public BinaryExpr {
public:
    BoolFactor(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env& env) const {
        return _left->eval(env) && _right->eval(env);
    }
};

class CmpNE : public BinaryExpr {               //NOT EQUAL
public:
    CmpNE(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env& env) const {
        return _left->eval(env) != _right->eval(env);
    }
};

class CmpEQ : public BinaryExpr {               //EQUAL
public:
    CmpEQ(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env& env) const {
        return _left->eval(env) == _right->eval(env);
    }
};

class CmpGT : public BinaryExpr {               //GREATER THAN
public:
    CmpGT(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env& env) const {
        return _left->eval(env) > _right->eval(env);
    }
};

class CmpGE : public BinaryExpr {               //GREATER THAN EQUAL
public:
    CmpGE(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env& env) const {
        return _left->eval(env) >= _right->eval(env);
    }
};

class CmpLT : public BinaryExpr {               //LESS THAN
public:
    CmpLT(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env& env) const {
        return _left->eval(env) < _right->eval(env);
    }
};

class CmpLE : public BinaryExpr {              //LESS THAN EQUAL
public:
    CmpLE(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env& env) const {
        return _left->eval(env) <= _right->eval(env);
    }
};



#endif //TURTLE_AST_H
