#include <iostream>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include"lexanal.h"
using namespace std;
int read_command(char *input){
    size_t len;
    int index =0;
    int i;
    string read_string;
    while(1){
        if(getline(cin,read_string)){
        }
        len = read_string.size();
        if(len==0){
            printf("Error:read_string 0 size\n");
            return 0;
        }
        if((read_string[0]=='g'||read_string[0]=='G')&&(read_string[1]=='o'||read_string[1]=='O')&&(read_string[2]=='\0')){
            break;
        }else{
            for(i=0;i<len;i++){
                input[index]=read_string[i];
                index++;
            }
        }
        
        input[index++] =' ';
    }

//    cout<<"index is :"<<index<<endl;
    return index;
}
char *lex_anal(char *current_pos, char token[ ], int *token_type){
    int i=0;
    char singleTokens[] = "= , ( ) [ ] : ; *  /  \" ";
    char rep_token[64];
    *token_type =0;
    int j=0;
    int pos=0;
    size_t len = strlen(token);
    for(i=0;i<len;i++){
        token[i]=0;
    }
    size_t rep_len = strlen(rep_token);
    for(i=0;i<rep_len;i++){
        rep_token[i]=0;
    }
    while(isspace(*current_pos))
        current_pos++;
    if(isalpha(*current_pos)){
        for(i=0;i<strlen(current_pos);i++){
            if(isalpha(*(current_pos+i))||*(current_pos+i)=='_'||isdigit(*(current_pos+i))){
                rep_token[i]=(char)toupper(*(current_pos+i));
//                cout<<"i is"<<i<<"  "<<rep_token[i]<<endl;
                token[i] =*(current_pos+i);
            }else{
//                *token_type = IDENTIFIER;
//                cout<<"rep_token"<<rep_token<<endl;
                pos = i;
                rep_token[i]='\0';
                token[i] = '\0';
                break;
            }
            
            
        }
    }
    else if (isdigit(*current_pos)){
        for(i=0;i<strlen(current_pos);i++){
            if(isdigit(*(current_pos+i))){
                token[i]=*(current_pos+i);
            }else if(*(current_pos+i)=='.'){
                token[i]=*(current_pos+i);
                for(j=1;j<strlen(current_pos)-i;j++){
                    if(isdigit(*(current_pos+i+j))){
                        token[i+j] = *(current_pos+i+j);
                    }else{
                        *token_type = FLOAT_NUMBER;
                        pos =i+j;
                        token[i+j] = '\0';
                        break;
                    }
                }
            }else{
                *token_type = INTEGER_NUMBER;
                pos =i;
                token[i]='\0';
                break;
            }
        }
    }
    else if(*current_pos=='.'){
        token[0] = '.';
        if(!isdigit(*(current_pos+1))){
            *token_type = (int)'.';
            pos = 1;
        }
        else{
            for(i=1;i<strlen(current_pos);i++){
                if(isdigit(*(current_pos+i))){
                    token[i]=*(current_pos+i);
                    
                }
                else{
                    *token_type = FLOAT_NUMBER;
                    pos = i+1;
                    token[i] = '\0';
                    break;
                }
            }
        }
    }
    else if((*current_pos=='+')||(*current_pos=='-')){
        token[0] = *current_pos;
        if(isdigit(*(current_pos+1))){
            for(i=1;i<strlen(current_pos);i++){
                if(isdigit(*(current_pos+i))){
                    token[i]= *(current_pos+i);
                }
                else if(*(current_pos+i)=='.'){
                    token[i]=*(current_pos+i);
                    for(j=1;j<strlen(current_pos)-i;j++){
                        if(isdigit(*(current_pos+i+j))){
                            token[i+j] = *(current_pos+i+j);
                        }else{
                            *token_type = FLOAT_NUMBER;
                            pos = i+j;
                            token[i+j] = '\0';
                            break;
                        }
                    }
                    break;
                }else{
                    *token_type = INTEGER_NUMBER;
                    token[i] = '\0';
                    pos = i;
                }
            }
        }else if(*(current_pos+1)=='.'){
            for(i=2;i<strlen(current_pos);i++){
                token[1] = '.';
                if(isdigit(*(current_pos+i))){
                    token[i]= *(current_pos+i);
                }else{
                    *token_type = INTEGER_NUMBER;
                    token[i] = '\0';
                    pos = i;
                }
            }
        }else{
            token[1]='\0';
            *token_type = (int)(*current_pos);
            pos =1 ;
        }
    }
    
    else if(*current_pos == '\''){
        for(i=0;i<strlen(current_pos);i++){
            if((*(current_pos +i+1))=='\''){
                *token_type =STRING;
                pos = i+2;
                break;
            } else
                token[i] = *(current_pos+i+1);
        }
        token[i] = '\0';
    }
    else if(strchr(singleTokens,*current_pos)!=NULL){
        token[0] = *current_pos;
        *token_type = (int)(*current_pos);
        pos =1;
    }
    else if(((*current_pos)=='<')||((*current_pos)=='>')||((*current_pos)=='!')){
        token[0] = *current_pos;
        if((*(current_pos+1))=='='){
            token[1]='=';
            *token_type = (int)(*current_pos);
            //            pos = 1;
            pos =2;
        }else{
            *token_type = (int)(*current_pos);
            //            pos = 0;
            pos = 1;
        }
    }
    if(strcmp(token,"<=")==0)
        *token_type = LESSOREQ;
    else if(strcasecmp(rep_token,"DROP")==0){
//        cout<<"token_type"<<endl;
        *token_type = DROP;
        
    }
    else if(strcmp(token,">=")==0)
        *token_type = GREATEROREQ;
    else if(strcmp(token,"!=")==0)
        *token_type = NOTEQ;
    else if(strcmp(rep_token,"ALL")==0)
        *token_type = ALL;
    else if(strcmp(rep_token,"AND")==0)
        *token_type = AND;
    else if(strcmp(rep_token,"ANY")==0)
        *token_type = ANY;
    else if(strcmp(rep_token,"AS")==0)
        *token_type = AS;
    else if(strcmp(rep_token, "AVG")==0)
        *token_type = AVG;
    else if(strcmp(rep_token,"BETWEEN")==0)
        *token_type = BETWEEN;
    else if(strcmp(rep_token,"BY")==0)
        *token_type = BY;
    else if(strcmp(rep_token,"CHAR")==0)
        *token_type = CHAR;
    else if(strcmp(rep_token,"CHECK")==0)
        *token_type = CHECK;
    else if(strcmp(rep_token,"CLOSE")==0)
        *token_type = CLOSE;
    else if(strcmp(rep_token,"COMMIT")==0)
        *token_type = COMMIT;
    else if(strcmp(rep_token,"COUNT")==0)
        *token_type = COUNT;
    else if(strcmp(rep_token,"CREATE")==0)
        *token_type = CREATE;
    else if(strcmp(rep_token,"DECIMAL")==0)
        *token_type = DECIMAL;
    else if(strcmp(rep_token,"DELETE")==0)
        *token_type = DELETE;
    else if(strcmp(rep_token,"DISTINCT")==0)
        *token_type =DISTINCT;
    else if(strcmp(rep_token,"DOUBLE")==0)
        *token_type =DOUBLE;

    else if(strcmp(rep_token,"EXISTS")==0)
        *token_type = EXISTS;
    else if(strcmp(rep_token,"FLOAT")==0)
        *token_type = FLOAT;
    else if(strcmp(rep_token,"INT")==0)
        *token_type = INT;
    else if(strcmp(rep_token,"FROM")==0)
        *token_type = FROM;
    else if(strcmp(rep_token,"GO")==0)
        *token_type = GO;
    else if(strcmp(rep_token,"GROUP")==0)
        *token_type = GROUP;
    else if(strcmp(rep_token,"HAVING")==0)
        *token_type = HAVING;
    else if(strcmp(rep_token,"IN")==0)
        *token_type = IN;
    else if(strcmp(rep_token,"INSERT")==0)
        *token_type = INSERT;
    else if(strcmp(rep_token,"INTO")==0)
        *token_type = INTO;
    else if(strcmp(rep_token,"IS")==0)
        *token_type =IS;
    else if(strcmp(rep_token,"LIKE")==0)
        *token_type = LIKE;
    else if(strcmp(rep_token,"MAX")==0)
        *token_type = MAX;
    else if(strcmp(rep_token,"MIN")==0)
        *token_type = MIN;
    else if(strcmp(rep_token,"NOT")==0)
        *token_type = NOT;
    else if(strcmp(rep_token,"NULL")==0)
        *token_type = NULL0;
    else if(strcmp(rep_token,"NUMERIC")==0)
        *token_type = NUMERIC;
    else if(strcmp(rep_token,"OF")==0)
        *token_type = OF;
    else if(strcmp(rep_token,"ON")==0)
        *token_type = ON;
    else if(strcmp(rep_token,"OR")==0)
        *token_type = OR;
    else if(strcmp(rep_token,"ORDER")==0)
        *token_type = ORDER;
    else if(strcmp(rep_token,"PRIMARY")==0)
        *token_type = PRIMARY;
    else if(strcmp(rep_token,"REAL")==0)
        *token_type = REAL;
    else if(strcmp(rep_token,"SCHEMA")==0)
        *token_type =SCHEMA;
    else if(strcmp(rep_token,"SELECT")==0)
        *token_type = SELECT;
    else if(strcmp(rep_token,"SET")==0)
        *token_type = SET;
    else if(strcmp(rep_token,"SOME")==0)
        *token_type = SOME;
    else if(strcmp(rep_token,"SUM")==0)
        *token_type = SUM;
    else if(strcmp(rep_token,"TABLE")==0)
        *token_type = TABLE;
    else if(strcmp(rep_token,"TO")==0)
        *token_type = TO;
    else if(strcmp(rep_token,"UNION")==0)
        *token_type = UNION;
    else if(strcmp(rep_token,"UNIQUE")==0)
        *token_type = UNIQUE;
    else if(strcmp(rep_token,"UPDATE")==0)
        *token_type =UPDATE;
    else if(strcmp(rep_token,"USER")==0)
        *token_type = USER;
    else if(strcmp(rep_token,"VALUES")==0)
        *token_type = VALUES;
    else if(strcmp(rep_token,"VIEW")==0)
        *token_type = VIEW;
    else if(strcmp(rep_token,"WHERE")==0)
        *token_type = WHERE;
    else if(strcmp(rep_token,"WITH")==0)
        *token_type = WITH;
    else if(isalpha(rep_token[0]))
        *token_type = IDENTIFIER;
    return (char*)(current_pos+pos);
}



















