// Developed by Nalin Ahuja, nalinahuja22

#include <assert.h>
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

// String Struct Type
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

static void _verify(bool cmp, char * err_msg) {
  if (!cmp) {
    // Flush All Output Streams
    fflush(NULL);

    // Display Error Message
    kprintf("\ncstring: %s\n", err_msg);

    // Exit Program
    exit(1);
  }
}

// End Verify Function-----------------------------------------------------------------------------------------------------------------------------------------------------

int max_allocs, num_allocs;
string ** allocs = NULL;

void _add_struct(string * s) {
  // Assert Pointer Validity
  _verify(s, "[_add_struct] failed to add string to table");

  // Initialize Allocs Table
  if (!allocs) {
    max_allocs = CSTRING_ALC, num_allocs = 0;
    allocs = (string **) calloc(sizeof(string *), CSTRING_ALC);
    _verify(allocs, "[_add_struct] failed initialize structure table");
  }

  // Determine Available Space
  if ((num_allocs + 1) < max_allocs) {
    // Iterate Over Allocs Table
    for (int i = 0; i < max_allocs; ++i) {
      if (!allocs[i]) {
        // Update Num Allocs
        if (i > num_allocs) {
          num_allocs = i;
        }

        // Set Internal Structure Index
        s->ind = i;

        // Add Structure To Allocs
        allocs[i] = s;

        // End Insertion Operation
        break;
      }
    }
  } else {
    // Update Space Requirements
    max_allocs *= 2;

    // Create Resized Allocs Array
    string ** new_allocs = (string **) calloc(sizeof(string *), max_allocs);
    _verify(new_allocs, "[_add_struct] failed to resize structure table");

    // Copy Alloc Data To Resized Array
    for (int i = 0; i < (max_allocs / 2); ++i) {
      new_allocs[i] = allocs[i];
    }

    // Free Alloc Data
    free(allocs);
    allocs = new_allocs;

    // Retry Add Operation
    _add_struct(s);
  }
}

void _remove_struct(string * s) {
  // Assert Pointer Validity
  _verify((s) && (s->str), "[_remove_struct] one or more components of the structure are NULL");

  // Find String Structure Via Index
  if ((s->ind < max_allocs) && (allocs[s->ind]) && (allocs[s->ind] == s)) {
    allocs[s->ind] = NULL;
    return;
  }

  // Find String Structure Via Pointer
  for (int i = 0; i < max_allocs; ++i) {
    // Remove Structure From Table On Match
    if (allocs[i] == s) {
      allocs[i] = NULL;
      break;
    }
  }
}

// End Structure Table-----------------------------------------------------------------------------------------------------------------------------------------------------

string * cstring(char * init_str) {
  // Lock Mutex
  pthread_mutex_lock(&mutex);

  // Calculate Memory Requirements
  size_t init_size = CSTRING_ALC;
  int req_len = 0;

  // Extend Memory For Init String
  if (init_str) {
    req_len = strlen(init_str);
    init_size += req_len;
  }

  // Initialize Structure
  string * s = (string *) calloc(sizeof(string), 1);

  // Set Structure Members
  s->str = (char *) calloc(sizeof(char), init_size);
  s->cap = init_size;
  s->len = req_len;

  // Copy String To Structure
  if (init_str) {
    strcpy(s->str, init_str);
  }

  // Assert Pointer Validity
  _verify((s) && (s->str), "[cstring] one or more components of the structure are NULL");

  // Add Structure To Table
  _add_struct(s);

  // Unlock Mutex
  pthread_mutex_unlock(&mutex);

  // Return Structure Pointer
  return (s);
}

// End String Initializer--------------------------------------------------------------------------------------------------------------------------------------------------

inline int len(string * s) {
  // Assert Pointer Validity
  _verify(s, "[len] the structure is NULL");

  // Return String Length
  return (s->len);
}

inline int cap(string * s) {
  // Assert Pointer Validity
  _verify(s, "[cap] the structure is NULL");

  // Return String Length
  return (s->cap);
}

inline char * str(string * s) {
  // Assert Pointer Validity
  _verify((s) && (s->str), "[str] one or more components of the structure are NULL");

  // Return String Length
  return (s->str);
}

// End Field Access Functions----------------------------------------------------------------------------------------------------------------------------------------------

inline void clear(string * s) {
  // Assert Pointer Validity
  _verify((s) && (s->str), "[clear] one or more components of the structure are NULL");

  // Reset Contents Of String
  memset(s->str, 0, s->cap);

  // Reset String Length
  s->len = 0;
}

inline void delete(string * s) {
  // Remove Structure From Table
  _remove_struct(s);

  // Free String Memory
  free(s->str);
  s->str = NULL;

  // Free Structure Memory
  free(s);
  s = NULL;
}

void delete_all(void) {
  if (allocs) {
    // Free Allocs
    for (int i = 0; i < max_allocs; ++i) {
      if (allocs[i]) {
        // Free String Memory
        free(allocs[i]->str);
        allocs[i]->str = NULL;

        // Free Structure Memory
        free(allocs[i]);
        allocs[i] = NULL;
      }
    }

    // Free Table Memory
    free(allocs);
    allocs = NULL;
  }
}

// End Memory Management Functions-----------------------------------------------------------------------------------------------------------------------------------------

string * copy(string * s) {
  // Assert Pointer Validity
  _verify((s) && (s->str), "[copy] one or more components of the structure are NULL");

  // Return Duplicate String
  return (cstring(s->str));
}

string * substr(string * s, int i) {
  // Assert Pointer Validity
  _verify((s) && (s->str), "[substr] one or more components of the structure are NULL");

  // Range Check Index
  if ((i < 0) || (i >= s->len)) {
    return (NULL);
  }

  // Return Duplicate String From [i, len)
  return (cstring(s->str + i));
}

string * substrn(string * s, int i, int j) {
  // Assert Pointer Validity
  _verify((s) && (s->str), "[substrn] one or more components of the structure are NULL");

  // Range Check Indices
  if ((i >= 0) && (j <= s->len) && (i < j)) {
    // Duplicate String
    char * sdup = s->str + i;
    char rem_c = sdup[j - i];

    // Set Null Terminator
    sdup[j - i] = 0;

    // Create Substring
    string * sub = (cstring(sdup));

    // Unset Null Terminator
    sdup[j - i] = rem_c;

    // Return Duplicate String From [i, j)
    return (sub);
  }

  return (NULL);
}

// End String Duplication Functions----------------------------------------------------------------------------------------------------------------------------------------

bool insert(string * s, char * c, int ins) {
  // Assert Pointer Validity
  _verify((s) && (s->str) && (c), "[insert] one or more components of the structure and or arguments to the function are NULL");

  // Assert Range
  if ((ins < 0) || (ins > s->len)) {
    return (CSTRING_ERR);
  }

  // Calculate Length Of Request
  int req_len = strlen(c);

  // Calculate Memory Requirements
  int str_mem = s->cap;
  int req_mem = s->len + req_len;

  // Sufficient Memory
  if (str_mem >= req_mem) {
    // Shift Required
    if (ins < s->len) {
      // Right Shift String
      for (int i = 0; i < req_len; ++i) {
        for (int j = s->len; j > ins; j--) {
          s->str[j + i] = s->str[j + i - 1];
        }
      }
    }

    // Copy Request String To Structure
    for (int i = 0; i < req_len; ++i) {
      s->str[ins + i] = c[i];
    }

    // Adjust Structure Length
    s->len += req_len;
  } else {
    // Extend Memory
    int new_cap = req_mem + CSTRING_ALC;
    char * new_str = (char *) calloc(sizeof(char), new_cap);
    _verify(new_str, "[insert] failed to resize string memory space");

    // Copy Old Memory Contents To New Memory
    for (int i = 0; i < s->len; ++i) {
      new_str[i] = s->str[i];
    }

    // Free Old Memory Contents
    free(s->str);
    s->str = NULL;

    // Update Structure Members
    s->str = new_str;
    s->cap = new_cap;

    // Retry Append Operation
    insert(s, c, ins);
  }

  // Return Success
  return (CSTRING_SUC);
}

inline bool append(string * s, char * c) {
  return (insert(s, c, s->len));
}

inline bool prepend(string * s, char * c) {
  return (insert(s, c, 0));
}

inline bool concat(string * s1, string * s2) {
  return (append(s1, s2->str));
}

// End String Manipulation Functions---------------------------------------------------------------------------------------------------------------------------------------

inline int find(string * s, char * c) {
  // Get Character Position
  char * pos = strstr(s->str, c);

  // Determine Position Validity
  if (pos) {
    return (int) (pos - (s->str));
  } else {
    return (CSTRING_EOL);
  }
}

/*
 * get - gets the ith character in string
 */

uint8 get(string * s, uint32 i) {
  // Verify Arguments
  _verify((s) && (s->str), "[get] arguments to the function or components of the string structure are null");

  // Verify Index Range
  if (i >= (s->len)) {
    // Return Error
    return (CSTRING_ERR);
  } else {
    // Return ith Character In String
    return (s->str[i]);
  }
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
    for (uint32 j = i; j < (s->len); j++) {
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

unit8 set(string * s, uint32 i, uint8 c) {
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

static void _cstring_init(void) {
  // Initialize Mutex Lock
  pthread_mutex_init(&mutex, NULL);

  // TODO, setup memory table here

  // Set Exit Procedure
  atexit(delete_all);
}

// End Initializer Function------------------------------------------------------------------------------------------------------------------------------------------------
