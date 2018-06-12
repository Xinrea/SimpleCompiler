/*
    本程序在遍历语法树中，完成了符号表的创建、删除等操作，符号表使用的是一个顺序表，
实际操作中还可以用hash表的不同形式。注意不同的作用域符号表的开始和结束删除的时机。
    这个程序是一个很不完整的语义检查，只是开了一个头，目前只具有下列功能的一部分：
    1. 检查变量重复定义；
    2. 未定义的变量使用；
    3. 变量类型不匹配的一种情况；
    4. 计算外部变量在数据区的偏移量；局部变量偏移量还未处理
仅作参考，希望大家看后，能知道从什么地方下手进行语义分析。
有关属性的计算，中间代码的生成，可以用播放的方式参考PPT:"语法树的遍历（符号表与中间代码）"以及第8章课件
*/


#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "def.h"
#include "parser.tab.h"

#define MAXLENGTH   200

struct symbol {  //这里只列出了一个符号表项的部分属性，没考虑属性间的互斥
    char name[33];   //变量或函数名
    int level;       //层号
    int type;        //变量类型或函数返回值类型
    int  paramnum;    //形式参数个数
    char alias[10];  //别名，为解决嵌套层次使用
    char flag;       //符号标记，函数：'F'  变量：'V'   参数：'P'
    char offset;    //外部变量和局部变量在其静态数据区或活动记录中的偏移量，目标代码生成时使用
    //函数入口...
    };

struct symboltable{
    struct symbol symbols[MAXLENGTH];
    int index;
    } symbolTable;

struct symbol_scope_begin {
//当前作用域的符号在符号表的起始位置序号,这是一个栈结构
    int TX[30];
    int top;
    } symbol_scope_TX;


char *newAlias()
{
static int no=1;
static char alias[10];
char s[10];
strcpy(alias,"v");
itoa(no++,s,10);
strcat(alias,s);
return alias;
}

char *newLabel()
{
static int no=1;
static char label[10];
char s[10];
strcpy(label,"label");
itoa(no++,s,10);
strcat(label,s);
return label;
}

void semantic_error(int line,char *msg1,char *msg2)
{   //这里可以只收集错误信息，最后在一次显示
    printf("在%d行,%s %s\n",line,msg1,msg2);
}
void prn_symbol()
{
int i=0;
printf("%6s %6s %6s  %6s %4s %6s\n","变量名","别 名","层 号","类  型","标记","偏移量");
for(i=0;i<symbolTable.index;i++)
    printf("%6s %6s %6d  %6s %4c %6d\n",symbolTable.symbols[i].name,symbolTable.symbols[i].alias,\
       symbolTable.symbols[i].level,\
       symbolTable.symbols[i].type==INT?"int":"float",\
       symbolTable.symbols[i].flag,symbolTable.symbols[i].offset);
}
int searchSymbolTable(char *name)
{
int i;
for(i=symbolTable.index-1;i>=0;i--)
    if (!strcmp(symbolTable.symbols[i].name, name))  return i;
return -1;
}
int fillSymbolTable(char *name,char *alias,int level,int type,char flag,int offset)
{
//首先根据name查符号表，不能重复定义 重复定义返回-1
int i;
/*符号查重，考虑外部变量声明前有函数定义，
其形参名还在符号表中，这时的外部变量与前函数的形参重名是允许的*/
for(i=symbolTable.index-1;symbolTable.symbols[i].level==level||(level==0 && i>=0);i--)
    {
    if (level==0 && symbolTable.symbols[i].level==1) continue;  //外部变量和形参不必比较重名
    if (!strcmp(symbolTable.symbols[i].name, name))  return -1;
    }
//填写符号表内容
strcpy(symbolTable.symbols[symbolTable.index].name,name);
strcpy(symbolTable.symbols[symbolTable.index].alias,alias);
symbolTable.symbols[symbolTable.index].level=level;
symbolTable.symbols[symbolTable.index].type=type;
symbolTable.symbols[symbolTable.index].flag=flag;
symbolTable.symbols[symbolTable.index].offset=offset;
symbolTable.index++;
return symbolTable.index-1; //返回的是符号在符号表中的位置序号，中间代码生成时可用序号取到符号别名
}

int LEV=0;

void ext_var_list(struct node *T){  //处理变量列表
    int rtn,num=1;
    switch (T->kind){
        case EXT_DEC_LIST:
                T->ptr[0]->type=T->type;              //将类型属性向下传递变量结点
                T->ptr[0]->offset=T->offset;          //外部变量的偏移量向下传递
                T->ptr[1]->offset=T->offset+T->width; //外部变量的偏移量向下传递
                T->ptr[1]->width=T->width;
                ext_var_list(T->ptr[0]);
                ext_var_list(T->ptr[1]);
                T->num=T->ptr[1]->num+1;
                break;
        case ID:
            rtn=fillSymbolTable(T->type_id,newAlias(),LEV,T->type,'V',T->offset);  //最后一个变量名
            if (rtn==-1)
                semantic_error(T->pos,T->type_id, "变量重复定义");
            else T->place=rtn;
            T->num=1;
            break;
        }
    }

int  match_param(int i,struct node *T){
    int j,num=symbolTable.symbols[i].paramnum;
    int type1,type2;
    if (num==0 && T==NULL) return 1;
    for (j=1;j<num;j++) {
        type1=symbolTable.symbols[i+j].type;  //形参类型
        if (T->kind!=ARGS){
            semantic_error(T->pos,"", "函数调用参数太少");
            return 0;
        }
        type2=T->ptr[0]->type;
        if (type1!=type2){
            semantic_error(T->pos,"", "参数类型不匹配");
            return 0;
        }
        T=T->ptr[1];
    }
    type1=symbolTable.symbols[i+num].type;  //形参类型
    if (T->kind==ARGS){
        semantic_error(T->pos,"", "函数调用参数太多");
        return 0;
    }
    type2=T->type;
    if (type1!=type2){
        printf("num=%d，type1=%stype2=%s\n",num,type1==INT?"int":"float",type2==INT?"int":"float");
        semantic_error(T->pos,"", "参数类型不匹配");
        return 0;
    }
    return 1;
    }

void semantic_Analysis(struct node *T)
{//对抽象语法树的先根遍历,按display的控制结构修改完成符号表管理和语义检查
  int rtn,num;
  struct node *T0;
  if (T)
	{
	switch (T->kind) {
	case EXT_DEF_LIST:  if (T->ptr[0]){
                            T->ptr[0]->offset=T->offset;
                            }
                        semantic_Analysis(T->ptr[0]);    //访问外部定义列表中的第一个
                        if (T->ptr[1]){
                            T->ptr[1]->offset=T->ptr[0]->offset+T->ptr[0]->width;
                            }
                        semantic_Analysis(T->ptr[1]);    //访问该外部定义列表中的其它外部定义
                        break;
	case EXT_VAR_DEF:   //处理外部说明,将第一个孩子(TYPE结点)中的类型送到第二个孩子的类型域
                        T->type=T->ptr[1]->type=!strcmp(T->ptr[0]->type_id,"int")?INT:FLOAT;
                        T->ptr[1]->offset=T->offset;        //这个外部变量的偏移量向下传递
                        T->ptr[1]->width=T->type==INT?2:4;  //将一个变量的宽度向下传递
                        ext_var_list(T->ptr[1]);
                        T->width=(T->type==INT?2:4)* T->ptr[1]->num;
                        break;
	case FUNC_DEF:      //填写符号表
                        T->ptr[1]->type=!strcmp(T->ptr[0]->type_id,"int")?INT:FLOAT;//获取函数返回类型送到含函数名、参数的结点
                        T->width=0;
                        semantic_Analysis(T->ptr[1]);     //处理函数名和参数结点部分
                        semantic_Analysis(T->ptr[2]);     //处理函数体结点
                        break;                        break;
	case FUNC_DEC:      //根据返回类型，函数名填写符号表
                        rtn=fillSymbolTable(T->type_id,newAlias(),LEV,T->type,'F',0);//此处偏移量未计算，暂时为0
                        if (rtn==-1)
                                semantic_error(T->pos,T->type_id, "函数重复定义");
                        else T->place=rtn;
                        semantic_Analysis(T->ptr[0]);  //处理函数参数列表
                        if (T->ptr[0]) //T->ptr[0]->num 参数个数需要写到符号表中
                             symbolTable.symbols[rtn].paramnum=T->ptr[0]->num;
                        else symbolTable.symbols[rtn].paramnum=0;
                        break;
	case PARAM_LIST:    //处理函数形式参数列表
                        semantic_Analysis(T->ptr[0]);
                        semantic_Analysis(T->ptr[1]);
                        T->num=T->ptr[0]->num+T->ptr[1]->num;  //统计参数个数
                        break;
	case  PARAM_DEC:    rtn=fillSymbolTable(T->ptr[1]->type_id,newAlias(),1,T->ptr[0]->type,'P',0);//此处偏移量未计算，暂时为0
                        if (rtn==-1)
                            semantic_error(T->ptr[1]->pos,T->ptr[1]->type_id, "参数名重复定义");
                        else T->ptr[1]->place=rtn;
                        T->num=1;       //参数个数计算的初始值
                        break;
	case COMP_STM:      LEV++;
                        //设置层号加1，并且保存该层局部变量在符号表中的起始位置在symbol_scope_TX
                        symbol_scope_TX.TX[symbol_scope_TX.top++]=symbolTable.index;
                        semantic_Analysis(T->ptr[0]);  //处理该层的局部变量DEF_LIST
                        semantic_Analysis(T->ptr[1]);  //处理复合语句的语句序列
         //               prn_symbol();       //c在退出一个符合语句前显示的符号表
                        LEV--;    //出复合语句，层号减1
                        symbolTable.index=symbol_scope_TX.TX[--symbol_scope_TX.top]; //删除该作用域中的符号
                        break;
    case DEF_LIST:      semantic_Analysis(T->ptr[0]);   //处理一个局部变量定义
                        semantic_Analysis(T->ptr[1]);   //处理剩下的局部变量定义
                        break;
    case VAR_DEF:       //处理一个局部变量定义,将第一个孩子(TYPE结点)中的类型送到第二个孩子的类型域
                        //类似于上面的外部变量EXT_VAR_DEF，换了一种处理方法
                        T->ptr[1]->type=!strcmp(T->ptr[0]->type_id,"int")?INT:FLOAT;  //确定变量序列各变量类型
                        T0=T->ptr[1];  //T0为变量名列表子树根指针，对ID、ASSIGNOP类结点在登记到符号表，作为局部变量
                        while (T0->kind==DEC_LIST) {
                            T0->ptr[0]->type=T0->type;  //类型属性向下传递
                            T0->ptr[1]->type=T0->type;
                            if (T0->ptr[0]->kind==ID){
                                rtn=fillSymbolTable(T0->ptr[0]->type_id,newAlias(),LEV,T0->ptr[0]->type,'V',0);//此处偏移量未计算，暂时为0
                                if (rtn==-1)
                                     semantic_error(T0->ptr[0]->pos,T0->ptr[0]->type_id, "变量重复定义");
                                else T->ptr[0]->place=rtn;
                                }
                            else if (T0->ptr[0]->kind==ASSIGNOP){
                                rtn=fillSymbolTable(T0->ptr[0]->ptr[0]->type_id,newAlias(),LEV,T0->ptr[0]->type,'V',0);//此处偏移量未计算，暂时为0
                                if (rtn==-1)
                                     semantic_error(T0->ptr[0]->ptr[0]->pos,T0->ptr[0]->ptr[0]->type_id, "变量重复定义");
                                else T->ptr[0]->place=rtn;
                                }
                            T0=T0->ptr[1];
                            }
                        if (T0->kind==ID){
                                rtn=fillSymbolTable(T0->type_id,newAlias(),LEV,T0->type,'V',0);//此处偏移量未计算，暂时为0
                                if (rtn==-1)
                                     semantic_error(T0->pos,T0->type_id, "变量重复定义");
                                else T->place=rtn;
                                }
                        else if (T0->kind==ASSIGNOP){
                                rtn=fillSymbolTable(T0->ptr[0]->type_id,newAlias(),LEV,T0->type,'V',0);//此处偏移量未计算，暂时为0
                                if (rtn==-1)
                                     semantic_error(T0->ptr[0]->pos,T0->ptr[0]->type_id, "变量重复定义");
                                else T->ptr[0]->place=rtn;
                                //后续处理T0->ptr[1]的表达式，完成初始化赋值
                                }
                        break;
	case STM_LIST:      semantic_Analysis(T->ptr[0]);
                        semantic_Analysis(T->ptr[1]);
                        break;
	case IF_THEN:       semantic_Analysis(T->ptr[0]);      //条件
                        semantic_Analysis(T->ptr[1]);      //if子句
                        break;
	case IF_THEN_ELSE:  semantic_Analysis(T->ptr[0]);      //条件
                        semantic_Analysis(T->ptr[1]);      //if子句
                        semantic_Analysis(T->ptr[2]);      //子句
                        break;
	case WHILE:         semantic_Analysis(T->ptr[0]);      //循环条件
                        semantic_Analysis(T->ptr[1]);      //循环题
                        break;
    case EXP_STMT:      semantic_Analysis(T->ptr[0]);
                        T->code=T->ptr[0]->code;
                        break;
	case RETURN:        semantic_Analysis(T->ptr[0]);
                        //代码：T->ptr[0].code||返回 T->ptr[0]结点的值，
                        break;
	case ID:	        //查符号表，获得符号表中的位置，类型送type
                        rtn=searchSymbolTable(T->type_id);
                        if (rtn==-1)
                            semantic_error(T->pos,T->type_id, "变量未定义");
                        if (symbolTable.symbols[rtn].flag=='F')
                            semantic_error(T->pos,T->type_id, "是函数名，类型不匹配");
                        else {
                            T->place=rtn;       //结点保存变量在符号表中的位置
                            T->type=symbolTable.symbols[rtn].type;
                            }
                        break;
	case ASSIGNOP:  if (T->ptr[0]->kind!=ID)
                        semantic_error(T->pos,"", "赋值语句需要左值");
                    else {
                          semantic_Analysis(T->ptr[0]);
                          semantic_Analysis(T->ptr[1]);
                          T->type=T->ptr[0]->type;
                         }
                    break;
	case AND:
	case OR:
	case RELOP:     //下面的类型属性计算，没有考虑错误处理情况
                    T->type=INT;
                    break;
	case PLUS:
	case MINUS:
	case STAR:
	case DIV:       semantic_Analysis(T->ptr[0]);
                    semantic_Analysis(T->ptr[1]);
                    //判断T->ptr[0]，T->ptr[1]类型是否正确，可能根据运算符生成不同形式的代码，给T的type赋值
                    //下面的类型属性计算，没有考虑错误处理情况
                    if (T->ptr[0]->type==FLOAT || T->ptr[1]->type==FLOAT)
                         T->type=FLOAT;
                    else T->type=INT;
                    break;
	case NOT:       T->type=INT;
                    break;
	case UMINUS:    semantic_Analysis(T->ptr[0]);
                    T->type=T->ptr[0]->type;
                    //判断T->ptr[0]类型是否正确，生成不同形式的代码，给T的type赋值
                    break;
    case FUNC_CALL: //根据T->type_id查出函数的定义，如果语言中增加了实验教材的read，write需要单独处理一下
                    rtn=searchSymbolTable(T->type_id);
                    if (rtn==-1){
                          semantic_error(T->pos,T->type_id, "函数未定义");
                          break;
                        }
                    if (symbolTable.symbols[rtn].flag!='F'){
                           semantic_error(T->pos,T->type_id, "不是一个函数");
                           break;
                        }
                    semantic_Analysis(T->ptr[0]); //处理所以实参表达式求值，及类型
                    match_param(rtn,T->ptr[0]);   //处理所以参数的匹配
                    break;
    case ARGS:      semantic_Analysis(T->ptr[0]);
                    semantic_Analysis(T->ptr[1]);
                    break;
         }
      }
}

void semantic_Analysis0(struct node *T)
{
    symbolTable.index=0;
    symbol_scope_TX.TX[0]=0;  //外部变量在符号表中的起始序号为0
    symbol_scope_TX.top=1;
    T->offset=0;             //外部变量在数据区的偏移量
    semantic_Analysis(T);
 //   prn_symbol();
 }
