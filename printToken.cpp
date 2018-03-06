
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "lexanal.h"

using namespace std;

void printToken( int code, char *token )
{
    cout << "Token code = " << code << ", token = '" << token << "'" << endl;
    
    
    /* =====================================================
     Check the keywords first
     ===================================================== */
    switch ( code )
    {
        case LESSOREQ:
            if (strcasecmp( token, "<=" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
            
        case GREATEROREQ:
            if (strcasecmp( token, ">=" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
            
        case NOTEQ:
            if (strcasecmp( token, "!=" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
            
        case ALL:
            if (strcasecmp( token, "ALL" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case AND:
            if (strcasecmp( token, "AND" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case ANY:
            if (strcasecmp( token, "ANY" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case AS:
            if (strcasecmp( token, "AS" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case AVG:
            if (strcasecmp( token, "AVG" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case BETWEEN:
            if (strcasecmp( token, "BETWEEN" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case BY:
            if (strcasecmp( token, "BY" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case CHAR:
            if (strcasecmp( token, "CHAR" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case CHECK:
            if (strcasecmp( token, "CHECK" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case CLOSE:
            if (strcasecmp( token, "CLOSE" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case COMMIT:
            if (strcasecmp( token, "COMMIT" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case COUNT:
            if (strcasecmp( token, "COUNT" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case CREATE:
            if (strcasecmp( token, "CREATE" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case DECIMAL:
            if (strcasecmp( token, "DECIMAL" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case DELETE:
            if (strcasecmp( token, "DELETE" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case DISTINCT:
            if (strcasecmp( token, "DISTINCT" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case DOUBLE:
            if (strcasecmp( token, "DOUBLE" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case DROP:
            if (strcasecmp( token, "DROP" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case EXISTS:
            if (strcasecmp( token, "EXISTS" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case FLOAT:
            if (strcasecmp( token, "FLOAT" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case FROM:
            if (strcasecmp( token, "FROM" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case GO:
            if (strcasecmp( token, "GO" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case GROUP:
            if (strcasecmp( token, "GROUP" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case HAVING:
            if (strcasecmp( token, "HAVING" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case IN:
            if (strcasecmp( token, "IN" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case INSERT:
            if (strcasecmp( token, "INSERT" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case INT:
            if (strcasecmp( token, "INT" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case INTO:
            if (strcasecmp( token, "INTO" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case IS:
            if (strcasecmp( token, "IS" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case LIKE:
            if (strcasecmp( token, "LIKE" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case MAX:
            if (strcasecmp( token, "MAX" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case MIN:
            if (strcasecmp( token, "MIN" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case NOT:
            if (strcasecmp( token, "NOT" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case NULL0:
            if (strcasecmp( token, "NULL" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case NUMERIC:
            if (strcasecmp( token, "NUMERIC" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case OF:
            if (strcasecmp( token, "OF" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case ON:
            if (strcasecmp( token, "ON" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case OR:
            if (strcasecmp( token, "OR" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case ORDER:
            if (strcasecmp( token, "ORDER" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case PRIMARY:
            if (strcasecmp( token, "PRIMARY" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case REAL:
            if (strcasecmp( token, "REAL" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case SCHEMA:
            if (strcasecmp( token, "SCHEMA" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case SELECT:
            if (strcasecmp( token, "SELECT" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case SET:
            if (strcasecmp( token, "SET" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case SOME:
            if (strcasecmp( token, "SOME" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case SUM:
            if (strcasecmp( token, "SUM" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case TABLE:
            if (strcasecmp( token, "TABLE" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case TO:
            if (strcasecmp( token, "TO" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case UNION: 
            if (strcasecmp( token, "UNION" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case UNIQUE: 
            if (strcasecmp( token, "UNIQUE" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case UPDATE: 
            if (strcasecmp( token, "UPDATE" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case USER: 
            if (strcasecmp( token, "USER" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case VALUES: 
            if (strcasecmp( token, "VALUES" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case VIEW: 
            if (strcasecmp( token, "VIEW" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case WHERE: 
            if (strcasecmp( token, "WHERE" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
        case WITH: 
            if (strcasecmp( token, "WITH" ) == 0 )
                return;
            else
                printf("Wrong token code: %d for token '%s'\n", code, token);
            break;
    }
    
    
    
    if ( code < 128 && code == *token && *(token+1) == '\0' )
    {
        return;
    }
    
    // Cannot check the categories, but print the fact only
    
    if ( code == IDENTIFIER )
    {
        cout << "IDENTIFIER" << endl;
        return;
    }
    
    if ( code == INTEGER_NUMBER)
    {
        cout << "INTEGER_NUMBER" << endl;
        return;
    }
    
    if ( code == FLOAT_NUMBER)
    {
        cout << "FLOAT_NUMBER" << endl;
        return;
    }
    
    if ( code == STRING )
    {
        cout << "STRING" << endl;
        return;
    }
    
    
}
