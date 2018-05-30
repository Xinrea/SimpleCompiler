#pragma once
#include <iostream>
#include <vector>

class CodeGenContext;
class NStatement;
class NExpression;
class NVariableDeclaration;

typedef std::vector<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;
typedef std::vector<NVariableDeclaration*> VariableList;

class Node {
public:
    virtual ~Node() {}
    virtual void printNode(int depth){}
};

class NExpression : public Node {
};

class NStatement : public Node {
};

class NInteger : public NExpression {
public:
    long long value;
    NInteger(long long value) : value(value) { }
    virtual void printNode(int depth){
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Const-Integer: " << value << std::endl;
    }
};

class NDouble : public NExpression {
public:
    double value;
    NDouble(double value) : value(value) { }
    virtual void printNode(int depth){
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Const-Float: " << value << std::endl;
    }
};

class NIdentifier : public NExpression {
public:
    std::string name;
    NIdentifier(const std::string& name) : name(name) { }
    virtual void printNode(int depth){
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Identifier: " << name << std::endl;
    }
};

class NMethodCall : public NExpression {
public:
    const NIdentifier& id;
    ExpressionList arguments;
    NMethodCall(const NIdentifier& id, ExpressionList& arguments) :
        id(id), arguments(arguments) { }
    NMethodCall(const NIdentifier& id) : id(id) { }
    virtual void printNode(int depth){
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Func_Call: " << std::endl;
        id.printNode(depth+1);
        for(auto i : arguments){
            i->printNode(depth+1);
        }
    }
};

class NBinaryOperator : public NExpression {
public:
    int op;
    NExpression& lhs;
    NExpression& rhs;
    NBinaryOperator(NExpression& lhs, int op, NExpression& rhs) :
        lhs(lhs), rhs(rhs), op(op) { }
    virtual void printNode(int depth){
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Operator_exp: " << std::endl;
        lhs.printNode(depth+1);
        rhs.printNode(depth+1);
    }
};

class NAssignment : public NExpression {
public:
    NIdentifier& lhs;
    NExpression& rhs;
    NAssignment(NIdentifier& lhs, NExpression& rhs) : 
        lhs(lhs), rhs(rhs) { }
    virtual void printNode(int depth){
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Assignment_exp: " << std::endl;
        lhs.printNode(depth+1);
        rhs.printNode(depth+1);
    }
};

class NBlock : public NExpression {
public:
    StatementList statements;
    NBlock() { }
    virtual void printNode(int depth){
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Block: " << std::endl;
        for(auto i : statements){
            i->printNode(depth+1);
        }
    }
};

class NExpressionStatement : public NStatement {
public:
    NExpression& expression;
    NExpressionStatement(NExpression& expression) : 
        expression(expression) { }
    virtual void printNode(int depth){
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Exp: " << std::endl;
        expression.printNode(depth+1);
    }
};

class NVariableDeclaration : public NStatement {
public:
    const NIdentifier& type;
    NIdentifier& id;
    NExpression *assignmentExpr;
    NVariableDeclaration(const NIdentifier& type, NIdentifier& id) :
        type(type), id(id) { }
    NVariableDeclaration(const NIdentifier& type, NIdentifier& id, NExpression *assignmentExpr) :
        type(type), id(id), assignmentExpr(assignmentExpr) { }
    virtual void printNode(int depth){
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "var_decl: " << std::endl;
        type.printNode(depth+1);
        id.printNode(depth+1);
        if(assignmentExpr)assignmentExpr->printNode(depth+1);
    }
};

class NFunctionDeclaration : public NStatement {
public:
    const NIdentifier& type;
    const NIdentifier& id;
    VariableList arguments;
    NBlock& block;
    NFunctionDeclaration(const NIdentifier& type, const NIdentifier& id, 
            const VariableList& arguments, NBlock& block) :
        type(type), id(id), arguments(arguments), block(block) { }
    virtual void printNode(int depth){
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "fun_decl: " << std::endl;
        type.printNode(depth+1);
        id.printNode(depth+1);
        for(auto i : arguments){
            i->printNode(depth+1);
            block.printNode(depth+1);
        }
    }
};