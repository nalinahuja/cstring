#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#define CSTRING_SUC (1)
#define CSTRING_ERR (0)
#define CSTRING_EOL (-1)
#define CSTRING_ALC (15)

// TODO, create own assert function

// End Includes and Definitions--------------------------------------------------------------------------------------------------------------------------------------------

typedef struct string {
  char * str;
  int cap;
  int len;
} string;

// End String Structure----------------------------------------------------------------------------------------------------------------------------------------------------

static pthread_mutex_t mutex;

// End Synchronization Mutex-----------------------------------------------------------------------------------------------------------------------------------------------

static void _cstring_init(void) __attribute__ ((constructor));

// End Function Prototypes-------------------------------------------------------------------------------------------------------------------------------------------------

int max_allocs, num_allocs;
string ** allocs = NULL;

void _add_struct(string * s) {
  // Assert Pointer Validity
  assert(s);

  // Initialize Allocs Map
  if (!allocs) {
    max_allocs = CSTRING_ALC, num_allocs = 0;
    allocs = (string **) calloc(sizeof(string *), CSTRING_ALC);
    assert(allocs);
  }

  // Determine Available Space
  if ((num_allocs + 1) < max_allocs) {
    // Iterate Over Allocs Map
    for (int i = 0; i < max_allocs; ++i) {
      if (!allocs[i]) {
        // Update Num Allocs
        if (i > num_allocs) {
          num_allocs = i
        }

        // Add Structure To Allocs
        allocs[i] = s;
        break;
      }
    }
  } else {
    // Update Space Requirements
    max_allocs *= 2;

    // Create Resized Allocs Array
    string ** new_allocs = (string **) calloc(sizeof(string *), max_allocs);
    assert(new_allocs);

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
  assert((s) && (s->str));

  // Find String Structure
  for (int i = 0; i < num_allocs; ++i) {
    if (allocs[i] == s) {
      allocs[i] = NULL;
      break;
    }
  }
}

// End Structure Map-------------------------------------------------------------------------------------------------------------------------------------------------------

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
  assert((s) && (s->str));

  // Add Structure To Map
  _add_struct(s);

  // Unlock Mutex
  pthread_mutex_unlock(&mutex);

  // Return Structure Pointer
  return (s);
}

// End String Initializer--------------------------------------------------------------------------------------------------------------------------------------------------

int len(string * s) {
  // Assert Pointer Validity
  assert(s);

  // Return String Length
  return (s->len);
}

int cap(string * s) {
  // Assert Pointer Validity
  assert(s);

  // Return String Length
  return (s->cap);
}

char * str(string * s) {
  // Assert Pointer Validity
  assert(s);

  // Return String Length
  return (s->str);
}

// End Field Access Functions----------------------------------------------------------------------------------------------------------------------------------------------

void clear(string * s) {
  // Assert Pointer Validity
  assert((s) && (s->str));

  // Reset Contents Of String
  memset(s->str, 0, s->cap);

  // Reset String Length
  s->len = 0;
}

void delete(string * s) {
  // Remove Structure From Map
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
    for (int i = 0; i < num_allocs; ++i) {
      if (allocs[i]) {
        // Free String Memory
        free(allocs[i]->str);
        allocs[i]->str = NULL;

        // Free Structure Memory
        free(allocs[i]);
        allocs[i] = NULL;
      }
    }

    // Free Map Memory
    free(allocs);
    allocs = NULL;
  }
}

// End Memory Management Functions-----------------------------------------------------------------------------------------------------------------------------------------

string * copy(string * s) {
  // Assert Pointer Validity
  assert((s) && (s->str));

  // Return Duplicate String
  return (cstring(s->str));
}

string * substr(string * s, int i) {
  // Assert Pointer Validity
  assert((s) && (s->str));

  // Range Check Index
  if ((i < 0) || (i >= s->len)) {
    return (NULL);
  }

  // Return Duplicate String From [i, len)
  return (cstring(s->str + i));
}

string * substrn(string * s, int i, int j) {
  // Assert Pointer Validity
  assert((s) && (s->str));

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
  assert((s) && (s->str) && (c));

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
    assert(new_str);

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

bool append(string * s, char * c) {
  return (insert(s, c, s->len));
}

bool prepend(string * s, char * c) {
  return (insert(s, c, 0));
}

bool concat(string * s1, string * s2) {
  return (append(s1, s2->str));
}

// End String Manipulation Functions---------------------------------------------------------------------------------------------------------------------------------------

int find(string * s, char * c) {
  char * pos = strstr(s->str, c);

  if (pos) {
    return (int) (pos - (s->str));
  } else {
    return (CSTRING_EOL);
  }
}

char get(string * s, int i) {
  // Assert Pointer Validity
  assert((s) && (s->str));

  // Range Check Index
  if ((i < 0) || (i >= s->len)) {
    return (CSTRING_ERR);
  }

  // Return ith Character In String
  return (s->str[i]);
}

char rem(string * s, int i) {
  // Assert Pointer Validity
  assert((s) && (s->str));

  // Range Check Index
  if ((i < 0) || (i >= s->len)) {
    return (CSTRING_ERR);
  }

  // Store Removed Character For Return
  char rem_c = s->str[i];

  // Left Shift String
  for (int j = i; j < s->len; j++) {
    s->str[j] = s->str[j + 1];
  }

  // Update String Length
  s->len -= 1;

  // Return Removed Character
  return (rem_c);
}

bool set(string * s, int i, char c) {
  // Assert Pointer Validity
  assert((s) && (s->str));

  // Assert Range
  if ((i < 0) || (i >= s->len)) {
    return (CSTRING_ERR);
  }

  // Set ith Character In String
  s->str[i] = c;

  // Return Success
  return (CSTRING_SUC);
}

// End String Access Functions---------------------------------------------------------------------------------------------------------------------------------------------

static void _cstring_init(void) {
  // Initialize Mutex Lock
  pthread_mutex_init(&mutex, NULL);

  // Set Exit Procedure
  atexit(delete_all);
}

// End Initializer Function------------------------------------------------------------------------------------------------------------------------------------------------
