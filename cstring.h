// Developed by Nalin Ahuja, nalinahuja

// Return Codes
#define CSTRING_OK (0)
#define CSTRING_ERR (1)

// Embedded Values
#define CSTRING_EOL (-1)
#define CSTRING_ALLOC (16)

// End Macro Definitions---------------------------------------------------------------------------------------------------------------------------------------------------

// Numerical Types
typedef unsigned int uint;

// String Type
typedef struct string {
  // Public Attributes
  char * str;
  uint cap;
  uint len;

  // Private Attributes
  struct string * _prev;
  struct string * _next;
} string;

// End Type Definitions----------------------------------------------------------------------------------------------------------------------------------------------------

// String Initializer Function Prototype
string * cstring(char *);

// String Attribute Function Prototypes
uint cap(string *);
uint len(string *);
char * str(string *);

// String Memory Management Function Prototypes
void clear(string * s);
void delete(string * s);
void delete_all(void);

// String Replication Function Prototypes
string * copy(string *);
string * substr(string *, uint);
string * substrn(string *, uint, uint);

// String Manipulation Function Prototypes
int insert(string *, char *, uint);
int append(string * , char *);
int prepend(string *, char *);
int concat(string *, string *);

// String Access Function Prototypes
int find(string *, char *);
char get(string *, uint);
char rem(string *, uint);
char set(string *, uint, char);

// End Function Prototypes-------------------------------------------------------------------------------------------------------------------------------------------------
