
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include "ScanTable.h"

using namespace std;

int ScanTable::Open(char *relname)
{
   if ( (fp = fopen("catalog", "r")) == NULL )
   {
      cout << "Error: cannot open database catalog `./catalog'" << endl;
      return 0;
   }else{
//       cout<<"catalog ok"<<endl;
   }
//    fd = open(relname, O_RDWR);


   n_fields = 0;
   record_size = 0;

   /* ==================================================
	 Read attribute information on relation "relname"
      ================================================== */
   while ( fscanf(fp, "%s %s %c %d %d",
             DataDes[n_fields].relname, DataDes[n_fields].fieldname,
             &DataDes[n_fields].fieldtype, &DataDes[n_fields].startpos,
             &DataDes[n_fields].fieldsize) > 0 )
   {
      if ( strcmp( DataDes[n_fields].relname, relname ) == 0 )
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

   fclose(fp);
//    cout<<"try to open rel"<<endl;
   if ( n_fields > 0 )
   {
      if ( (fd = open(relname, O_RDWR)) == -1 )
      {
	 cout << "Error: canot open relation file " << relname << endl;
         return 0;
      }else{
//          cout<<"relation file has been opened"<<endl;
      }

      strcpy( TableName, relname);

      return 1;
   }
   else
   {
      return 0;		// Relation not found
   }
}

/* ==================================================
   GetNext(): read next tuple

     The tuple is return in this->buf

     return 1 for OK
            0 for error
   ================================================== */
int ScanTable::GetNext()
{ 
   sizeOfGetNext=0;

   sizeOfGetNext = read(fd, buf, record_size+1);

   if ( sizeOfGetNext == record_size+1 )
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

int ScanTable::getValid(){
    char *valid;
    valid=&(buf[record_size]);
    if(strcmp(valid,"Y")==0){
        return 1;
    }else{
        return 0;
    }
    
}

void ScanTable::Close()
{
   close(fd);		// Close relation file
}



/* ================================================================
401 `p04' 2 || -- Valid flag: N
401 `p01' 10 || -- Valid flag: Y
401 `p02' 6 || -- Valid flag: Y
   getAttrType(attrName): Get data type of attr attrName
   ================================================================ */
int ScanTable::getAttrType( char *attrName )
{
   int i;

   for ( i = 0; i < n_fields; i++ )
   {
      if ( strcmp( attrName, DataDes[i].fieldname ) == 0 )
         return DataDes[i].fieldtype;
   }

   return -1;		// Attribute not found...
}



/* ================================================================
   getAttrSize(attrName): Get size of attr attrName
	(Only necessary for 'C' typed attributes)
   ================================================================ */
int ScanTable::getAttrSize( char *attrName )
{
   int i;

   for ( i = 0; i < n_fields; i++ )
   {
      if ( strcmp( attrName, DataDes[i].fieldname ) == 0 )
         return DataDes[i].fieldsize;
   }

   return -1;		// Attribute not found...
}


/* ================================================================
   getAttrPtr(attrName): return the pointer to attribute attrName
			 in buf[ ]
   ================================================================ */
void * ScanTable::getAttrPtr( char *attrName )
{
   int i;

   for ( i = 0; i < n_fields; i++ )
   {
      if ( strcmp( attrName, DataDes[i].fieldname ) == 0 )
         return &buf[ DataDes[i].startpos ];
   }

   return NULL;		// Attribute not found...
}




/* ================================================================
   PrintTuple(): print the tuple in buf[ ]
   ================================================================ */
void ScanTable::PrintTuple()
{
   int j;

   for (j = 0; j < n_fields; j++)
   {
      int    *i_ptr;   // To access an int variable
      double *f_ptr;   // To access a  "float" (= double) variable
      char   *c_ptr;   // To access a  char[ ] variable (string)

      if ( DataDes[j].fieldtype == 'I' )
      {
         i_ptr = (int *) &(buf[ DataDes[j].startpos]) ;
         printf("%d ", *i_ptr );         // *i_ptr is an int variable
      }
      else if ( DataDes[j].fieldtype == 'F' )
      {
         f_ptr = (double *) &(buf[ DataDes[j].startpos]) ;
         printf("%lf ", *f_ptr );         // *f_ptr is a double variable
      }
      else
      {
         c_ptr = (char *) &(buf[ DataDes[j].startpos]) ;
         printf("`%s' ", c_ptr );        // String is a (char *)
      }
   }

   printf("|| -- Valid flag: %c\n", buf[record_size] );
}

void ScanTable::PrintRelationInfo()
{
   int i;

   printf("\nThe relation contains these fields:\n");
   for (i = 0; i < n_fields; i++)
      cout << DataDes[i].relname << "." << DataDes[i].fieldname <<
		"\ttype = " << DataDes[i].fieldtype <<
		"\tstartpos = " << DataDes[i].startpos <<
		"\tsize = " << DataDes[i].fieldsize << endl;

   cout << endl << endl;
}

void ScanTable::WriteTuple(){
    int fd;
    fd = open("tmp", O_WRONLY|O_CREAT|O_APPEND, 0666);
    write(fd, buf, record_size+1);
    close(fd);

}
