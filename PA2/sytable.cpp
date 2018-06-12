/*
    �������ڱ����﷨���У�����˷��ű�Ĵ�����ɾ���Ȳ��������ű�ʹ�õ���һ��˳���
ʵ�ʲ����л�������hash��Ĳ�ͬ��ʽ��ע�ⲻͬ����������ű�Ŀ�ʼ�ͽ���ɾ����ʱ����
    ���������һ���ܲ������������飬ֻ�ǿ���һ��ͷ��Ŀǰֻ�������й��ܵ�һ���֣�
    1. �������ظ����壻
    2. δ����ı���ʹ�ã�
    3. �������Ͳ�ƥ���һ�������
    4. �����ⲿ��������������ƫ�������ֲ�����ƫ������δ����
�����ο���ϣ����ҿ�����֪����ʲô�ط����ֽ������������
�й����Եļ��㣬�м��������ɣ������ò��ŵķ�ʽ�ο�PPT:"�﷨���ı��������ű����м���룩"�Լ���8�¿μ�
*/


#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "def.h"
#include "parser.tab.h"

#define MAXLENGTH   200

struct symbol {  //����ֻ�г���һ�����ű���Ĳ������ԣ�û�������Լ�Ļ���
    char name[33];   //����������
    int level;       //���
    int type;        //�������ͻ�������ֵ����
    int  paramnum;    //��ʽ��������
    char alias[10];  //������Ϊ���Ƕ�ײ��ʹ��
    char flag;       //���ű�ǣ�������'F'  ������'V'   ������'P'
    char offset;    //�ⲿ�����;ֲ��������侲̬����������¼�е�ƫ������Ŀ���������ʱʹ��
    //�������...
    };

struct symboltable{
    struct symbol symbols[MAXLENGTH];
    int index;
    } symbolTable;

struct symbol_scope_begin {
//��ǰ������ķ����ڷ��ű����ʼλ�����,����һ��ջ�ṹ
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
{   //�������ֻ�ռ�������Ϣ�������һ����ʾ
    printf("��%d��,%s %s\n",line,msg1,msg2);
}
void prn_symbol()
{
int i=0;
printf("%6s %6s %6s  %6s %4s %6s\n","������","�� ��","�� ��","��  ��","���","ƫ����");
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
//���ȸ���name����ű������ظ����� �ظ����巵��-1
int i;
/*���Ų��أ������ⲿ��������ǰ�к������壬
���β������ڷ��ű��У���ʱ���ⲿ������ǰ�������β������������*/
for(i=symbolTable.index-1;symbolTable.symbols[i].level==level||(level==0 && i>=0);i--)
    {
    if (level==0 && symbolTable.symbols[i].level==1) continue;  //�ⲿ�������ββ��رȽ�����
    if (!strcmp(symbolTable.symbols[i].name, name))  return -1;
    }
//��д���ű�����
strcpy(symbolTable.symbols[symbolTable.index].name,name);
strcpy(symbolTable.symbols[symbolTable.index].alias,alias);
symbolTable.symbols[symbolTable.index].level=level;
symbolTable.symbols[symbolTable.index].type=type;
symbolTable.symbols[symbolTable.index].flag=flag;
symbolTable.symbols[symbolTable.index].offset=offset;
symbolTable.index++;
return symbolTable.index-1; //���ص��Ƿ����ڷ��ű��е�λ����ţ��м��������ʱ�������ȡ�����ű���
}

int LEV=0;

void ext_var_list(struct node *T){  //��������б�
    int rtn,num=1;
    switch (T->kind){
        case EXT_DEC_LIST:
                T->ptr[0]->type=T->type;              //�������������´��ݱ������
                T->ptr[0]->offset=T->offset;          //�ⲿ������ƫ�������´���
                T->ptr[1]->offset=T->offset+T->width; //�ⲿ������ƫ�������´���
                T->ptr[1]->width=T->width;
                ext_var_list(T->ptr[0]);
                ext_var_list(T->ptr[1]);
                T->num=T->ptr[1]->num+1;
                break;
        case ID:
            rtn=fillSymbolTable(T->type_id,newAlias(),LEV,T->type,'V',T->offset);  //���һ��������
            if (rtn==-1)
                semantic_error(T->pos,T->type_id, "�����ظ�����");
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
        type1=symbolTable.symbols[i+j].type;  //�β�����
        if (T->kind!=ARGS){
            semantic_error(T->pos,"", "�������ò���̫��");
            return 0;
        }
        type2=T->ptr[0]->type;
        if (type1!=type2){
            semantic_error(T->pos,"", "�������Ͳ�ƥ��");
            return 0;
        }
        T=T->ptr[1];
    }
    type1=symbolTable.symbols[i+num].type;  //�β�����
    if (T->kind==ARGS){
        semantic_error(T->pos,"", "�������ò���̫��");
        return 0;
    }
    type2=T->type;
    if (type1!=type2){
        printf("num=%d��type1=%stype2=%s\n",num,type1==INT?"int":"float",type2==INT?"int":"float");
        semantic_error(T->pos,"", "�������Ͳ�ƥ��");
        return 0;
    }
    return 1;
    }

void semantic_Analysis(struct node *T)
{//�Գ����﷨�����ȸ�����,��display�Ŀ��ƽṹ�޸���ɷ��ű�����������
  int rtn,num;
  struct node *T0;
  if (T)
	{
	switch (T->kind) {
	case EXT_DEF_LIST:  if (T->ptr[0]){
                            T->ptr[0]->offset=T->offset;
                            }
                        semantic_Analysis(T->ptr[0]);    //�����ⲿ�����б��еĵ�һ��
                        if (T->ptr[1]){
                            T->ptr[1]->offset=T->ptr[0]->offset+T->ptr[0]->width;
                            }
                        semantic_Analysis(T->ptr[1]);    //���ʸ��ⲿ�����б��е������ⲿ����
                        break;
	case EXT_VAR_DEF:   //�����ⲿ˵��,����һ������(TYPE���)�е������͵��ڶ������ӵ�������
                        T->type=T->ptr[1]->type=!strcmp(T->ptr[0]->type_id,"int")?INT:FLOAT;
                        T->ptr[1]->offset=T->offset;        //����ⲿ������ƫ�������´���
                        T->ptr[1]->width=T->type==INT?2:4;  //��һ�������Ŀ�����´���
                        ext_var_list(T->ptr[1]);
                        T->width=(T->type==INT?2:4)* T->ptr[1]->num;
                        break;
	case FUNC_DEF:      //��д���ű�
                        T->ptr[1]->type=!strcmp(T->ptr[0]->type_id,"int")?INT:FLOAT;//��ȡ�������������͵����������������Ľ��
                        T->width=0;
                        semantic_Analysis(T->ptr[1]);     //���������Ͳ�����㲿��
                        semantic_Analysis(T->ptr[2]);     //����������
                        break;                        break;
	case FUNC_DEC:      //���ݷ������ͣ���������д���ű�
                        rtn=fillSymbolTable(T->type_id,newAlias(),LEV,T->type,'F',0);//�˴�ƫ����δ���㣬��ʱΪ0
                        if (rtn==-1)
                                semantic_error(T->pos,T->type_id, "�����ظ�����");
                        else T->place=rtn;
                        semantic_Analysis(T->ptr[0]);  //�����������б�
                        if (T->ptr[0]) //T->ptr[0]->num ����������Ҫд�����ű���
                             symbolTable.symbols[rtn].paramnum=T->ptr[0]->num;
                        else symbolTable.symbols[rtn].paramnum=0;
                        break;
	case PARAM_LIST:    //��������ʽ�����б�
                        semantic_Analysis(T->ptr[0]);
                        semantic_Analysis(T->ptr[1]);
                        T->num=T->ptr[0]->num+T->ptr[1]->num;  //ͳ�Ʋ�������
                        break;
	case  PARAM_DEC:    rtn=fillSymbolTable(T->ptr[1]->type_id,newAlias(),1,T->ptr[0]->type,'P',0);//�˴�ƫ����δ���㣬��ʱΪ0
                        if (rtn==-1)
                            semantic_error(T->ptr[1]->pos,T->ptr[1]->type_id, "�������ظ�����");
                        else T->ptr[1]->place=rtn;
                        T->num=1;       //������������ĳ�ʼֵ
                        break;
	case COMP_STM:      LEV++;
                        //���ò�ż�1�����ұ���ò�ֲ������ڷ��ű��е���ʼλ����symbol_scope_TX
                        symbol_scope_TX.TX[symbol_scope_TX.top++]=symbolTable.index;
                        semantic_Analysis(T->ptr[0]);  //����ò�ľֲ�����DEF_LIST
                        semantic_Analysis(T->ptr[1]);  //�����������������
         //               prn_symbol();       //c���˳�һ���������ǰ��ʾ�ķ��ű�
                        LEV--;    //��������䣬��ż�1
                        symbolTable.index=symbol_scope_TX.TX[--symbol_scope_TX.top]; //ɾ�����������еķ���
                        break;
    case DEF_LIST:      semantic_Analysis(T->ptr[0]);   //����һ���ֲ���������
                        semantic_Analysis(T->ptr[1]);   //����ʣ�µľֲ���������
                        break;
    case VAR_DEF:       //����һ���ֲ���������,����һ������(TYPE���)�е������͵��ڶ������ӵ�������
                        //������������ⲿ����EXT_VAR_DEF������һ�ִ�����
                        T->ptr[1]->type=!strcmp(T->ptr[0]->type_id,"int")?INT:FLOAT;  //ȷ���������и���������
                        T0=T->ptr[1];  //T0Ϊ�������б�������ָ�룬��ID��ASSIGNOP�����ڵǼǵ����ű���Ϊ�ֲ�����
                        while (T0->kind==DEC_LIST) {
                            T0->ptr[0]->type=T0->type;  //�����������´���
                            T0->ptr[1]->type=T0->type;
                            if (T0->ptr[0]->kind==ID){
                                rtn=fillSymbolTable(T0->ptr[0]->type_id,newAlias(),LEV,T0->ptr[0]->type,'V',0);//�˴�ƫ����δ���㣬��ʱΪ0
                                if (rtn==-1)
                                     semantic_error(T0->ptr[0]->pos,T0->ptr[0]->type_id, "�����ظ�����");
                                else T->ptr[0]->place=rtn;
                                }
                            else if (T0->ptr[0]->kind==ASSIGNOP){
                                rtn=fillSymbolTable(T0->ptr[0]->ptr[0]->type_id,newAlias(),LEV,T0->ptr[0]->type,'V',0);//�˴�ƫ����δ���㣬��ʱΪ0
                                if (rtn==-1)
                                     semantic_error(T0->ptr[0]->ptr[0]->pos,T0->ptr[0]->ptr[0]->type_id, "�����ظ�����");
                                else T->ptr[0]->place=rtn;
                                }
                            T0=T0->ptr[1];
                            }
                        if (T0->kind==ID){
                                rtn=fillSymbolTable(T0->type_id,newAlias(),LEV,T0->type,'V',0);//�˴�ƫ����δ���㣬��ʱΪ0
                                if (rtn==-1)
                                     semantic_error(T0->pos,T0->type_id, "�����ظ�����");
                                else T->place=rtn;
                                }
                        else if (T0->kind==ASSIGNOP){
                                rtn=fillSymbolTable(T0->ptr[0]->type_id,newAlias(),LEV,T0->type,'V',0);//�˴�ƫ����δ���㣬��ʱΪ0
                                if (rtn==-1)
                                     semantic_error(T0->ptr[0]->pos,T0->ptr[0]->type_id, "�����ظ�����");
                                else T->ptr[0]->place=rtn;
                                //��������T0->ptr[1]�ı��ʽ����ɳ�ʼ����ֵ
                                }
                        break;
	case STM_LIST:      semantic_Analysis(T->ptr[0]);
                        semantic_Analysis(T->ptr[1]);
                        break;
	case IF_THEN:       semantic_Analysis(T->ptr[0]);      //����
                        semantic_Analysis(T->ptr[1]);      //if�Ӿ�
                        break;
	case IF_THEN_ELSE:  semantic_Analysis(T->ptr[0]);      //����
                        semantic_Analysis(T->ptr[1]);      //if�Ӿ�
                        semantic_Analysis(T->ptr[2]);      //�Ӿ�
                        break;
	case WHILE:         semantic_Analysis(T->ptr[0]);      //ѭ������
                        semantic_Analysis(T->ptr[1]);      //ѭ����
                        break;
    case EXP_STMT:      semantic_Analysis(T->ptr[0]);
                        T->code=T->ptr[0]->code;
                        break;
	case RETURN:        semantic_Analysis(T->ptr[0]);
                        //���룺T->ptr[0].code||���� T->ptr[0]����ֵ��
                        break;
	case ID:	        //����ű���÷��ű��е�λ�ã�������type
                        rtn=searchSymbolTable(T->type_id);
                        if (rtn==-1)
                            semantic_error(T->pos,T->type_id, "����δ����");
                        if (symbolTable.symbols[rtn].flag=='F')
                            semantic_error(T->pos,T->type_id, "�Ǻ����������Ͳ�ƥ��");
                        else {
                            T->place=rtn;       //��㱣������ڷ��ű��е�λ��
                            T->type=symbolTable.symbols[rtn].type;
                            }
                        break;
	case ASSIGNOP:  if (T->ptr[0]->kind!=ID)
                        semantic_error(T->pos,"", "��ֵ�����Ҫ��ֵ");
                    else {
                          semantic_Analysis(T->ptr[0]);
                          semantic_Analysis(T->ptr[1]);
                          T->type=T->ptr[0]->type;
                         }
                    break;
	case AND:
	case OR:
	case RELOP:     //������������Լ��㣬û�п��Ǵ��������
                    T->type=INT;
                    break;
	case PLUS:
	case MINUS:
	case STAR:
	case DIV:       semantic_Analysis(T->ptr[0]);
                    semantic_Analysis(T->ptr[1]);
                    //�ж�T->ptr[0]��T->ptr[1]�����Ƿ���ȷ�����ܸ�����������ɲ�ͬ��ʽ�Ĵ��룬��T��type��ֵ
                    //������������Լ��㣬û�п��Ǵ��������
                    if (T->ptr[0]->type==FLOAT || T->ptr[1]->type==FLOAT)
                         T->type=FLOAT;
                    else T->type=INT;
                    break;
	case NOT:       T->type=INT;
                    break;
	case UMINUS:    semantic_Analysis(T->ptr[0]);
                    T->type=T->ptr[0]->type;
                    //�ж�T->ptr[0]�����Ƿ���ȷ�����ɲ�ͬ��ʽ�Ĵ��룬��T��type��ֵ
                    break;
    case FUNC_CALL: //����T->type_id��������Ķ��壬���������������ʵ��̲ĵ�read��write��Ҫ��������һ��
                    rtn=searchSymbolTable(T->type_id);
                    if (rtn==-1){
                          semantic_error(T->pos,T->type_id, "����δ����");
                          break;
                        }
                    if (symbolTable.symbols[rtn].flag!='F'){
                           semantic_error(T->pos,T->type_id, "����һ������");
                           break;
                        }
                    semantic_Analysis(T->ptr[0]); //��������ʵ�α��ʽ��ֵ��������
                    match_param(rtn,T->ptr[0]);   //�������Բ�����ƥ��
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
    symbol_scope_TX.TX[0]=0;  //�ⲿ�����ڷ��ű��е���ʼ���Ϊ0
    symbol_scope_TX.top=1;
    T->offset=0;             //�ⲿ��������������ƫ����
    semantic_Analysis(T);
 //   prn_symbol();
 }
