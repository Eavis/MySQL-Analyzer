
struct DataDescription
{
   char relname[30];
   char fieldname[30];
   char fieldtype;
   int  startpos;
   int  fieldsize;
};


class ScanTable
{
public:
   FILE *fp;
   int  fd;
   char TableName[33];		// Name of the relation

   DataDescription DataDes[10];  /* Holds data descriptions
                                           for upto 10 fields */
   int  n_fields;                       /* Actual # fields */
   int  record_size;
  int sizeOfGetNext;
   char buf[1000];			// Buffer to store tuples

   int Open(char *relname);
   int GetNext();
   void Close();
    int getValid();

   int getAttrType( char *attrName );	// Get data type of attr attrName
   int getAttrSize( char *attrName );	// Get size of attr attrName
   void *getAttrPtr( char *attrName );	// Get pointer to attrName in buf[1000]
    void WriteTuple();

   void PrintTuple();
   void PrintRelationInfo();
};






