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
    virtual void printNode(int depth) const = 0;
};

class NExpression : public Node {
public:
    void printNode(int depth) const {}
};

class NStatement : public Node {
public:
    void printNode(int depth) const {}
};

class NInteger : public NExpression {
public:
    long long value;
    NInteger(long long value) : value(value) { }
    void printNode(int depth) const{
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
    void printNode(int depth) const{
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
    void printNode(int depth) const{
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
    void printNode(int depth) const{
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Func_Call: " << std::endl;
        id.printNode(depth+1);
        for(auto i : arguments){
            i->printNode(depth+1);
        }
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "end" << std::endl;
    }
};

class NBinaryOperator : public NExpression {
public:
    int op;
    NExpression& lhs;
    NExpression& rhs;
    NBinaryOperator(NExpression& lhs, int op, NExpression& rhs) :
        lhs(lhs), rhs(rhs), op(op) { }
    void printNode(int depth) const{
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Operator_exp: " << op << std::endl;
        lhs.printNode(depth+1);
        rhs.printNode(depth+1);
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "end" << std::endl;
    }
};

class NSelfLOperator : public NExpression {
public:
    int op;
    NExpression& hs;
    NSelfLOperator(int op, NExpression& hs) :
        hs(hs),op(op) { }
    void printNode(int depth) const{
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "LOperator_exp: " << op << std::endl;
        hs.printNode(depth+1);
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "end" << std::endl;
    }
};

class NSelfROperator : public NExpression {
public:
    int op;
    NExpression& hs;
    NSelfROperator(NExpression& hs,int op) :
        hs(hs),op(op) { }
    void printNode(int depth) const{
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "ROperator_exp: " << op << std::endl;
        hs.printNode(depth+1);
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "end" << std::endl;
    }
};

class NAssignment : public NExpression {
public:
    int optype;
    NIdentifier& lhs;
    NExpression& rhs;
    NAssignment(int optype, NIdentifier& lhs, NExpression& rhs) : 
        optype(optype), lhs(lhs), rhs(rhs) { }
    void printNode(int depth) const{
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Assignment_exp: " << optype << std::endl;
        lhs.printNode(depth+1);
        rhs.printNode(depth+1);
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Assignment end" << std::endl;
    }
};

class NBlock : public NExpression {
public:
    StatementList statements;
    NBlock() { }
    void printNode(int depth) const{
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Block: " << std::endl;
        for(auto i : statements){
            i->printNode(depth+1); 
        }
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Block end" << std::endl;
    }
};

class NExpressionStatement : public NStatement {
public:
    NExpression& expression;
    NExpressionStatement(NExpression& expression) : 
        expression(expression) { }
    void printNode(int depth) const{
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Exp: " << std::endl;
        expression.printNode(depth+1);
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Exp end" << std::endl;
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
    void printNode(int depth) const{
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Var_decl: " << std::endl;
        type.printNode(depth+1);
        id.printNode(depth+1);
        if(assignmentExpr){
            for(int i=0;i<depth+1;++i){
                std::cout << " ";
            }
            std::cout << "Assignment: " << std::endl;
            assignmentExpr->printNode(depth+2);
        }
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Decl end" << std::endl;
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
    void printNode(int depth) const{
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Fun_decl: " << std::endl;
        type.printNode(depth+1);
        id.printNode(depth+1);
        for(int i=0;i<depth+1;++i){
            std::cout << " ";
        }
        std::cout << "Para: " << std::endl;
        for(auto i : arguments){
            i->printNode(depth+2);
        }
        for(int i=0;i<depth+1;++i){
            std::cout << " ";
        }
        std::cout << "Para end" << std::endl;
        block.printNode(depth+1);
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Decl end " << std::endl;
    }
};

class NIfstatement : public NStatement {
public:
    const NExpression& condition;
    const NBlock& block;
    const NBlock& elseBlock;
    NIfstatement(const NExpression& condition,const NBlock& block,const NBlock& elseBlock):condition(condition),block(block),elseBlock(elseBlock){}
    void printNode(int depth) const{
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "IfStat:" << std::endl;
        for(int i=0;i<depth+1;++i){
            std::cout << " ";
        }
        std::cout << "cond:" << std::endl;
        condition.printNode(depth+2);
        for(int i=0;i<depth+1;++i){
            std::cout << " ";
        }
        std::cout << "cond end" << std::endl;
        block.printNode(depth+1);
        for(int i=0;i<depth+1;++i){
            std::cout << " ";
        }
        std::cout << "else:" << std::endl;
        elseBlock.printNode(depth+2);
        for(int i=0;i<depth+1;++i){
            std::cout << " ";
        }
        std::cout << "else end" << std::endl;
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "IfStat end" << std::endl;
    }
};

class NWhilestatement : public NStatement {
public:
    const NExpression& condition;
    const NBlock& block;
    NWhilestatement(const NExpression& condition,const NBlock& block):condition(condition),block(block){}
    void printNode(int depth) const{
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "WhileStat:" << std::endl;
        for(int i=0;i<depth+1;++i){
            std::cout << " ";
        }
        std::cout << "cond:" << std::endl;
        condition.printNode(depth+2);
        for(int i=0;i<depth+1;++i){
            std::cout << " ";
        }
        std::cout << "cond end" << std::endl;
        block.printNode(depth+1);
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "While end" << std::endl;
    }
};