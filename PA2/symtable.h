#pragma once

#include <map>
#include <vector>
#include <string>

using std::vector;
using std::map;
using std::string;

/* Basic struct of single record */
struct symbol
{
    string name[32];
    int level;
    int type;
    int paranum;
    char flag;
    char offset;
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
    symbol* get(const string& name);
    static void error(int line, const string& msg1, const string& msg2){
        printf("Line: %d Error: %s %s", line, msg1.c_str(), msg2.c_str());
    }
    int addSymbol(const string& name, int level, int type, int paranum, char flag, char offset);
};
