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
    bool return_flag = false;
    symtable(/* args */);
    ~symtable();
    symbol* get(const string& name, int level);
    void remove(int level);
    static void error(int line, int error, const string& name){
        switch(error){
            case 0:{
                printf("Line: %d Error: Undefined Identifier: %s\n", line, name.c_str());
                break;
            }
            case 1:{
                printf("Line: %d Error: Undefined Function: %s\n", line, name.c_str());
                break;
            }
            case 2:{
                printf("Line: %d Error: Unmatched ParaNumber %s\n", line, name.c_str());
                break;
            }
            case 3:{
                printf("Line: %d Error: Redefined Identifier: %s\n", line, name.c_str());
                break;
            }
            case 4:{
                printf("Line: %d Error: Undefined Type: %s\n", line, name.c_str());
                break;
            }
            case 5:{
                printf("Line: %d Error: Function Without Return: %s\n", line, name.c_str());
                break;
            }
            case 6:{
                printf("Line: %d Error: Function Is Not Variable:  %s\n", line, name.c_str());
                break;
            }
        }
        
    }
    int addSymbol(const string& name, int level, stype type, int paranum, const vector<stype>& paratype);
    void show();
};
