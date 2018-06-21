#include "symtable.h"

symtable::symtable(/* args */)
{
}

symtable::~symtable()
{
}

symbol* symtable::get(const string& name, int level){
    if(!table[name])return nullptr;
    symbol* res = nullptr;
    int nearest = level;
    symbol* head = table[name];
    if(head->level<=level){
        res = head;
        nearest = level;
    }
    while(head->next){
        head = head->next;
        if(head->level<=nearest){
            head = head->next;
            nearest = head->level;
        }
    }
    return res;
}

void symtable::remove(int level){
    for(auto& i : table){
        if(i.second == nullptr)continue;
        symbol* head = i.second;
        if(head->level==level){
            i.second = head->next;
            delete head;
            head = i.second;
        }
        symbol* temp;
        if(head == nullptr)continue;
        while(head->next){
            temp = head;
            head = head->next;
            if(head->level==level){
                temp->next = head->next;
                delete head;
                head = temp;
            }
        }
    }
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
    return;
    printf("Symbol-Table\n");
    for(auto i : table){
        if(i.second == nullptr)continue;
        symbol* head = i.second;
        printf("Level:%d\tType:%d\tIden:%s\tParaNum:%d\tParaType:",head->level,head->type,head->name.c_str(),head->paranum);
        for(auto k : head->paratype){
            printf("%d ",k);
        }
        printf("\n");
        while(head->next){
            head = head->next;
            printf("Level:%d\tType:%d\tIden:%s\tParaNum:%d\tParaType:",head->level,head->type,head->name.c_str(),head->paranum);
            for(auto k : head->paratype){
                printf("%d ",k);
            }
            printf("\n");
        }
    }
}