#include <iostream>
#include <cstdio>
#include "symtable.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ(equality, expect, actual,format) \
    do {\
        test_count++;\
        if(equality)test_pass++;\
        else {\
            printf("%s:%d: expect: " format " actual: " format "\n",__FILE__,__LINE__,expect,actual);\
        }\
    }while(0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ((expect)==(actual),expect,actual,"%d")
#define EXPECT_NULL(actual) \
    do {\
        test_count++;\
        if((actual)==nullptr)test_pass++;\
        else {\
            printf("%s:%d: expect: nullptr actual: \"%p\"\n",__FILE__,__LINE__,actual);\
        }\
    }while(0)
#define EXPECT_NOT_NULL(actual) \
    do {\
        test_count++;\
        if(!((actual)==nullptr))test_pass++;\
        else {\
            printf("%s:%d: expect: nullptr actual: \"%p\"\n",__FILE__,__LINE__,actual);\
        }\
    }while(0)

static void test_table_get(){
    printf("# Get Test\n");
    symtable t;
    symbol* ret = t.get("init",0);
    EXPECT_NULL(ret); /* after init, all pointer should be null */
    t.addSymbol("test",0,T_FUNC,2,{T_INT, T_INT});
    ret = t.get("test",0);
    EXPECT_NOT_NULL(ret);
    ret = t.get("test",1);
    EXPECT_NOT_NULL(ret);
}

static void test_table_add(){
    printf("# Add Test\n");
    symtable t;
    string name = "test";
    int level = 1;
    stype type = T_FUNC;
    int paranum = 2;
    vector<stype> paratype = {T_INT, T_FLOAT};
    /* fisrt symbol */
    int ret = t.addSymbol(name,level,type,paranum,paratype);
    EXPECT_EQ_INT(0,ret);
    /* add conflict symbol */
    ret = t.addSymbol(name,level,type,paranum,paratype);
    EXPECT_EQ_INT(-1,ret);
    /* add same name but non-conflict symbol */
    ret = t.addSymbol(name,2,type,paranum,paratype);
    EXPECT_EQ_INT(0,ret);
}

static void test_table_remove(){
    printf("# Remove Test\n");
    symtable t;
    symbol* ret;
    t.addSymbol("test",0,T_FUNC,2,{T_INT, T_INT});
    t.addSymbol("test2",1,T_FUNC,2,{T_INT, T_INT});
    t.addSymbol("test3",1,T_FUNC,2,{T_INT, T_INT});
    t.show();
    t.remove(1);
    t.show();
    ret = t.get("test",2);
    EXPECT_EQ_INT(0,ret->level);
}

static void test_symtable(){
    test_table_get();
    test_table_add();
    test_table_remove();
}

int main(int argc, char const *argv[])
{
    printf("======START-TEST======\n");
    test_symtable();
    float pass_rate = (float)test_pass/test_count;
    printf("# Result of Test\n");
    std::cout << test_pass << "/" << test_count << " pass_rate: " << pass_rate << std::endl;
    printf("=======END-TEST=======\n");
    return 0;
}
