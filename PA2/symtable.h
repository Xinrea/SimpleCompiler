#pragma once

#include <map>
#include <vector>
#include <string>

using std::vector;
using std::map;
using std::string;

enum stype {T_INT,T_FLOAT,T_CHAR,T_BOOL,T_FUNC};

/* Basic struct of single record */
struct symbol
{
    string name;
    int level;
    stype type;
    int paranum;
    vector<stype> paratype;
    symbol* next;
};

/* The main class, define how to manage symtable */
class symtable
{
private:
    map<string,symbol*> table;
public:
    symtable(/* args */);
    ~symtable();
    symbol* get(const string& name, int level);
    static void error(int line, const string& msg1, const string& msg2){
        printf("Line: %d Error: %s %s\n", line, msg1.c_str(), msg2.c_str());
    }
    int addSymbol(const string& name, int level, stype type, int paranum, const vector<stype>& paratype);
    void show();
};
