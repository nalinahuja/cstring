// Developed by Nalin Ahuja, nalinahuja22

// Header Files
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// Embedded Values
#define CSTRING_SUC (1)
#define CSTRING_ERR (0)
#define CSTRING_ALC (16)
#define CSTRING_EOL (-1)

// End Includes and Definitions--------------------------------------------------------------------------------------------------------------------------------------------

// Numerical Types
typedef int int32;
typedef unsigned char byte;
typedef unsigned int uint32;

// String Type
typedef struct string {
  // Public Attributes
  char * str;
  uint32 cap;
  uint32 len;

  // Private Attributes
  struct string * _prev;
  struct string * _next;
} string;

// End Defined Types-------------------------------------------------------------------------------------------------------------------------------------------------------

// Allocation List
static string * _alloc_list;

// Thread Mutex Lock
static pthread_mutex_t _mutex;

// End Global Variables----------------------------------------------------------------------------------------------------------------------------------------------------

// Library Initializer
static void _init(void) __attribute__ ((constructor));

// End Function Prototypes-------------------------------------------------------------------------------------------------------------------------------------------------

/*
 * _print_exit - Prints An Error Message Before Exiting Program
 */

void _print_exit(char * msg) {
  // Flush All Output Streams
  fflush(NULL);

  // Display Error Message
  printf("\ncstring: %s\n", msg);

  // Exit Program
  exit(1);
}

/*
 * _add_alloc - Adds A String Structure To Allocation List
 */

void _add_alloc(string * s) {
  // Verify Allocation List Head
  if (_alloc_list == NULL) {
    // Initialize Allocation List
    _alloc_list = s;
  } else {
    // Apppend Allocation To List
    _alloc_list->_next = s;
    s->_prev = _alloc_list;

    // Increment Allocation List
    _alloc_list = _alloc_list->_next;
  }
}

/*
 * _remove_alloc - Removes A String Structure From The Allocation List
 */

void _remove_alloc(string * s) {
  // Verify Allocation List
  if (_alloc_list != NULL) {
    // Decrement Allocation List Pointer
    if (_alloc_list == s) {
      _alloc_list = _alloc_list->_prev;
    }

    // Unbind Right Node
    if (s->_next != NULL) {
      s->_next->_prev = s->_prev;
    }

    // Unbind Left Node
    if (s->_prev != NULL) {
      s->_prev->_next = s->_next;
    }
  }
}

// End Private Library Functions-------------------------------------------------------------------------------------------------------------------------------------------

/*
 * cstring - Returns A New String Allocation
 */

string * cstring(char * init_str) {
  // Lock Thread Mutex
  pthread_mutex_lock(&_mutex);

  // Initialize Default String Attributes
  uint32 init_len = 0, init_mem = CSTRING_ALC;

  // Verify Initialization String
  if (init_str != NULL) {
    // Update Length
    init_len = strlen(init_str);

    // Update Memory
    init_mem += init_len;
  }

  // Initialize String Structure
  string * s = (string *) malloc(sizeof(string));

  // Set Structure Members
  s->str = (char *) calloc(sizeof(char), init_mem);
  s->cap = init_mem;
  s->len = init_len;

  // Verify Initialization
  if ((s == NULL) || (s->str == NULL)) {
    // Check String Allocation
    if (s->str != NULL) {
      // Free String Memory
      free(s->str);
      s->str = NULL;
    }

    // Check Structure Allocation
    if (s != NULL) {
      // Unload Structure Memory
      free(s);
      s = NULL;
    }

    // Verify Arguments
    _print_exit("[cstring] failed to initialize new string");
  }

  // Verify Initialization String
  if (init_str != NULL) {
    // Copy Initialization String To Structure
    strncpy(s->str, init_str, init_len);
  }

  // Add Structure Allocation To List
  _add_alloc(s);

  // Unlock Mutex
  pthread_mutex_unlock(&_mutex);

  // Return String Pointer
  return (s);
}

// End String Initializer--------------------------------------------------------------------------------------------------------------------------------------------------

/*
 * cap - Returns Capacity Of Indicated String Allocation
 */

int cap(string * s) {
  // Verify Parameters
  if (s == NULL) {
    _print_exit("[cap] string pointer is NULL");
  }

  // Return String Capacity
  return (s->cap);
}

/*
 * len - Returns Length Of Indicated String Allocation
 */

int len(string * s) {
  // Verify Parameters
  if (s == NULL) {
    _print_exit("[len] string pointer is NULL");
  }

  // Return String Length
  return (s->len);
}

/*
 * str - Returns Contents Of Indicated String Allocation
 */

char * str(string * s) {
  // Verify Parameters
  if (s == NULL) {
    _print_exit("[str] string pointer is NULL");
  } else if (s->str == NULL) {
    _print_exit("[str] string attribute is NULL");
  }

  // Return String Pointer
  return (s->str);
}

// End Field Access Functions----------------------------------------------------------------------------------------------------------------------------------------------

/*
 * clear - Resets Contents Of The Indicated String Allocation
 */

void clear(string * s) {
  // Verify Parameters
  if (s == NULL) {
    _print_exit("[clear] string pointer is NULL");
  } else if (s->str == NULL) {
    _print_exit("[clear] string attribute is NULL");
  }

  // Reset String Length
  s->str[0] = 0;
}

/*
 * delete - Frees Memory Of The Indicated String Allocation
 */

void delete(string * s) {
  // Verify Parameters
  if (s == NULL) {
    _print_exit("[delete] string pointer is NULL");
  } else if (s->str == NULL) {
    _print_exit("[delete] string attribute is NULL");
  }

  // Lock Thread Mutex
  pthread_mutex_lock(&_mutex);

  // Remove Allocation From List
  _remove_alloc(s);

  // Free String Memory
  free(s->str);
  s->str = NULL;

  // Free Structure Memory
  free(s);
  s = NULL;

  // Unlock Thread Mutex
  pthread_mutex_unlock(&_mutex);
}

/*
 * delete_all - Frees Memory Of All String Allocations
 */

void delete_all(void) {
  // Lock Thead Mutex
  pthread_mutex_lock(&_mutex);

  // Clear Over Allocation List
  while (_alloc_list != NULL) {
    // Get Allocation From List
    string * s = _alloc_list;

    // Decrement Allocation List
    _alloc_list = _alloc_list->_prev;

    // Free String Memory
    free(s->str);
    s->str = NULL;

    // Free Structure Memory
    free(s);
    s = NULL;
  }

  // Unlock Thead Mutex
  pthread_mutex_unlock(&_mutex);
}

// End Memory Management Functions-----------------------------------------------------------------------------------------------------------------------------------------

/*
 * copy - Returns A Copy Of Indicated String
 */

string * copy(string * s) {
  // Verify Parameters
  if (s == NULL) {
    _print_exit("[copy] string pointer is NULL");
  } else if (s->str == NULL) {
    _print_exit("[copy] string attribute is NULL");
  }

  // Return Copy Of String
  return (cstring(s->str));
}

/*
 * substr - Returns A Copy Of Indicated String From [i, len(s))
 */

string * substr(string * s, uint32 i) {
  // Verify Parameters
  if (s == NULL) {
    _print_exit("[substr] string pointer is NULL");
  } else if (s->str == NULL) {
    _print_exit("[substr] string attribute is NULL");
  } else if (i >= s->len) {
    _print_exit("[substr] string index out of bounds");
  }

  // Return Substring From [i, len(s))
  return (cstring(s->str + i));
}

/*
 * substrn - Returns A Copy Of Indicated String From [i, i + n)
 */

string * substrn(string * s, uint32 i, uint32 n) {
  // Verify Parameters
  if (s == NULL) {
    _print_exit("[substrn] string pointer is NULL");
  } else if (s->str == NULL) {
    _print_exit("[substrn] string attribute is NULL");
  } else if (i + n > (s->len)) {
    _print_exit("[substrn] string index out of bounds");
  }

  // Get Substring From ith Index
  char * si = (s->str + i);

  // Store Removed Character
  char rc = si[j - i];

  // Set Null Terminator
  si[j - i] = 0;

  // Create Substring
  string * sn = (cstring(si));

  // Unset Null Terminator
  si[j - i] = rc;

  // Return Substring From [i, j)
  return (sn);
}

// End String Duplication Functions----------------------------------------------------------------------------------------------------------------------------------------

/*
 * insert - Inserts A Substring At ith Index
 */

byte insert(string * s, char * c, uint32 i) {
  // Verify Parameters
  if (s == NULL) {
    _print_exit("[insert] string pointer is NULL");
  } else if (s->str == NULL) {
    _print_exit("[insert] string attribute is NULL");
  } else if (c == NULL) {
    _print_exit("[insert] insertion string is NULL");
  } else if (i > s->len) {
    _print_exit("[insert] string index out of bounds");
  }

  // Calculate Updated String Length
  uint32 req_len = strlen(c);

  // Calculate Updated String Memory
  uint32 req_mem = (s->len + req_len);

  // Verify Available Memory
  if ((s->cap) >= req_mem) {
    // Sufficient Memory
    if (i < (s->len)) {
      // Perform Character Shifts
      for (uint32 j = 0; j < req_len; ++j) {
        // Right Shift String
        for (uint32 k = s->len; k > i; --k) {
          s->str[k + j] = s->str[k + j - 1];
        }
      }
    }

    // Copy Insertion String Into Structure
    for (uint32 j = 0; j < req_len; ++j) {
      s->str[i + j] = c[j];
    }

    // Update String Length
    s->len += req_len;
  } else {
    // Calculate Memory Requirement
    uint32 new_mem = (req_mem << 1);

    // Allocate String Memory
    char * new_str = (char *) calloc(sizeof(char), new_mem);

    // Verify Memory Pointer
    if (new_str == NULL) {
      _print_exit("[insert] failed to extend string memory space");
    }

    // Transfer Memory Contents
    for (uint32 i = 0; i < s->len; i++) {
      new_str[i] = s->str[i];
    }

    // Free Old Memory Contents
    free(s->str);
    s->str = NULL;

    // Update Structure Members
    s->str = new_str;
    s->cap = new_mem;

    // Retry Insertion Operation
    insert(s, c, i);
  }

  // Return Success
  return (CSTRING_SUC);
}

/*
 * append - Appends A Character String To String Structure
 */

byte append(string * s, char * c) {
  // Verify Parameters
  if (s == NULL) {
    _print_exit("[append] string pointer is NULL");
  } else if (s->str == NULL) {
    _print_exit("[append] string attribute is NULL");
  } else if (c == NULL) {
    _print_exit("[append] insertion string is NULL");
  }

  // Perform Append Operation
  return (insert(s, c, s->len));
}

/*
 * prepend - Prepends A Character String To String Structure
 */

byte prepend(string * s, char * c) {
  // Verify Parameters
  if (s == NULL) {
    _print_exit("[prepend] string pointer is NULL");
  } else if (s->str == NULL) {
    _print_exit("[prepend] string attribute is NULL");
  } else if (c == NULL) {
    _print_exit("[prepend] insertion string is NULL");
  }

  // Perform Prepend Operation
  return (insert(s, c, 0));
}

/*
 * concat - Concatenates Two String Structures Togehter
 */

byte concat(string * s1, string * s2) {
  // Verify Parameters
  if ((s1 == NULL) || (s2 == NULL)) {
    _print_exit("[concat] at least one string pointer is NULL");
  } else if ((s1->str == NULL) || (s2->str == NULL)) {
    _print_exit("[concat] at least one string attribute is NULL");
  }

  // Perform Concatenation Operation
  return (append(s1, s2->str));
}

// End String Manipulation Functions---------------------------------------------------------------------------------------------------------------------------------------

/*
 * find - Determines The Position Of Substring
 */

int32 find(string * s, char * c) {
  // Verify Parameters
  if (s == NULL) {
    _print_exit("[find] string pointer is NULL");
  } else if (s->str == NULL) {
    _print_exit("[find] string attribute is NULL");
  } else if (c == NULL) {
    _print_exit("[find] pattern string is NULL");
  }

  // Get Substring Position
  char * pos = strstr(s->str, c);

  // Return Substring Position
  return ((pos) ? (pos - (s->str)) : (CSTRING_EOL));
}

/*
 * get - Gets The ith Character In String
 */

char get(string * s, uint32 i) {
  // Verify Parameters
  if (s == NULL) {
    _print_exit("[get] string pointer is NULL");
  } else if (s->str == NULL) {
    _print_exit("[get] string attribute is NULL");
  } else if (i >= (s->len)) {
    _print_exit("[get] string index out of bounds");
  }

  // Return Character
  return (s->str[i]);
}

/*
 * rem - Removes The ith Character In Indicated String
 */

char rem(string * s, uint32 i) {
  // Verify Parameters
  if (s == NULL) {
    _print_exit("[get] string pointer is NULL");
  } else if (s->str == NULL) {
    _print_exit("[get] string attribute is NULL");
  } else if (i >= (s->len)) {
    _print_exit("[find] string index out of bounds");
  }

  // Store Removed Character
  char rc = s->str[i];

  // Left Shift String
  for (uint32 j = i; j < (s->len); ++j) {
    s->str[j] = s->str[j + 1];
  }

  // Update String Length
  s->len -= 1;

  // Return Removed Character
  return (rc);
}

/*
 * set - Sets The ith Character In Indicated String
 */

char set(string * s, uint32 i, char c) {
  // Verify Parameters
  if (s == NULL) {
    _print_exit("[get] string pointer is NULL");
  } else if (s->str == NULL) {
    _print_exit("[get] string attribute is NULL");
  } else if (i >= (s->len)) {
    _print_exit("[find] string index out of bounds");
  }

  // Store Overwritten Character
  char oc = s->str[i];

  // Set Character In String
  s->str[i] = c;

  // Return Overwritten Character
  return (oc);
}

// End String Access Functions---------------------------------------------------------------------------------------------------------------------------------------------

/*
 * _init - Initializes Library
 */

static void _init(void) {
  // Initialize Synchronization Lock
  pthread_mutex_init(&_mutex, NULL);

  // Set Exit Procedure
  atexit(delete_all);
}

// End Initialization Function---------------------------------------------------------------------------------------------------------------------------------------------
