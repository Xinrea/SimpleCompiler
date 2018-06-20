#include "symtable.h"

symtable::symtable(/* args */)
{
}

symtable::~symtable()
{
}

symbol* symtable::get(const string& name){
    return table[name];
}

int symtable::addSymbol(const string& name, int level, int type, int paranum, char flag, char offset){

}