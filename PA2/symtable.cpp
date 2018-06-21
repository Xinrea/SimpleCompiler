#include "symtable.h"

symtable::symtable(/* args */)
{
}

symtable::~symtable()
{
}

symbol* symtable::get(const string& name, int level){
    if(!table[name])return nullptr;
    symbol* head = table[name];
    if(head->level==level){
        return head;
    }
    while(head->next){
        head = head->next;
        if(head->level==level){
            return head;
        }
    }
    return nullptr;
}

int symtable::addSymbol(const string& name, int level, stype type, int paranum, const vector<stype>& paratype){
    symbol* newsym = new symbol;
    if(newsym == nullptr)return -1;
    newsym->name = name;
    newsym->level = level;
    newsym->type = type;
    newsym->paranum = paranum;
    newsym->paratype = paratype;
    newsym->next = nullptr;
    symbol* head = table[name];
    /* already exist, need to check conflicts */
    if(head){
        if(head->level==level){
            return -1;
        }
        while(head->next){
            head = head->next;
            if(head->level==level){
                return -1;
            }
        }
        head->next = newsym;
    }
    else table[name] = newsym;
    return 0;
}

void symtable::show(){
    for(auto i : table){
        if(i.second == nullptr)continue;
        symbol* head = i.second;
        printf("L:%d T:%d I:%s PN:%d PT:",head->level,head->type,head->name.c_str(),head->paranum);
        for(auto k : head->paratype){
            printf("%d ",k);
        }
        printf("\n");
    }
}