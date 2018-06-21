#pragma once
#include <iostream>
#include <vector>
#include "symtable.h"

class CodeGenContext;
class NStatement;
class NExpression;
class NVariableDeclaration;

static symtable t;

typedef std::vector<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;
typedef std::vector<NVariableDeclaration*> VariableList;

class Node {
public:
    virtual void printNode(int depth) const = 0;
    virtual void semantics(int depth) const = 0;
};

class NExpression : public Node {
public:
    void printNode(int depth) const {}
    void semantics(int depth) const {}
};

class NStatement : public Node {
public:
    void printNode(int depth) const {}
    void semantics(int depth) const {}
};

class NInteger : public NExpression {
public:
    int line;
    long long value;
    NInteger(long long value, int line) : value(value),line(line) { }
    void printNode(int depth) const{
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Const-Integer: " << value << std::endl;
    }
    void semantics(int depth) const {
        return;
    }
};

class NDouble : public NExpression {
public:
    int line;
    double value;
    NDouble(double value, int line) : value(value),line(line) { }
    void printNode(int depth) const{
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Const-Float: " << value << std::endl;
    }
    void semantics(int depth) const {
        return;
    }
};

class NIdentifier : public NExpression {
public:
    int line;
    std::string name;
    NIdentifier(const std::string& name, int line) : name(name),line(line) { }
    void printNode(int depth) const{
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        std::cout << "Identifier: " << name << std::endl;
    }
    void semantics(int depth) const {
        symbol* ret = t.get(name,depth);
        if(ret == nullptr)symtable::error(line,0,name);
        else if(ret->paranum != -1)symtable::error(line,6,name);
        return;
    }
};

class NMethodCall : public NExpression {
public:
    int line;
    const NIdentifier& id;
    ExpressionList arguments;
    NMethodCall(const NIdentifier& id, ExpressionList& arguments, int line) :
        id(id), arguments(arguments), line(line) { }
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
    void semantics(int depth) const {
        symbol* ret = t.get(id.name,depth);
        if(ret == nullptr){
            symtable::error(line,1,id.name);
            return;
        }
        if(arguments.size()!=ret->paranum){
            symtable::error(line,2,"");
        }
        for(auto i : arguments){
            i->semantics(depth+1);
        }
        return;
    }
};

class NBinaryOperator : public NExpression {
public:
    int line;
    int op;
    NExpression& lhs;
    NExpression& rhs;
    NBinaryOperator(NExpression& lhs, int op, NExpression& rhs, int line) :
        lhs(lhs), rhs(rhs), op(op), line(line) { }
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
    void semantics(int depth) const {
        lhs.semantics(depth);
        rhs.semantics(depth);
        return;
    }
};

class NSelfLOperator : public NExpression {
public:
    int line;
    int op;
    NExpression& hs;
    NSelfLOperator(int op, NExpression& hs, int line) :
        hs(hs),op(op), line(line) { }
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
    void semantics(int depth) const {
        hs.semantics(depth);
        return;
    }
};

class NSelfROperator : public NExpression {
public:
    int line;
    int op;
    NExpression& hs;
    NSelfROperator(NExpression& hs,int op, int line) :
        hs(hs),op(op), line(line) { }
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
    void semantics(int depth) const {
        hs.semantics(depth);
        return;
    }
};

class NAssignment : public NExpression {
public:
    int line;
    int optype;
    NIdentifier& lhs;
    NExpression& rhs;
    NAssignment(int optype, NIdentifier& lhs, NExpression& rhs, int line) : 
        optype(optype), lhs(lhs), rhs(rhs), line(line) { }
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
    void semantics(int depth) const {
        lhs.semantics(depth);
        rhs.semantics(depth);
        return;
    }
};

class NBlock : public NExpression {
public:
    int line;
    StatementList statements;
    NBlock(int line):line(line) { }
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
    void semantics(int depth) const {
        for(auto i : statements){
            i->semantics(depth);
        }
        t.show();
    }
};

class NExpressionStatement : public NStatement {
public:
    int line;
    NExpression& expression;
    NExpressionStatement(NExpression& expression, int line) : 
        expression(expression), line(line) { }
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
    void semantics(int depth) const {
        expression.semantics(depth);
        return;
    }
};

class NVariableDeclaration : public NStatement {
public:
    int line;
    const NIdentifier& type;
    NIdentifier& id;
    NExpression *assignmentExpr;
    NVariableDeclaration(const NIdentifier& type, NIdentifier& id, int line) :
        type(type), id(id), line(line-1) { }
    NVariableDeclaration(const NIdentifier& type, NIdentifier& id, NExpression *assignmentExpr, int line) :
        type(type), id(id), assignmentExpr(assignmentExpr), line(line-1) { }
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
    void semantics(int depth) const {
        int ret;
        if(type.name == "Int"){
            ret = t.addSymbol(id.name,depth,T_INT,-1,{});
            if(ret == -1)symtable::error(line, 3, id.name);
            return;
        }
        if(type.name == "Float"){
            ret = t.addSymbol(id.name,depth,T_FLOAT,-1,{});
            if(ret == -1)symtable::error(line, 3, id.name);
            return;
        }
        if(type.name == "Char"){
            ret = t.addSymbol(id.name,depth,T_CHAR,-1,{});
            if(ret == -1)symtable::error(line, 3, id.name);
            return;
        }
        if(type.name == "Bool"){
            ret = t.addSymbol(id.name,depth,T_BOOL,-1,{});
            if(ret == -1)symtable::error(line, 3, id.name);
            return;
        }
        symtable::error(line, 4, type.name);
        return;
    }
};

class NFunctionDeclaration : public NStatement {
public:
    int line;
    const NIdentifier& type;
    const NIdentifier& id;
    VariableList arguments;
    NBlock& block;
    NFunctionDeclaration(const NIdentifier& type, const NIdentifier& id, 
            const VariableList& arguments, NBlock& block, int line) :
        type(type), id(id), arguments(arguments), block(block), line(line) { }
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
    void semantics(int depth) const {
        int ret;
        vector<stype> paratypelist;
        for(int i = 0;i < arguments.size(); ++i){
            if(arguments[i]->type.name == "Int")paratypelist.push_back(T_INT);
            else if(arguments[i]->type.name == "Float")paratypelist.push_back(T_FLOAT);
            else if(arguments[i]->type.name == "Char")paratypelist.push_back(T_CHAR);
            else if(arguments[i]->type.name == "Bool")paratypelist.push_back(T_BOOL);
        }
        if(type.name == "Int"){
            ret = t.addSymbol(id.name,depth,T_INT,arguments.size(),paratypelist);
            if(ret == -1)symtable::error(line, 3, id.name);
            for(auto i : arguments){
                i->semantics(depth+1);
            }
            block.semantics(depth+1);
        }
        else if(type.name == "Float"){
            ret = t.addSymbol(id.name,depth,T_FLOAT,arguments.size(),paratypelist);
            if(ret == -1)symtable::error(line, 3, id.name);
            for(auto i : arguments){
                i->semantics(depth+1);
            }
            block.semantics(depth+1);
        }
        else if(type.name == "Char"){
            ret = t.addSymbol(id.name,depth,T_CHAR,arguments.size(),paratypelist);
            if(ret == -1)symtable::error(line, 3, id.name);
            for(auto i : arguments){
                i->semantics(depth+1);
            }
            block.semantics(depth+1);
        }
        else if(type.name == "Bool"){
            ret = t.addSymbol(id.name,depth,T_BOOL,arguments.size(),paratypelist);
            if(ret == -1)symtable::error(line, 3, id.name);
            for(auto i : arguments){
                i->semantics(depth+1);
            }
            block.semantics(depth+1);
        }
        else symtable::error(line, 4, type.name);
        if(!t.return_flag)symtable::error(line, 5, id.name);
        t.remove(depth+1);
        return;
    }
};

class NIfstatement : public NStatement {
public:
    int line;
    const NExpression& condition;
    const NBlock& block;
    const NBlock& elseBlock;
    NIfstatement(const NExpression& condition,const NBlock& block,const NBlock& elseBlock, int line):condition(condition),block(block),elseBlock(elseBlock),line(line){}
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
    void semantics(int depth) const {
        condition.semantics(depth+1);
        block.semantics(depth+1);
        t.remove(depth+1);
    }
};

class NWhilestatement : public NStatement {
public:
    int line;
    const NExpression& condition;
    const NBlock& block;
    NWhilestatement(const NExpression& condition,const NBlock& block, int line):condition(condition),block(block), line(line){}
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
    void semantics(int depth) const {
        condition.semantics(depth+1);
        block.semantics(depth+1);
        t.remove(depth+1);
    }
};

class NKeywordstatement : public NStatement {
public:
    int line;
    int keyword;
    const NExpression& exp;
    NKeywordstatement(int keyword, const NExpression& exp, int line):keyword(keyword),exp(exp), line(line){}
    void printNode(int depth) const{
        for(int i=0;i<depth;++i){
            std::cout << " ";
        }
        printf("Keystat: %d\n",keyword);
        exp.printNode(depth+1);
    }
    void semantics(int depth) const {
        exp.semantics(depth);
        t.return_flag = true;
    }
};