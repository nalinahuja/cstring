// Developed by Nalin Ahuja, nalinahuja

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "cstring.h"

// Return Codes
#define CSTRING_OK (0)
#define CSTRING_ERR (1)

// Embedded Values
#define CSTRING_EOL (-1)
#define CSTRING_ALLOC (16)

// End Includes and Definitions--------------------------------------------------------------------------------------------------------------------------------------------

// Allocation List
static string * alloc_list;

// Allocation Mutex Lock
static pthread_mutex_t alloc_mutex;

// End Private Variables---------------------------------------------------------------------------------------------------------------------------------------------------

// Library Initializer
static void cstring_init(void) __attribute__ ((constructor));

// End Function Prototypes-------------------------------------------------------------------------------------------------------------------------------------------------

/*
 * print_exit - Prints An Error Message Before Exiting
 */
static void print_exit(const char * fn, const char * msg) {
  // Flush All Output Streams
  fflush(NULL);

  // Display Error Message
  fprintf(stderr, "\ncstring → [%s]: %s\n", fn, msg);

  // Exit Program
  exit(CSTRING_ERR);
}

/*
 * add_alloc - Adds A String Structure To The Allocation List
 */
static void add_alloc(string * s) {
  // Verify Allocation List Pointer
  if (alloc_list == NULL) {
    // Initialize Allocation List
    alloc_list = s;
  } else {
    // Apppend Allocation To List
    alloc_list->_next = s;
    s->_prev = alloc_list;

    // Increment Allocation List Pointer
    alloc_list = alloc_list->_next;
  }
}

/*
 * remove_alloc - Removes A String Structure From The Allocation List
 */
static void remove_alloc(string * s) {
  // Verify Allocation List Pointer
  if (alloc_list != NULL) {
    // Shift Allocation List Pointer
    if (s == alloc_list) {
      alloc_list = alloc_list->_prev;
    }

    // Unbind Next Node
    if (s->_next != NULL) {
      s->_next->_prev = s->_prev;
    }

    // Unbind Previous Node
    if (s->_prev != NULL) {
      s->_prev->_next = s->_next;
    }
  }
}

// End Private Library Functions-------------------------------------------------------------------------------------------------------------------------------------------

/*
 * cstring - Creates A New String Stucture
 */
string * cstring(char * init_str) {
  // Lock Thread Mutex
  pthread_mutex_lock(&alloc_mutex);

  // Initialize String Structure
  string * s = (string *) malloc(sizeof(string));

  // Verify String Structure Allocation
  if (s == NULL) {
    print_exit(&__func__[0], "failed to allocate new string");
  }

  // Initialize Default String Attributes
  uint init_len = 0, init_mem = CSTRING_ALLOC;

  // Verify Initialization String
  if (init_str != NULL) {
    // Update String Length
    init_len = strlen(init_str);

    // Update String Memory
    init_mem += init_len;
  }

  // Set String Structure Attributes
  s->str = (char *) calloc(sizeof(char), init_mem);
  s->cap = init_mem;
  s->len = init_len;

  // Verify String Attribute Initialization
  if (s->str == NULL) {
    print_exit(&__func__[0], "failed to initialize new string");
  }

  // Verify Initialization String
  if (init_str != NULL) {
    // Copy Initialization String To String Structure
    strncpy(s->str, init_str, init_len);
  }

  // Add String Structure Allocation To List
  add_alloc(s);

  // Unlock Thread Mutex
  pthread_mutex_unlock(&alloc_mutex);

  // Return String Structure Pointer
  return (s);
}

// End String Initializer--------------------------------------------------------------------------------------------------------------------------------------------------

/*
 * cap - Returns Capacity Of Indicated String Allocation
 */
uint cap(string * s) {
  // Verify Parameters
  if (s == NULL) {
    print_exit(&__func__[0], "string pointer is NULL");
  }

  // Return String Capacity
  return (s->cap);
}

/*
 * len - Returns Length Of Indicated String Allocation
 */
uint len(string * s) {
  // Verify Parameters
  if (s == NULL) {
    print_exit(&__func__[0], "string pointer is NULL");
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
    print_exit(&__func__[0], "string pointer is NULL");
  } else if (s->str == NULL) {
    print_exit(&__func__[0], "string attribute is NULL");
  }

  // Return String Pointer
  return (s->str);
}

// End Attribute Access Functions-------------------------------------------------------------------------------------------------------------------------------------------

/*
 * clear - Clears Contents Of The Indicated String Allocation
 */
void clear(string * s) {
  // Verify Parameters
  if (s == NULL) {
    print_exit(&__func__[0], "string pointer is NULL");
  } else if (s->str == NULL) {
    print_exit(&__func__[0], "string attribute is NULL");
  }

  // Reset String Contents
  s->str[0] = 0;

  // Reset String Length
  s->len = 0;
}

/*
 * delete - Frees Memory Of The Indicated String Allocation
 */
void delete(string * s) {
  // Verify Parameters
  if (s == NULL) {
    print_exit(&__func__[0], "string pointer is NULL");
  } else if (s->str == NULL) {
    print_exit(&__func__[0], "string attribute is NULL");
  }

  // Lock Thread Mutex
  pthread_mutex_lock(&alloc_mutex);

  // Remove Structure Allocation From List
  remove_alloc(s);

  // Free String Attribute Memory
  free(s->str);
  s->str = NULL;

  // Free String Structure Memory
  free(s);
  s = NULL;

  // Unlock Thread Mutex
  pthread_mutex_unlock(&alloc_mutex);
}

/*
 * delete_all - Frees Memory Of All String Allocations
 */
void delete_all() {
  // Lock Thead Mutex
  pthread_mutex_lock(&alloc_mutex);

  // Iterate Over Allocation List
  while (alloc_list != NULL) {
    // Get Allocation From List
    string * s = alloc_list;

    // Decrement Allocation List Pointer
    alloc_list = alloc_list->_prev;

    // Free String Attribute Memory
    free(s->str);
    s->str = NULL;

    // Free String Structure Memory
    free(s);
    s = NULL;
  }

  // Unlock Thead Mutex
  pthread_mutex_unlock(&alloc_mutex);
}

// End Memory Management Functions-----------------------------------------------------------------------------------------------------------------------------------------

/*
 * copy - Returns A Copy Of Indicated String
 */
string * copy(string * s) {
  // Verify Parameters
  if (s == NULL) {
    print_exit(&__func__[0], "string pointer is NULL");
  } else if (s->str == NULL) {
    print_exit(&__func__[0], "string attribute is NULL");
  }

  // Return Copy Of String
  return (cstring(s->str));
}

/*
 * substr - Returns A Copy Of Indicated String From [i, len(s))
 */
string * substr(string * s, uint i) {
  // Verify Parameters
  if (s == NULL) {
    print_exit(&__func__[0], "string pointer is NULL");
  } else if (s->str == NULL) {
    print_exit(&__func__[0], "string attribute is NULL");
  } else if (i >= s->len) {
    print_exit(&__func__[0], "string index error");
  }

  // Return Substring From [i, len(s))
  return (cstring(s->str + i));
}

/*
 * substrn - Returns A Copy Of Indicated String From [i, i + n)
 */
string * substrn(string * s, uint i, uint n) {
  // Verify Parameters
  if (s == NULL) {
    print_exit(&__func__[0], "string pointer is NULL");
  } else if (s->str == NULL) {
    print_exit(&__func__[0], "string attribute is NULL");
  } else if (n == 0 || i >= s->len || i + n > s->len) {
    print_exit(&__func__[0], "string index error");
  }

  // Trim String To ith Index
  char * si = s->str + i;

  // Store Removed Character
  char rc = si[n];

  // Set Null Terminator
  si[n] = 0;

  // Trim String Till nth Index
  string * sn = cstring(si);

  // Unset Null Terminator
  si[n] = rc;

  // Return Substring From [i, i + n)
  return (sn);
}

// End String Duplication Functions----------------------------------------------------------------------------------------------------------------------------------------

/*
 * insert - Inserts A Substring At ith Index
 */
int insert(string * s, char * c, uint i) {
  // Verify Parameters
  if (s == NULL) {
    print_exit(&__func__[0], "string pointer is NULL");
  } else if (s->str == NULL) {
    print_exit(&__func__[0], "string attribute is NULL");
  } else if (c == NULL) {
    print_exit(&__func__[0], "insertion string is NULL");
  } else if (i > s->len) {
    print_exit(&__func__[0], "string index error");
  }

  // Calculate Insertion String Length
  uint ins_len = strlen(c);

  // Calculate Updated String Memory
  uint req_mem = s->len + ins_len;

  // Verify Available Memory
  if (s->cap >= req_mem) {
    // Sufficient Memory
    if (i < s->len) {
      // Perform Character Shifts
      for (uint j = 0; j < ins_len; ++j) {
        // Right Shift String
        for (uint k = s->len; k > i; --k) {
          s->str[k + j] = s->str[k + j - 1];
        }
      }
    }

    // Copy Insertion String Into Structure
    for (uint j = 0; j < ins_len; ++j) {
      s->str[i + j] = c[j];
    }

    // Update String Length
    s->len += ins_len;
  } else {
    // Calculate New Memory Requirement
    uint new_mem = (req_mem << 1);

    // Allocate String Memory
    char * new_str = (char *) calloc(sizeof(char), new_mem);

    // Verify Memory Pointer
    if (new_str == NULL) {
      print_exit(&__func__[0], "failed to extend string memory space");
    }

    // Transfer Memory Contents
    for (uint i = 0; i < s->len; i++) {
      new_str[i] = s->str[i];
    }

    // Free Old Memory Contents
    free(s->str);
    s->str = NULL;

    // Update Structure Attributes
    s->str = new_str;
    s->cap = new_mem;

    // Retry Insertion Operation
    insert(s, c, i);
  }

  // Return Success
  return (CSTRING_OK);
}

/*
 * append - Appends A Character String To String Structure
 */
int append(string * s, char * c) {
  // Verify Parameters
  if (s == NULL) {
    print_exit(&__func__[0], "string pointer is NULL");
  } else if (s->str == NULL) {
    print_exit(&__func__[0], "string attribute is NULL");
  } else if (c == NULL) {
    print_exit(&__func__[0], "insertion string is NULL");
  }

  // Perform Append Operation
  return (insert(s, c, s->len));
}

/*
 * prepend - Prepends A Character String To String Structure
 */
int prepend(string * s, char * c) {
  // Verify Parameters
  if (s == NULL) {
    print_exit(&__func__[0], "string pointer is NULL");
  } else if (s->str == NULL) {
    print_exit(&__func__[0], "string attribute is NULL");
  } else if (c == NULL) {
    print_exit(&__func__[0], "insertion string is NULL");
  }

  // Perform Prepend Operation
  return (insert(s, c, 0));
}

/*
 * concat - Concatenates Two String Structures Togehter
 */
int concat(string * s1, string * s2) {
  // Verify Parameters
  if ((s1 == NULL) || (s2 == NULL)) {
    print_exit(&__func__[0], "at least one string pointer is NULL");
  } else if ((s1->str == NULL) || (s2->str == NULL)) {
    print_exit(&__func__[0], "at least one string attribute is NULL");
  }

  // Perform Concatenation Operation
  return (append(s1, s2->str));
}

// End String Manipulation Functions---------------------------------------------------------------------------------------------------------------------------------------

/*
 * find - Determines The Position Of Substring
 */
int find(string * s, char * c) {
  // Verify Parameters
  if (s == NULL) {
    print_exit(&__func__[0], "string pointer is NULL");
  } else if (s->str == NULL) {
    print_exit(&__func__[0], "string attribute is NULL");
  } else if (c == NULL) {
    print_exit(&__func__[0], "pattern string is NULL");
  }

  // Get Substring Position
  char * pos = strstr(s->str, c);

  // Return Substring Position
  return ((pos) ? (pos - (s->str)) : (CSTRING_EOL));
}

/*
 * get - Gets The ith Character In String
 */
char get(string * s, uint i) {
  // Verify Parameters
  if (s == NULL) {
    print_exit(&__func__[0], "string pointer is NULL");
  } else if (s->str == NULL) {
    print_exit(&__func__[0], "string attribute is NULL");
  } else if (i >= s->len) {
    print_exit(&__func__[0], "string index error");
  }

  // Return ith Character
  return (s->str[i]);
}

/*
 * rem - Removes The ith Character In Indicated String
 */

char rem(string * s, uint i) {
  // Verify Parameters
  if (s == NULL) {
    print_exit(&__func__[0], "string pointer is NULL");
  } else if (s->str == NULL) {
    print_exit(&__func__[0], "string attribute is NULL");
  } else if (i >= s->len) {
    print_exit(&__func__[0], "string index error");
  }

  // Store Removed Character
  char rc = s->str[i];

  // Left Shift String
  for (uint j = i; j < s->len; ++j) {
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

char set(string * s, uint i, char c) {
  // Verify Parameters
  if (s == NULL) {
    print_exit(&__func__[0], "string pointer is NULL");
  } else if (s->str == NULL) {
    print_exit(&__func__[0], "string attribute is NULL");
  } else if (i >= s->len) {
    print_exit(&__func__[0], "string index error");
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
 * cstring_init() - Initializes Library
 */
static void cstring_init() {
  // Initialize Synchronization Lock
  pthread_mutex_init(&alloc_mutex, NULL);

  // Set Exit Procedure
  atexit(delete_all);
}

// End Initialization Function---------------------------------------------------------------------------------------------------------------------------------------------
