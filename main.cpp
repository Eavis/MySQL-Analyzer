#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include"lexanal.h"
#include <fstream>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include"ScanTable.h"
using namespace std;
#define MAX_STRING_LEN 512
enum TYPE {PLUS, MINUS, MULT,DIV,EQ,LT,STRING_CONST,INT_ATTR,FLOAT_ATTR,CHAR_ATTR,GT,EQSS,EQLT};
char RELOP[] = "< <= > >= = !=";
char inp[10000];    //for user input command
char token[100];    // Output for lexanal( )
int  token_code;    // Output for lexanal( )
int  num_of_char;   //number of character in the whole input command
char *p = inp;      //moving pointer
char *t = inp;   //temporary pointer  use to realize the peek function
char rel_dup[20];
int freq =0;
int offset=0;
int validflag = -1;
ScanTable *f;
int nrels;//num of relations
int wildflag = 0;

void printToken( int code, char *token );
int DoSelect(int Nrel,int Crel);
char *lex_anal(char *current_pos, char *token, int *token_type);
//double eval_expr( struct MyNODE *op);
class MyNODE
{
public:
    int type;	/* Can be one of FLOAT, PLUS, MINUS, MULT, DIV */
    union
    {
        int   i;                  /* INT_CONST */
        float f;                  /* FLOAT_CONST */
        char  c[100];             /* STRING_CONST */
        int   * i_attr;           /* Attribute of type INT */
        double * f_attr;           /* Attribute of type FLOAT */
        char  * c_attr;           /* Attribute of type CHAR */
        struct MyNODE * p[2];       /* Points to operands of a
                                     binary arithmetic, relational
                                     or logic operation. */
        struct MyNODE * q;          /* Points to operand of a
                                     uniary arithmetic (e.g.: -x)
                                     or logic (e.g.: NOT x) operation */
    } value;// Value of the node
    //    struct SELECT_DB *select;
    double eval_expr();
    char *eval_string(){
        if(type == STRING_CONST){
            return value.c;
        }
        if(type == CHAR_ATTR){
            return value.c_attr;
        }
        return NULL;
    };
    
};
class SELECT_DB
{
public:
    
    int NAttrs;
    // int a[100];
    char AttrName[100][32];
    int NExprs;//number of expressions in select clause
    MyNODE *Select[100];
    //use an array of Nodes store the list of expressions in the SELECT-claus
    int Nrels;
    char RelName[20][32];
    char AliasName[20][32];
    ScanTable *Rel[10];// this is an array which contains ten scantable
    MyNODE *Where;
    int rel;
    int ambiguity;
    
    int selectall;
};
char *attr_start;
char *where_start;
SELECT_DB * slct;

struct MyNODE *E();
struct MyNODE *T();
struct MyNODE *F();
struct MyNODE *BE();
struct MyNODE *BT();
struct MyNODE *BF();
int main(int argc, const char * argv[])
{   struct DataDescription DataDes[10];
    int n_fields = 0;
    while (1){
        cout << "Please input your SQL command:" << endl;
        num_of_char = read_command(inp);
        p = inp;// p points to inp, where the input is stored
        while((p<inp+num_of_char)&&isspace(*p))
            p++;
        p = lex_anal(p, token, &token_code);
        printToken( token_code, token );
        if (token_code == CREATE)
        {
            FILE *fp_cata;
            n_fields=0;
            int num_of_attr = 0;
            int sub_nfields=1;
            if ((fp_cata = fopen("catalog", "a+"))== NULL)
            {
                cout << "Error: cannot open database catalog `./catalog'" << endl;
                return 0;
            }
            while(p<inp+num_of_char){
                while((p<inp+num_of_char)&&isspace(*p))
                    p++;
                p = lex_anal(p, token, &token_code);
             //   printToken( token_code, token );
                if(token_code!=TABLE){
                    printf("error:table not found");
                }
                while((p<inp+num_of_char)&&isspace(*p))
                    p++;
                
                p = lex_anal(p, token, &token_code);
              //  printToken( token_code, token );
                if(token_code!=IDENTIFIER){
                    printf("error:table name not found\n");
                }
                strcpy(DataDes[n_fields].relname,token);
                FILE *file = fopen(token, "r" );
                if(file){
                    cout<<"Error:table exists!"<<endl;
                    break;
                }else{
                    file = fopen(token,"w");
                    fclose(file);
                }
                while((p<inp+num_of_char)&&isspace(*p))
                    p++;
                p = lex_anal(p, token, &token_code);
                //printToken( token_code, token );
                if(strcmp(token,"")==0){
                    cout<<"create table has been done"<<endl;
                    return 0;
                }
                
                if(token_code!=(int)('(')){
                    printf("error:( not found");
                }
                do{
                    while((p<inp+num_of_char)&&isspace(*p))
                        p++;
                    p = lex_anal(p, token, &token_code);
                    //printToken( token_code, token );
                    if(token_code!=IDENTIFIER){
                        printf("error:attribute name");
                    }
                    //store the first attribute name
                    strcpy(DataDes[n_fields].fieldname,token);
                    num_of_attr++;
                    while((p<inp+num_of_char)&&isspace(*p))
                        p++;
                    p = lex_anal(p, token, &token_code);
                    //printToken( token_code, token );
                    if(token_code==INT){
                        DataDes[n_fields].fieldtype='I';
                        DataDes[n_fields].fieldsize=sizeof(int);
                        while (DataDes[n_fields].startpos%4!=0) {
                            DataDes[n_fields].startpos++;
                        }
                    }else if(token_code==FLOAT){
                        DataDes[n_fields].fieldtype='F';
                        DataDes[n_fields].fieldsize=sizeof(double);
                        while (DataDes[n_fields].startpos%8!=0) {
                            DataDes[n_fields].startpos++;
                        }
                    }else if(token_code==CHAR){
                        DataDes[n_fields].fieldtype='C';
                        while((p<inp+num_of_char)&&isspace(*p))
                            p++;
                        p = lex_anal(p, token, &token_code);
                        //printToken( token_code, token );
                        if(token_code!=(int)('(')){
                            printf("Error  `(` expected");
                        }
                        while((p<inp+num_of_char)&&isspace(*p))
                            p++;
                        p = lex_anal(p, token, &token_code);
                        //printToken( token_code, token );
                        DataDes[n_fields].fieldsize=atoi(token)+1;
                        if(token_code!=INTEGER_NUMBER){
                            printf("Error:integer expected");
                        }
                        while((p<inp+num_of_char)&&isspace(*p))
                            p++;
                        p = lex_anal(p, token, &token_code);
                        //printToken( token_code, token );
                        if(token_code!=(int)(')')){
                            printf("Error  `)` expected");
                        }
                    }
                    if(n_fields==0){
                        DataDes[0].startpos=0;
                        DataDes[sub_nfields].startpos=DataDes[n_fields].startpos+DataDes[n_fields].fieldsize;
                        strcpy(DataDes[sub_nfields].relname,DataDes[n_fields].relname);
                        sub_nfields++;
                        n_fields++;
                    }else{
                        strcpy(DataDes[sub_nfields].relname,DataDes[n_fields].relname);
                        DataDes[sub_nfields].startpos=DataDes[n_fields].startpos+DataDes[n_fields].fieldsize;
                        sub_nfields++;
                        n_fields++;
                    }
                    while((p<inp+num_of_char)&&isspace(*p))
                        p++;
                    p = lex_anal(p, token, &token_code);
                    //printToken( token_code, token );
                }while(token_code==(int)(','));
                if(token_code!=(int)(')')){
                    printf("Error  `)` expected");
                }
                p=inp+num_of_char;
                int i;
                for(i=0;i<num_of_attr;i++){
                    fprintf(fp_cata, "%s %s %c %d %d\n",
                            DataDes[i].relname,
                            DataDes[i].fieldname,
                            DataDes[i].fieldtype,
                            DataDes[i].startpos,
                            DataDes[i].fieldsize);
                }
                fclose(fp_cata);
            }
        }else if(token_code==DROP){
            cout<<"drop"<<endl;
            FILE * catalog;
            FILE * catabuffer;
            catabuffer = fopen ("catabuffer","w");
            char line[100];
            char linebuffer[100];
            int signal=0;
//            ifstream fin( "catalog" );
            ifstream fin;
            fin.open ("catalog");
            char dup[20];
            while(p<inp+num_of_char){
                while((p<inp+num_of_char)&&isspace(*p))
                    p++;
                p = lex_anal(p, token, &token_code);
                if(token_code!=TABLE){
                    printf("error:table not found");
                }
                while((p<inp+num_of_char)&&isspace(*p))
                    p++;
                p = lex_anal(p, token, &token_code);
//                printToken( token_code, token );
                if(token_code!=IDENTIFIER){
                    printf("error:table name not found\n");
                }
                if(strcpy(dup,token)==0){
                    cout<<"rel_dup failed"<<endl;
                }
                remove(token);
                
                while ( fin.getline(line,100) )
                {
                    strcpy(linebuffer,"");
                    char *linepointer=line;
                    for(;isspace(*linepointer)==0;linepointer++)
                    {
                        char buffer[10]={*linepointer};
                        strcat(linebuffer,buffer);
                    }
                    if(strcmp(linebuffer,dup)!=0)
                    {
                        cout<<"look at here"<<endl;
                        fprintf(catabuffer,"%s", line);
                        fprintf(catabuffer,"\n");
                    }
                    else
                        signal=1;
                }
                fin.close();
                fclose (catabuffer);
                
                if (signal==0)
                {
                    cout<<"output:"<<endl<<"The table '"<<dup<<"' does not exist"<<endl;
                    remove("catabuffer");
                    return 1;
                }
                catalog = fopen ("catalog","w");
                ifstream bin( "catabuffer" );
                char bine[100];
                while ( bin.getline(bine,100) )
                {
                    fprintf(catalog,"%s", bine);
                    fprintf(catalog,"\n");
                }
                bin.close();
                fclose (catalog);
                remove(dup);
                remove("catabuffer");
                
                cout<<"output:"<<endl<<"1. The 'catalog' file now contains:"<<endl;
                ifstream in( "catalog" );
                string s;
                while ( getline(in,s) )
                {
                    cout << s << endl;
                }
                in.close();
                cout<<"2. The file '"<<dup<<"' is deleted"<<endl;
                p=inp+num_of_char;
            }
            
        }else if(token_code == INSERT){
            FILE *fp;
            int fd;
            char buf[1024];
            int record_size=0;
            //int record_size=0;
            int index=0;
            int input_int;
            int token_len;
            double input_double;
            // char buf[1024];
            //char ans;
            while(p<inp+num_of_char){
                while((p<inp+num_of_char)&&isspace(*p))
                    p++;
                p = lex_anal(p, token, &token_code);
                //printToken( token_code, token );
                if(token_code!=INTO){
                    printf("error:table not found");
                    break;
                }
                while((p<inp+num_of_char)&&isspace(*p))
                    p++;
                p = lex_anal(p, token, &token_code);
                //printToken( token_code, token );
                if(token_code!=TABLE){
                    printf("error: not found");
                    break;
                }
                while((p<inp+num_of_char)&&isspace(*p))
                    p++;
                p = lex_anal(p, token, &token_code);
                //printToken( token_code, token);
                if(token_code!=IDENTIFIER){
                    printf("error:table name not found\n");
                    break;
                }
                fd =open(token,O_CREAT|O_RDWR|O_APPEND, 0666);
                fp = fopen("catalog", "r");
                n_fields = 0;
                while ( fscanf(fp, "%s %s %c %d %d",
                               DataDes[n_fields].relname,
                               DataDes[n_fields].fieldname,
                               &DataDes[n_fields].fieldtype,
                               &DataDes[n_fields].startpos,
                               &DataDes[n_fields].fieldsize) > 0 )
                {
                    if ( strcmp( DataDes[n_fields].relname, token ) == 0 )
                    {
                        if ( DataDes[n_fields].startpos + DataDes[n_fields].fieldsize >
                            record_size )
                        {
                            record_size = DataDes[n_fields].startpos
                            + DataDes[n_fields].fieldsize;
                        }
                        
                        n_fields++;
                    }
                }
                if ( n_fields == 0 )
                {
                    printf("Relation not found !\n");
                    exit(1);
                }
                while((p<inp+num_of_char)&&isspace(*p))
                    p++;
                p = lex_anal(p, token, &token_code);
                //printToken( token_code, token);
                if(token_code!=VALUES){
                    printf("error:key word values not found\n");
                    break;
                }
                while((p<inp+num_of_char)&&isspace(*p))
                    p++;
                p = lex_anal(p, token, &token_code);
                //printToken( token_code, token );
                if(token_code!=(int)('(')){
                    printf("error:( not found");
                    break;
                }
                do{
                    if(index>n_fields){
                        printf("Error: Too many values!");
                        exit(1);
                    }
                    while((p<inp+num_of_char)&&isspace(*p))
                        p++;
                    p = lex_anal(p, token, &token_code);
                    token_len = (int)(strlen(token));
                    if(DataDes[index].fieldtype=='I'){
                        if(token_code==INTEGER_NUMBER||token_code==FLOAT_NUMBER){
                            input_int = atoi(token);
                            memcpy(&buf[DataDes[index].startpos],&input_int,sizeof(int));
                        }else{
                            printf("Error: Integer number expected!\n");
                            exit(1);
                        }
                    }else if(DataDes[index].fieldtype=='F'){
                        if(token_code==INTEGER_NUMBER||token_code==FLOAT_NUMBER){
                            input_double= atof(token);
                            memcpy(&buf[DataDes[index].startpos],&input_double,sizeof(double));
                        }else{
                            printf("Error: Float or int number expected!\n");
                            exit(1);
                        }
                    }else if(DataDes[index].fieldtype=='C'){
                        
                        if(token_code==STRING){
                            if(token_len>DataDes[index].fieldsize-1){
                                strncpy(&buf[DataDes[index].startpos],token, DataDes[index].fieldsize-1);
                            }else{
                                strcpy(&buf[DataDes[index].startpos],token);
                            }
                        }else{
                            printf("Error: Stringexpected!\n");
                            exit(1);
                        }
                    }else{
                        printf("Error: The type is not expected.\n");
                        exit(1);
                    }
                    index++;
                    while((p<inp+num_of_char)&&isspace(*p))
                        p++;
                    p = lex_anal(p, token, &token_code);
                }while(token_code==(int)(','));
                if(index<n_fields){
                    printf("Error: The argument is too few.\n");
                    exit(1);
                }
                if(token_code!=(int)(')')){
                    printf("error:) not found");
                    exit(1);
                }
                buf[record_size] = 'Y';	// Valid
                write(fd,buf,record_size+1);
                close(fd);
                p=inp+num_of_char;
            }
        }else if(token_code == DELETE){
            FILE * emptyfile;
            struct MyNODE *node;
            double result;
            string N = "N";
            f = new ScanTable();
            if(f==NULL){
                cout<<"scantable failed"<<endl;
                return 0;
            }
            while(p<inp+num_of_char){
                while((p<inp+num_of_char)&&isspace(*p))
                    p++;
                 p = lex_anal(p, token, &token_code);
//                 printToken(token_code, token);
                if(token_code!=TABLE){
                    printf("error:table not found");
                    break;
                }
                while((p<inp+num_of_char)&&isspace(*p))
                    p++;
                p = lex_anal(p, token, &token_code);
//                printToken(token_code, token);

                if(token_code!=IDENTIFIER){
                    printf("error:table name not found");
                    break;
                }
                if(strcpy(rel_dup,token)==0){
                    cout<<"rel_dup failed"<<endl;
                }
                if(f->Open(token)==0){
                    printf("error:cannot find the file");
                    break;
                }else{
                    cout<<"relation has been opened"<<endl;
                }
                p = lex_anal(p, token, &token_code);
//                printToken(token_code, token);
                if(strcmp(token,"")==0){//delete all
                    remove(rel_dup);
                    emptyfile = fopen(rel_dup,"w");
                    fclose(emptyfile);
                    cout<<"all tuples deleted"<<endl;
                    return 0;
                }
                 if(token_code!=WHERE){
                    cout<<"Error: WHERE EXPECTED"<<endl;
                    return 0;
                }
                //delete with where condition
                 emptyfile = fopen("tmp","w");
                fclose(emptyfile);
//                if(f->Open(rel_dup)==0){
//                    printf("error:cannot find the file");
//                    break;
//                }else{
//                    cout<<"relation has been opened"<<endl;
//                }
                node = BE();
                f->Close();
                
                if(f->Open(rel_dup)==0){
                    printf("error:table name not found");
                    break;
                }
                
                while(f->GetNext() != 0 ){
//                    cout<<"print_first"<<endl;
//                    f->PrintTuple();
                    if(node==NULL){
                        cout<<"BE() returned null"<<endl;
                        return 0;
                    }else{
                        result = node->eval_expr();
                        if(result ==0){
                            f->WriteTuple();
                        }
                    }
                }
                f->Close();
                remove(rel_dup);
                rename("tmp",rel_dup);
//                cout<<"tuple has been deleted"<<endl;
                
                
                if(f->Open(rel_dup)==0){
                    printf("error:table name not found");
                    break;
                }else{
//                    cout<<"relation open again to check"<<endl;
                }
                while(f->GetNext() != 0 ){
//                    cout<<"Tuples now in file"<<endl;
                    f->PrintTuple();
                }
                f->Close();
                p=inp+num_of_char;
            }
        }else if(token_code == SELECT){
            slct = new SELECT_DB();
            slct->Nrels=0;
            slct->NAttrs = 0;
            slct->selectall = 0;
            
            int i;
            for(i=0;i<100;i++){
                slct->Select[i]= new MyNODE();
            }
            
            while((p<inp+num_of_char)&&isspace(*p))
                p++;
            attr_start = p;
            while((t<inp+num_of_char)&&isspace(*t))
                t++;
            t = lex_anal(p, token, &token_code);
            if(token_code==(int)'*'){
                wildflag = 1;
            }
            
            do{
                while((p<inp+num_of_char)&&isspace(*p))
                    p++;
                p=lex_anal(p, token, &token_code);
            }while(token_code!=FROM);
            
            do{
                while((p<inp+num_of_char)&&isspace(*p))
                    p++;
                p=lex_anal(p, token, &token_code);
                if(token_code==IDENTIFIER){
                    strcpy(slct->RelName[slct->Nrels],token);
                }
                while((p<inp+num_of_char)&&isspace(*p))
                    p++;
                p=lex_anal(p, token, &token_code);
                if(token_code==IDENTIFIER){
                    strcpy(slct->AliasName[slct->Nrels],token);
                    while((p<inp+num_of_char)&&isspace(*p))
                        p++;
                    p=lex_anal(p, token, &token_code);
                    
                }else{
                    strcpy(slct->AliasName[slct->Nrels]," ");
                }
                slct->Nrels++;
                
                
            }while(token_code==(int)',');
            if(strcmp(token,"")==0){
//                cout<<"test here"<<endl;//select * from ..no where
                slct->selectall = 1;
                DoSelect(slct->Nrels,0);
                return 0;
            }
            
            while((p<inp+num_of_char)&&isspace(*p))
                p++;
            where_start = p;
            if(token_code!=WHERE){
                printf("ERROR:Where expected here\n");
//                return 0;
            }
            if(DoSelect(slct->Nrels,0)==0){
                return 0;
            }
            
        }//end of select
    }

    return 0;
}

int DoSelect(int Nrel,int Crel){
    double slct_result = 0;
    int i;
    slct->NAttrs=0;
    slct->Rel[Crel]=new ScanTable();
    
    
    if(Crel<Nrel){
        if((slct->Rel[Crel]->Open(slct->RelName[Crel]))==0){
            cout<<"Error: relation "<<slct->RelName[Crel]<<" not found!"<<endl;
            return 1;
        }else{
        }
        while(slct->Rel[Crel]->GetNext()!=0){
            DoSelect(Nrel, Crel+1);
        }
    }
    else{
            if(slct->selectall==0){//include where
                p =where_start;
                validflag = 1;
                slct->Where=BE();
                if(slct->Where==NULL){
                    printf("ERROR:boolean expresion failed\n");
                    return 1;
                }
                slct_result = (slct->Where)->eval_expr();
            }
            if(slct_result==1||slct->selectall==1){//slct->no where
                if(wildflag==1){//select *
                    for(i = 0; i<slct->Nrels;i++){
                     slct->Rel[i]->PrintTuple();
                    }
                    cout<<endl;
                    return 0;
                }
                p = attr_start;
                validflag =1;
                do{
                    slct->NAttrs++;
                    slct->Select[slct->NAttrs] = E();
                    if(slct->Select[slct->NAttrs]!=NULL){
                        if(slct->Select[slct->NAttrs]->type==STRING_CONST||slct->Select[slct->NAttrs]->type==CHAR_ATTR){
                            cout<<"'"<<slct->Select[slct->NAttrs]->eval_string()<<"'"<<" ";
                        }
                        else{
                            cout<<slct->Select[slct->NAttrs]->eval_expr()<<" ";
                        }
                    }
                    while((p<inp+num_of_char)&&isspace(*p))
                        p++;
                    p=lex_anal(p, token, &token_code);
                }while(token_code==(int)',');
                cout<<endl;
            }
        
    }
    return 0;
}

struct MyNODE *BE(){
    struct MyNODE *operand1,*op;
    operand1 = BT();
    if(operand1==NULL){
        cout<<"BE::operand1 is null"<<endl;
        return (NULL);
    }//pass
    while((t<inp+num_of_char)&&isspace(*t))
        t++;
    t = lex_anal(p, token, &token_code);
//    cout<<"BE_peek_after operand1 "<<endl;
//     printToken(token_code, token);
//    cout<<"BE_peek_after operand1 "<<endl;
    if(strcmp(token,"")==0){
//        cout<<"BE should return"<<endl;
        return operand1;
    }
    if(token_code==OR){
//        cout<<"BE_token=OR"<<endl;
        while((p<inp+num_of_char)&&isspace(*p))
            p++;
        p=lex_anal(p, token, &token_code);
//        printToken(token_code, token);
        op = (struct MyNODE*)malloc(sizeof(struct MyNODE));
        op->type = OR;
        op->value.p[0]=operand1;
        op->value.p[1]=BT();
        if(op->value.p[1]==NULL){
            cout<<"BE_OR_op is null"<<endl;
            return(NULL);
        }
        operand1 = op;
        return operand1;
    }
    return operand1;
}
struct MyNODE *BT(){
    struct MyNODE *operand1,*op;
    operand1 = BF();
    if(operand1==NULL){
        cout<<"BT_operand1_BF is null"<<endl;
        return (NULL);
    }else{
//        cout<<"BT not failed ------------------------nonononoo"<<endl;
    }
//    cout<<"BT_peek_after operand1 "<<endl;
    while((t<inp+num_of_char)&&isspace(*t))
        t++;
    t = lex_anal(p, token, &token_code);
//    printToken(token_code, token);
    if(strcmp(token,"")==0){
//        cout<<"BT should return"<<endl;
        return operand1;
    }
//    cout<<"BT_peek_after operand1 "<<endl;
    while(token_code==AND){
        
//        cout<<"BT_token=AND"<<endl;
        while((p<inp+num_of_char)&&isspace(*p))
            p++;
        p=lex_anal(p, token, &token_code);
//        printToken(token_code, token);
        op = (struct MyNODE*)malloc(sizeof(struct MyNODE));
        op->type = AND;
        op->value.p[0]=operand1;
        op->value.p[1]=BF();
        
//        cout<<"op result "<<op->eval_expr();
        if(op->value.p[1]==NULL){
            cout<<"BT_AND_op is null"<<endl;
            return(NULL);
        }else{
//            cout<<"see value.p[1]"<<op->value.p[1]<<endl;
//            cout<<"op->value.p[1]!=NULL"<<op->value.p[1]->eval_expr();
//            cout<<"look at here ~~!!!!!!!"<<endl;
        }
        operand1 = op;
//        cout<<"op result "<<operand1->eval_expr();
        
    }
    return operand1;
}
struct MyNODE * BF(){
    char *sub;
    struct MyNODE *operand1, *op;
    while((p<inp+num_of_char)&&isspace(*p))
        p++;
    sub = p;
//    cout<<"test3---------------"<<endl;
//    cout<<*sub<<endl;
//    cout<<"test4---------------"<<endl;
    while((t<inp+num_of_char)&&isspace(*t))
        t++;
    t = lex_anal(p, token, &token_code);
//    printToken(token_code, token);
    
    
    if(token_code==NOT){
//        cout<<"BF_token=NOT"<<endl;
        while((p<inp+num_of_char)&&isspace(*p))
            p++;
        p=lex_anal(p, token, &token_code);
//        printToken(token_code, token);
        operand1 = BF();
        if(operand1==NULL){
            cout<<"BF_NOT_operand1 is null"<<endl;
            return (NULL);
        }
        op = (struct MyNODE*)malloc(sizeof(struct MyNODE));
        op->type = NOT;
        op->value.q=operand1;
        return(op);
    }else if(token_code==(int)'('){
//        cout<<"right!!!!"<<endl;
        while((p<inp+num_of_char)&&isspace(*p))
            p++;
        p=lex_anal(p, token, &token_code);
        operand1 = BE();
        if(operand1!=NULL){
            p=lex_anal(p, token, &token_code);
//            printToken(token_code, token);
            if(token_code==(int)')'){
                return operand1;
            }
        }else{
            p = sub;
//            cout<<"==============see what p is here=================="<<*p<<"hello"<<endl;
            operand1 = E();
            if(operand1==NULL){
                cout<<"BF_NOT_operand1 is null"<<endl;
                return (NULL);
            }
            while((p<inp+num_of_char)&&isspace(*p))
                p++;
            p = lex_anal(p, token, &token_code);
//            printToken(token_code, token);
            if(strcmp(token,"")==0){
//                cout<<"E should return"<<endl;
                return operand1;
            }
            op = (struct MyNODE*)malloc(sizeof(struct MyNODE));
            if(strcmp(token,"=")==0){
                if(operand1->type==CHAR_ATTR||operand1->type==STRING){//PASS
                    op->type = EQSS;
                    op->value.p[0]=operand1;
                    op->value.p[1]=E();
                    return op;
                }else{
                    op->type = EQ;
                    op->value.p[0]=operand1;
                    op->value.p[1]=E();
                    return op;
                }
            }else if(strcmp(token,"<")==0){
                op->type = LT;
                op->value.p[0]=operand1;
                op->value.p[1]=E();
//                cout<<"BF:::relop without (_<"<<endl;
                return op;
            }else if(strcmp(token,"<=")==0){//PASS
                if(operand1->type==CHAR_ATTR||operand1->type==STRING){
                    op->type = EQLT;
                    op->value.p[0]=operand1;
                    op->value.p[1]=E();
                    return op;
                }else{
                    op->type = LT;
                    op->value.p[0]=operand1;
                    op->value.p[1]=E();
                    return op;
                }
            }else if(strcmp(token,">=")==0){
                op->type = GREATEROREQ;
                op->value.p[0]=operand1;
//                cout<<"WBF:::relop without (_>="<<endl;
                
                op->value.p[1]=E();
                return op;
            }else if(strcmp(token, ">")==0){
                op->type = GT;
                op->value.p[0]=operand1;
                op->value.p[1]=E();
//                cout<<"BF:::relop without (_>"<<endl;
                return op;
            }else if(strcmp(token, "!=")==0){
                op->type = NOTEQ;
                op->value.p[0]=operand1;
                op->value.p[1]=E();
//                cout<<"BF:::relop without (_!="<<endl;
                return op;
            }else{
//                cout<<"ttttttttttttttt"<<endl;
                return (NULL);
            }
        }
    }
    operand1 = E();
    if(operand1==NULL){
        cout<<"BF_NOT_operand1 is null"<<endl;
        return (NULL);
    }
    while((p<inp+num_of_char)&&isspace(*p))
        p++;
    p = lex_anal(p, token, &token_code);
//    printToken(token_code, token);
    if(strcmp(token,"")==0){
//        cout<<"E should return"<<endl;
        return operand1;
    }
    op = (struct MyNODE*)malloc(sizeof(struct MyNODE));
    if(strcmp(token,"=")==0){
//        cout<<"andandandandandadnadnadn"<<endl;
        if(operand1->type==CHAR_ATTR||operand1->type==STRING){//PASS
            op->type = EQSS;
            op->value.p[0]=operand1;
            op->value.p[1]=E();
            return op;
        }else{
            op->type = EQ;
            op->value.p[0]=operand1;
            op->value.p[1]=E();
            return op;
        }
    }else if(strcmp(token,"<")==0){
        op->type = LT;
        op->value.p[0]=operand1;
        op->value.p[1]=E();
//        cout<<"BF:::relop without (_<"<<endl;
        return op;
    }else if(strcmp(token,"<=")==0){//PASS
        if(operand1->type==CHAR_ATTR||operand1->type==STRING){
            op->type = EQLT;
            op->value.p[0]=operand1;
            op->value.p[1]=E();
            return op;
        }else{
            op->type = LT;
            op->value.p[0]=operand1;
            op->value.p[1]=E();
            return op;
        }
    }else if(strcmp(token,">=")==0){
        op->type = GREATEROREQ;
        op->value.p[0]=operand1;
//        cout<<"WBF:::relop without (_>="<<endl;
        
        op->value.p[1]=E();
        return op;
    }else if(strcmp(token, ">")==0){
        op->type = GT;
        op->value.p[0]=operand1;
        op->value.p[1]=E();
//        cout<<"BF:::relop without (_>"<<endl;
        return op;
    }else if(strcmp(token, "!=")==0){
        op->type = NOTEQ;
        op->value.p[0]=operand1;
        op->value.p[1]=E();
//        cout<<"BF:::relop without (_!="<<endl;
        return op;
    }else{
//        cout<<"ttttttttttttttt"<<endl;
        return (NULL);
    }
}

struct MyNODE* E(){
    struct MyNODE *operand1, *op;
    operand1 = T();
    if(operand1==NULL){
        cout<<"E_operand1 is null"<<endl;
        return (NULL);
    }else{
//        printf("prove____e is not null--------------\n");
    }
//    cout<<"E_peek_after operand1 "<<endl;
    while((t<inp+num_of_char)&&isspace(*t))
        t++;
    t = lex_anal(p, token, &token_code);
//    printToken(token_code, token);
    if(strcmp(token,"")==0){
//        cout<<"E should return"<<endl;
        return operand1;
    }
//    cout<<"E_peek_after operand1 "<<endl;
    while((token_code==(int)'+')||(token_code==(int)'-')){
        if(token_code==(int)'+'){
//            cout<<"E_token=+"<<endl;
            while((p<inp+num_of_char)&&isspace(*p))
                p++;
            p=lex_anal(p, token, &token_code);
//            printToken(token_code, token);
            op = (struct MyNODE *) malloc(sizeof(struct MyNODE));
            op->type = PLUS;
            op->value.p[0] = operand1;
            op->value.p[1] = T();
//            cout<<"op->value.p[1]"<<op->value.p[1]->eval_expr()<<endl;
            if (op->value.p[1] == NULL){
                cout<<"E_+_op is null"<<endl;
                return NULL;
            }
            operand1 = op;
        }else if(token_code==(int)'-'){
//            cout<<"E_token=-"<<endl;
            while((p<inp+num_of_char)&&isspace(*p))
                p++;
            p=lex_anal(p, token, &token_code);
//            printToken(token_code, token);
            op = (struct MyNODE *) malloc(sizeof(struct MyNODE));
            op->type = MINUS;
            op->value.p[0] = operand1;
            op->value.p[1] = T();
            if (op->value.p[1] == NULL){
//                cout<<"E_-_op is null"<<endl;
                return(NULL);		// Error
            }
            operand1 = op;
        }
        while((t<inp+num_of_char)&&isspace(*t))
            t++;
        t = lex_anal(p, token, &token_code);
//        printToken(token_code, token);
    }
    return operand1;
}
struct MyNODE* T(){
    struct MyNODE *operand1, *op;
    operand1 = F();
    if(operand1==NULL){
//        cout<<"T_operand1 is null"<<endl;
        return (NULL);
    }
//    cout<<" T_peek_after operand1 "<<endl;
    while((t<inp+num_of_char)&&isspace(*t))
        t++;
    t = lex_anal(p, token, &token_code);////////////ROKEN = 0
//    printToken(token_code, token);
    if(strcmp(token,"")==0){
//        cout<<"T should return"<<endl;
        return operand1;
    }
//    cout<<" T_peek_after operand1 "<<endl;
    while((token_code==(int)'*')||(token_code==(int)'/')){
        if(token_code==(int)'*'){
//            cout<<"E_token=*"<<endl;
            while((p<inp+num_of_char)&&isspace(*p))
                p++;
            p=lex_anal(p, token, &token_code);
//            printToken(token_code, token);
            op = (struct MyNODE *) malloc(sizeof(struct MyNODE));
            op->type = MULT;
            op->value.p[0] = operand1;
            op->value.p[1] = F();
            if (op->value.p[1] == NULL){
//                cout<<"T_*_op is null"<<endl;
                return(NULL);		// Error
            }// Error
            operand1 = op;
        }else if(token_code==(int)'/'){
//            cout<<"E_token=/"<<endl;
            while((p<inp+num_of_char)&&isspace(*p))
                p++;
            p=lex_anal(p, token, &token_code);
//            printToken(token_code, token);
            op = (struct MyNODE *) malloc(sizeof(struct MyNODE));
            op->type = DIV;
            op->value.p[0] = operand1;
            op->value.p[1] = F();
            if (op->value.p[1] == NULL){
//                cout<<"T_/_op is null"<<endl;
                return(NULL);
            }// Error
            operand1 = op;
        }
    }
    return operand1;
}
struct MyNODE *F(){
    struct MyNODE *op;
    op = (struct MyNODE *) malloc(sizeof(struct MyNODE));
//    cout<<" F_READ "<<endl;//PASS
    while((p<inp+num_of_char)&&isspace(*p))
        p++;
    p = lex_anal(p, token, &token_code);
//    printToken(token_code, token);//READ THE FIRST WORD
    if(strcmp(token,"")==0){
        return NULL;
    }
    if(token_code==FLOAT_NUMBER){
//        cout<<"FLOAT_NUM"<<endl;
        op->type = FLOAT_NUMBER;
        op->value.f = atof(token);
        return op;
    }else if (token_code==INTEGER_NUMBER){
//        cout<<"INT_NUM"<<endl;
        op->type = token_code;
        op->value.i = atoi(token);
        return op;
    }else if (token_code==STRING){
//        cout<<"STRING_NUM"<<endl;//PASS
        op->type = STRING_CONST;
        strcpy(op->value.c,token);
        return op;
    }else if(token_code==IDENTIFIER){
//        cout<<"validflag==1"<<endl;
//        printToken( token_code, token );
        
//        cout<<"validflag==1"<<endl;
 
        if(validflag==1){
            int i;
            slct->ambiguity=0;
            for(i=0;i<slct->Nrels;i++){
                if(strcmp(slct->AliasName[i],token)==0){
                    while((p<inp+num_of_char)&&isspace(*p))
                        p++;
                    p = lex_anal(p, token, &token_code);
//                    printToken(token_code, token);
                    if(token_code==(int)'.'){
                        while((p<inp+num_of_char)&&isspace(*p))
                            p++;
                        p = lex_anal(p, token, &token_code);
//                        printToken(token_code, token);
                        if(slct->Rel[i]->getAttrType(token)==-1){
                            printf("Attribute not found\n");
                            return(NULL);
                        }else{
                            if(slct->Rel[i]->getAttrType(token)=='I'){
                                op->type = INT_ATTR;
                                op->value.i_attr=(int*)slct->Rel[i]->getAttrPtr(token);
                                return op;
                            }else if(slct->Rel[i]->getAttrType(token)=='F'){
                                op->type = FLOAT_ATTR;
                                op->value.f_attr=(double*)slct->Rel[i]->getAttrPtr(token);
                                return op;
                            }else if(slct->Rel[i]->getAttrType(token)=='C'){
//                                cout<<"ID.ID attribute typr is C"<<endl;
                                op->type = CHAR_ATTR;
                                op->value.c_attr=(char*)slct->Rel[i]->getAttrPtr(token);
                                return op;
                            }
                        }
                    }
                }else if(strcmp(slct->RelName[i],token)==0){
                    if(strcmp(slct->AliasName[i]," ")==0){
                        while((p<inp+num_of_char)&&isspace(*p))
                            p++;
                        p = lex_anal(p, token, &token_code);
//                        printToken(token_code, token);
                        if(token_code==(int)'.'){
                            while((p<inp+num_of_char)&&isspace(*p))
                                p++;
                            p = lex_anal(p, token, &token_code);
//                            printToken(token_code, token);
                            if(slct->Rel[i]->getAttrType(token)==-1){
                                printf("Attribute not found\n");
                                return(NULL);
                            }else{
                                if(slct->Rel[i]->getAttrType(token)=='I'){
                                    op->type = INT_ATTR;
                                    op->value.i_attr=(int*)slct->Rel[i]->getAttrPtr(token);
                                    return op;
                                }else if(slct->Rel[i]->getAttrType(token)=='F'){
                                    op->type = FLOAT_ATTR;
                                    op->value.f_attr=(double*)slct->Rel[i]->getAttrPtr(token);
                                    return op;
                                }else if(slct->Rel[i]->getAttrType(token)=='C'){
//                                    cout<<"ID.ID attribute typr is C"<<endl;
                                    op->type = CHAR_ATTR;
                                    op->value.c_attr=(char*)slct->Rel[i]->getAttrPtr(token);
                                    return op;
                                }
                            }
                        }
                    }else{//if it uses relname and have a alias report error
                        printf("ERROR:IT should not use relname when it has aliasname\n");
                        return NULL;
                    }
                }
            }
            for(i=0;i<slct->Nrels;i++){
                if(slct->Rel[i]->getAttrType(token)=='I'){
                    slct->ambiguity++;
                    slct->rel = i;
                    
                }else if(slct->Rel[i]->getAttrType(token)=='F'){
                    slct->ambiguity++;
                    slct->rel = i;
                }else if(slct->Rel[i]->getAttrType(token)=='C'){
//                    cout<<"look at here"<<endl;
                    slct->ambiguity++;
                    slct->rel = i;
                }
            }//attribute
//            cout<<"slct->rel"<<slct->rel<<endl;
//            cout<<"see the sum is ::"<<slct->ambiguity<<endl;
            if(slct->ambiguity==0){
                cout<<"The attribute does not exist in any rel file::token"<<token<<endl;
                return NULL;
            }
            if(slct->ambiguity>1){
                cout<<"ambiguity!!!"<<endl;
                return NULL;
            }
            if(slct->ambiguity == 1){
                if(slct->Rel[slct->rel]->getAttrType(token)=='I'){
//                    cout<<"i_attr"<<endl;
                    op->type = INT_ATTR;
                    op->value.i_attr=(int*)slct->Rel[slct->rel]->getAttrPtr(token);
                    return op;
                }else if(slct->Rel[slct->rel]->getAttrType(token)=='F'){
//                    cout<<"f_attr"<<endl;
                    op->type = FLOAT_ATTR;
                    op->value.f_attr=(double*)slct->Rel[slct->rel]->getAttrPtr(token);
                    return op;
                }else if(slct->Rel[slct->rel]->getAttrType(token)=='C'){
                    
//                    cout<<"C_attr"<<endl;
                    op->type = CHAR_ATTR;
                    op->value.c_attr=(char*)slct->Rel[slct->rel]->getAttrPtr(token);
                    return op;
                }else
                    return NULL;
            }
        }//valid fladg ==1
        if(strcmp(rel_dup,token)==0){
            while((p<inp+num_of_char)&&isspace(*p))
                p++;
            p = lex_anal(p, token, &token_code);
//            printToken(token_code, token);
            if(token_code==(int)'.'){
                while((p<inp+num_of_char)&&isspace(*p))
                    p++;
                p = lex_anal(p, token, &token_code);
//                printToken(token_code, token);
                if(f->getAttrType(token)==-1){
                    printf("Attribute not found\n");
                    return(NULL);
                }else{
                    
                    if(f->getAttrType(token)=='I'){
                        op->type = INT_ATTR;
                        op->value.i_attr=(int*)f->getAttrPtr(token);
                        return op;
                    }else if(f->getAttrType(token)=='F'){
                        op->type = FLOAT_ATTR;
                        op->value.f_attr=(double*)f->getAttrPtr(token);
                        return op;
                    }else if(f->getAttrType(token)=='C'){
//                        cout<<"ID.ID attribute typr is C"<<endl;
                        op->type = CHAR_ATTR;
                        op->value.c_attr=(char*)f->getAttrPtr(token);
                        return op;
                    }
                }
            }
            
        }else{//attribute name
//             printToken(token_code, token);
            if(f->getAttrType(token)=='I'){
//                cout<<"i_attr"<<endl;
                op->type = INT_ATTR;//////////////////////////////////////////////////////NOT OPEN
                op->value.i_attr=(int*)f->getAttrPtr(token);
                return op;
            }else if(f->getAttrType(token)=='F'){
//                cout<<"f_attr"<<endl;
                op->type = FLOAT_ATTR;
                op->value.f_attr=(double*)f->getAttrPtr(token);
                return op;
            }else if(f->getAttrType(token)=='C'){
                
//                cout<<"c_attr"<<endl;
                op->type = CHAR_ATTR;
                op->value.c_attr=(char*)f->getAttrPtr(token);
                return op;
            }else if(f->getAttrType(token)==-1){
                cout<<"Attr::attr not found in table"<<endl;
            }else{
                cout<<"no response attribute"<<endl;
                return NULL;
            }
        }
    }
    else if(token_code==(int)'('){
        op = E();
        if(op==NULL){
//            cout<<"F_op is null"<<endl;
            return (NULL);
        }
//        cout<<"F (E)"<<endl;
        while((p<inp+num_of_char)&&isspace(*p))
            p++;
        p = lex_anal(p, token, &token_code);
//        printToken( token_code, token );
        
        if(token_code!=')'){
            printf("Error: for node F ) expected\n");
            return(NULL);
        }else{
            return op;
        }
    }
    return op;
}
double MyNODE::eval_expr(){
    if (type== FLOAT_NUMBER ){
//        cout<<"FLOAR_NUMBER"<<endl;
        return value.f;
    }
    else if(type== INTEGER_NUMBER )
    {
//        cout<<"INTEGER_NUMBER"<<endl;
        return value.i;
    }
    else if(type== INT_ATTR ){
//        cout<<"int_attr"<<endl;
        return *value.i_attr;
    }
    else if(type== FLOAT_ATTR ){
//        cout<<"Float_attr"<<endl;
        return *value.f_attr;
    }
    else if(type==EQSS){
//        cout<<"EQSS eval"<<endl;
        
        if(strcmp(value.p[0]->eval_string(),value.p[1]->eval_string())==0){
            return 1;
        }else{
            return 0;
        }
    }
    else if(type==EQLT){
//        cout<<"EQLT eval"<<endl;
        if(strcmp(value.p[0]->eval_string(),value.p[1]->eval_string())<=0){
            return 1;
        }else{
            return 0;
        }
    }
    
    else if(type==EQ){
//        cout<<"EQ"<<endl;
//        cout<<"value.p[0]::::::::"<<value.p[0]<<endl;
//        cout<<"value.p[1]::::::::"<<value.p[1]<<endl;
//        cout<<"value.p[0]eval::::::::"<<value.p[0]->eval_expr()<<endl;
//        cout<<"value.p[1]eval::::::::"<<value.p[1]->eval_expr()<<endl;
//        
        return(value.p[0]->eval_expr()== value.p[1]->eval_expr());
        
    }
    else if(type==LT){
//        cout<<"LT"<<endl;
        return (value.p[0]->eval_expr() < value.p[1]->eval_expr());
        
    }
    else if (type== LESSOREQ){
//        cout<<"LESSQREQ"<<endl;
        return (value.p[0]->eval_expr() <= value.p[1]->eval_expr());
        
    }
    else if (type== GREATEROREQ){
//        cout<<"GREATEROREQ"<<endl;
//        cout<<"value.p[0]::::::::"<<value.p[0]<<endl;
//        cout<<"value.p[1]::::::::"<<value.p[1]<<endl;
//        cout<<"value.p[0]eval::::::::"<<value.p[0]->eval_expr()<<endl;
//        cout<<"value.p[1]eval::::::::"<<value.p[1]->eval_expr()<<endl;
        return (value.p[0]->eval_expr() >= value.p[1]->eval_expr()); //
        
    }
    
    else if (type==GT){
//        cout<<"GTTTT"<<endl;
        return  (value.p[0]->eval_expr() > value.p[1]->eval_expr());//
    }
    else if(type==NOTEQ){
//        cout<<"NOTEQ"<<endl;
        return (value.p[0]->eval_expr() != value.p[1]->eval_expr());
    }
    else if (type== PLUS){
//        cout<<"PLUS"<<endl;
        return (value.p[0]->eval_expr() + value.p[1]->eval_expr());//
        
    }
    else if (type== MINUS ){
//        cout<<"MINUS"<<endl;
        return (value.p[0]->eval_expr() - value.p[1]->eval_expr()); //
        
    }
    else if ( type== MULT){
//        cout<<"MULT"<<endl;
        return (value.p[0]->eval_expr() * value.p[1]->eval_expr()); //
    }
    else if ( type== DIV){
//        cout<<"DIV"<<endl;
        return (value.p[0]->eval_expr() / value.p[1]->eval_expr()); //
    }
    
    else if (type== AND){
//        cout<<"AND"<<endl;
        return (value.p[0]->eval_expr() && value.p[1]->eval_expr()); //
        
    }
    else if (type== OR){
//        cout<<"OR"<<endl;
        return (value.p[0]->eval_expr() || value.p[1]->eval_expr()); //
        
    }
    else if (type== NOT){
//        cout<<"NOT"<<endl;
        return (!(value.q->eval_expr()));
    }else if(type==STRING_CONST||CHAR_ATTR){
//        cout<<"test whether this works~~~~!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
        eval_string();
    }
    return 1;
}


