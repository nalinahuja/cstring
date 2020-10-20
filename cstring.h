// Developed by Nalin Ahuja, nalinahuja22

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#define CSTRING_SUC (1)
#define CSTRING_ERR (0)
#define CSTRING_EOL (-1)
#define CSTRING_ALC (15)

// End Includes and Definitions--------------------------------------------------------------------------------------------------------------------------------------------

// Numerical Types
typedef int int32;
typedef unsigned int uint32;
typedef unsigned char uint8;

// String Type
typedef struct string {
  char * str;
  uint32 cap;
  uint32 len;
  uint32 ind;
} string;

// End Defined Types-------------------------------------------------------------------------------------------------------------------------------------------------------

static pthread_mutex_t mutex;

// End Synchronization Resources-------------------------------------------------------------------------------------------------------------------------------------------

static void _cstring_init(void) __attribute__ ((constructor));

// End Function Prototypes-------------------------------------------------------------------------------------------------------------------------------------------------

/*
 * verify - displays an error message if comparision fails
 */

void _verify(bool cmp, char * err_msg) {
  if (!(cmp)) {
    // Flush All Output Streams
    fflush(NULL);

    // Display Error Message
    printf("\ncstring: %s\n", err_msg);

    // Exit Program
    exit(1);
  }
}

// End Verify Function-----------------------------------------------------------------------------------------------------------------------------------------------------

// String Allocation Table
uint32 max_allocs, num_allocs;
string ** allocs = NULL;

/*
 * _add_struct - adds a string structure to allocation table
 */

void _add_struct(string * s) {
  // Verify Arguments
  _verify(s, "[_add_struct] failed to add string allocation to table");

  // Check Available Memory
  if ((num_allocs + 1) < max_allocs) {
    // Iterate Over Allocation Table
    for (uint32 i = 0; i < max_allocs; ++i) {
      if (!(allocs[i])) {
        // Update Num Allocs
        if (i > num_allocs) {
          num_allocs = i;
        }

        // Set Internal Structure Index
        s->ind = i;

        // Add Structure To Allocation Table
        allocs[i] = s;

        // End Insertion Operation
        break;
      }
    }
  } else {
    // Update Allocation Limit
    max_allocs *= 2;

    // Create Resized Allocation Table
    string ** new_allocs = (string **) calloc(sizeof(string *), max_allocs);

    // Verify Allocation Table
    _verify(new_allocs, "[_add_struct] failed to resize allocation table");

    // Copy Old Memory Contents To New Memory
    for (uint32 i = 0; i < (max_allocs / 2); ++i) {
      new_allocs[i] = allocs[i];
    }

    // Free Old Allocation Data
    free(allocs);
    allocs = new_allocs;

    // Retry Add Operation
    _add_struct(s);
  }
}

/*
 * _remove_struct - removes a string structure to allocation table
 */

void _remove_struct(string * s) {
  // Verify Arguments
  _verify((s) && (s->str), "[_remove_struct] arguments to the function or components of the string structure are null");

  // Remove String Structure By Index
  if ((s->ind < max_allocs) && (allocs[s->ind] == s)) {
    allocs[s->ind] = NULL;
    return;
  }

  // Remove String Structure Via Pointer
  for (uint32 i = 0; i < max_allocs; ++i) {
    // Remove Strucutre From Table
    if (allocs[i] == s) {
      allocs[i] = NULL;
      break;
    }
  }
}

// End Structure Table-----------------------------------------------------------------------------------------------------------------------------------------------------

/*
 * cstring - returns a new string
 */

string * cstring(char * istr) {
  // Lock Mutex
  pthread_mutex_lock(&mutex);

  // Calculate Memory Requirements
  uint32 init_mem = CSTRING_ALC;
  uint32 init_len = 0;

  // Update Memory Requirements
  if (istr) {
    init_len = strlen(istr);
    init_mem += init_len;
  }

  // Initialize String Structure
  string * s = (string *) calloc(sizeof(string), 1);

  // Set Structure Members
  s->str = (char *) calloc(sizeof(char), init_mem);
  s->cap = init_mem;
  s->len = init_len;

  // Verify String Memory
  if (!(s) || !(s->str)) {
    if (!(s)) {
      free(s);
      s = NULL;
    }

    if (!(s->str)) {
      free(s->str);
      s->str = NULL;
    }

    // Verify Arguments
    _verify((s) && (s->str), "[cstring] failed to initialize new structure");
  }

  // Copy String To Structure
  if (istr) {
    strncpy(s->str, istr, init_len);
  }

  // Add Structure To Table
  _add_struct(s);

  // Unlock Mutex
  pthread_mutex_unlock(&mutex);

  // Return  StringStructure Pointer
  return (s);
}

// End String Initializer--------------------------------------------------------------------------------------------------------------------------------------------------

/*
 * cap - returns capacity of indicated string
 */

inline int cap(string * s) {
  // Verify Arguments
  _verify(s, "[cap] the structure is null");

  // Return String Capacity
  return (s->cap);
}

/*
 * len - returns length of indicated string
 */

inline int len(string * s) {
  // Verify Arguments
  _verify(s, "[len] the structure is null");

  // Return String Length
  return (s->len);
}

/*
 * str - returns contents of indicated string
 */

inline char * str(string * s) {
  // Verify Arguments
  _verify((s) && (s->str), "[str] arguments to the function or components of the string structure are null");

  // Return String Pointer
  return (s->str);
}

// End Field Access Functions----------------------------------------------------------------------------------------------------------------------------------------------

/*
 * clear - resets contents of indicated string
 */

void clear(string * s) {
  // Verify Arguments
  _verify((s) && (s->str), "[clear] arguments to the function or components of the string structure are null");

  // Reset Contents Of String
  memset(s->str, 0, s->cap);

  // Reset String Length
  s->len = 0;
}

/*
 * delete - frees memory of indicated string
 */

void delete(string * s) {
  // Verify Arguments
  _verify(s, "[delete] the structure is null");

  // Remove Structure From Table
  _remove_struct(s);

  // Free String Memory
  free(s->str);
  s->str = NULL;

  // Free Structure Memory
  free(s);
  s = NULL;
}

/*
 * delete_all - frees memory of all strings
 */

void delete_all(void) {
  // Verify Allocation Table
  if (allocs) {
    // Free String Allocations
    for (uint32 i = 0; i < max_allocs; ++i) {
      if (allocs[i]) {
        // Free String Memory
        free(allocs[i]->str);
        allocs[i]->str = NULL;

        // Free Structure Memory
        free(allocs[i]);
        allocs[i] = NULL;
      }
    }

    // Free Allocation Table Memory
    free(allocs);
    allocs = NULL;
  }
}

// End Memory Management Functions-----------------------------------------------------------------------------------------------------------------------------------------

/*
 * copy - returns a copy of indicated string
 */

string * copy(string * s) {
  // Verify Arguments
  _verify((s) && (s->str), "[copy] arguments to the function or components of the string structure are null");

  // Return Copy Of String
  return (cstring(s->str));
}

/*
 * substr - returns a copy of indicated string from [i, len(s)]
 */

string * substr(string * s, uint32 i) {
  // Verify Arguments
  _verify((s) && (s->str), "[substr] arguments to the function or components of the string structure are null");

  // Return Substring From [i, len(s)]
  return ((i >= s->len) ? (NULL) : (cstring(s->str + i)));
}

/*
 * substrn - returns a copy of indicated string from [i, j)
 */

string * substrn(string * s, uint32 i, uint32 j) {
  // Verify Arguments
  _verify((s) && (s->str), "[substrn] arguments to the function or components of the string structure are null");

  // Range Check Indices
  if ((i < j) && (j <= s->len)) {
    // Get Substring From ith Index
    char * dup = (s->str + i);

    // Store Removed Character
    char rc = dup[j - i];

    // Set Null Terminator
    dup[j - i] = 0;

    // Create Substring
    string * sub = (cstring(dup));

    // Unset Null Terminator
    dup[j - i] = rc;

    // Return Duplicate String From [i, j)
    return (sub);
  }

  // Return NULL Pointer
  return (NULL);
}

// End String Duplication Functions----------------------------------------------------------------------------------------------------------------------------------------

/*
 * insert - inserts a substring at kth index
 */

bool insert(string * s, char * c, uint32 k) {
  // Verify Arguments
  _verify((s) && (s->str) && (c), "[insert] arguments to the function or components of the string structure are null");

  // Verify Index Range
  if (k > (s->len)) {
    return (CSTRING_ERR);
  }

  // Calculate Insertion String Length
  uint32 req_len = strlen(c);

  // Calculate Memory Requirements
  uint32 str_mem = (s->cap);
  uint32 req_mem = (s->len + req_len);

  // Compare Memory Values
  if (str_mem >= req_mem) {
    // Sufficient Memory
    if (k < s->len) {
      // Perform Shifts
      for (uint32 i = 0; i < req_len; --i) {
        // Right Shift String
        for (uint32 j = s->len; j > k; --j) {
          s->str[j + i] = s->str[j + i - 1];
        }
      }
    }

    // Copy Insertion String To Structure
    for (uint32 i = 0; i < req_len; ++i) {
      s->str[k + i] = c[i];
    }

    // Update String Length
    s->len += req_len;
  } else {
    // Insufficent Memory
    uint32 new_mem = (req_mem + CSTRING_ALC);

    // Allocate New String Memory Space
    char * new_str = (char *) calloc(sizeof(char), new_mem);

    // Verify New String Memory Space
    _verify(new_str, "[insert] failed to resize string memory space");

    // Copy Old Memory Contents To New Memory
    for (int i = 0; i < s->len; i++) {
      new_str[i] = s->str[i];
    }

    // Free Old Memory Contents
    free(s->str);
    s->str = NULL;

    // Update String Members
    s->str = new_str;
    s->cap = new_mem;

    // Retry Insertion Operation
    insert(s, c, k);
  }

  // Return Success
  return (CSTRING_SUC);
}

/*
 * append - appends a character string to string structure
 */

inline bool append(string * s, char * c) {
  return (insert(s, c, s->len));
}

/*
 * prepend - prepends a character string to string structure
 */

inline bool prepend(string * s, char * c) {
  return (insert(s, c, 0));
}

/*
 * concat - concatenates two string structres into a new string struture
 */

inline bool concat(string * s1, string * s2) {
  return (append(s1, s2->str));
}

// End String Manipulation Functions---------------------------------------------------------------------------------------------------------------------------------------

/*
 * find - determines the position of substring
 */

int32 find(string * s, char * c) {
  // Verify Arguments
  _verify((s) && (s->str) && (c), "[find] arguments to the function or components of the string structure are null");

  // Get Substring Position
  char * pos = strstr(s->str, c);

  // Return Substring Position
  return ((pos) ? (pos - (s->str)) : (CSTRING_EOL));
}

/*
 * get - gets the ith character in string
 */

uint8 get(string * s, uint32 i) {
  // Verify Arguments
  _verify((s) && (s->str), "[get] arguments to the function or components of the string structure are null");

  // Return Character
  return ((i >= (s->len)) ? (CSTRING_ERR) : (s->str[i]));
}

/*
 * rem - removes the ith character in string
 */

uint8 rem(string * s, uint32 i) {
  // Verify Arguments
  _verify((s) && (s->str), "[rem] arguments to the function or components of the string structure are null");

  // Verify Index Range
  if (i >= (s->len)) {
    // Return Error
    return (CSTRING_ERR);
  } else {
    // Store Removed Character
    uint8 rc = s->str[i];

    // Left Shift String
    for (uint32 j = i; j < (s->len); ++j) {
      s->str[j] = s->str[j + 1];
    }

    // Update String Length
    s->len -= 1;

    // Return Removed Character
    return (rc);
  }
}

/*
 * set - sets the ith character in string
 */

uint8 set(string * s, uint32 i, uint8 c) {
  // Verify Arguments
  _verify((s) && (s->str), "[set] arguments to the function or components of the string structure are null");

  // Verify Index Range
  if (i >= (s->len)) {
    // Return Error
    return (CSTRING_ERR);
  } else {
    // Store Overwritten Character
    uint8 oc = s->str[i];

    // Set ith Character In String
    s->str[i] = c;

    // Return Overwritten Character
    return (oc);
  }
}

// End String Access Functions---------------------------------------------------------------------------------------------------------------------------------------------

/*
 * _cstring_init - initializes cstring program
 */

static void _cstring_init(void) {
  // Initialize Mutex Lock
  pthread_mutex_init(&mutex, NULL);

  // Initialize Allocations Counters
  max_allocs = CSTRING_ALC, num_allocs = 0;

  // Initialize Allocation Table
  allocs = (string **) calloc(sizeof(string *), max_allocs);

  // Verify Allocation Table
  _verify(allocs, "[_add_struct] failed initialize allocation table");

  // Set Exit Procedure
  atexit(delete_all);
}

// End Initializer Function------------------------------------------------------------------------------------------------------------------------------------------------
