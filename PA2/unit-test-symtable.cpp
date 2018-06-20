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
            printf("%s:%d: expect: \"format\" actual: \"format\"\n",__FILE__,__LINE__,expect,actual);\
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

static void test_table_get(){
    symtable t;
    EXPECT_NULL(t.get("init")); /* after init, all content should be null */
}

static void test_symtable(){
    test_table_get();
}

int main(int argc, char const *argv[])
{
    test_symtable();
    float pass_rate = (float)test_pass/test_count;
    std::cout << test_pass << "/" << test_count << " pass_rate: " << pass_rate << std::endl;
    return 0;
}
