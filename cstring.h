#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define CSTRING_NSET (0)
#define CSTRING_ERROR (0)
#define CSTRING_ALLOC (15)

// End Includes and Definitions--------------------------------------------------------------------------------------------------------------------------------------------

typedef struct string {
  char * str;
  int cap;
  int len;
} string;

// End String Structure----------------------------------------------------------------------------------------------------------------------------------------------------

int max_allocs, num_allocs;
string ** allocs = NULL;

// End Alloc Array---------------------------------------------------------------------------------------------------------------------------------------------------------

string * cstring(char * init_c) {
  // Calculate Memory Requirements
  size_t init_size = CSTRING_ALLOC;
  int req_len = CSTRING_NSET;

  // Extend Memory For Init String
  if (init_c) {
    req_len = strlen(init_c);
    init_size += req_len;
  }

  // Initialize Structure
  string * s = (string *) calloc(sizeof(string), 1);

  // Initialize Allocations Array
  if (!allocs) {
    num_allocs = 0;
    max_allocs = CSTRING_ALLOC;
    allocs = (string **) calloc(sizeof(string *), CSTRING_ALLOC);
  } else if (num_allocs >= max_allocs) {
    // Create Resized Allocations Array
    string ** new_allocs = (string **) calloc(sizeof(string *), max_allocs * 2);

    // Copy Old Data To New Array
    for (int i = 0; i < max_allocs; i++) {
      new_allocs[i] = allocs[i];
    }

    // Free Old Memory
    free(allocs);

    // Update Allocations Information
    allocs = new_allocs;
    max_allocs *= 2;
  }

  // Insert Allocation Into Array
  for (int i = 0; i < max_allocs; i++) {
    if (!allocs[i]) {
      allocs[i] = s;
    }
  }

  // Set Structure Members
  s->str = (char *) calloc(sizeof(char), init_size);
  s->cap = init_size;
  s->len = req_len;

  // Copy String To Structure
  if (init_c) {
    strcpy(s->str, init_c);
  }

  // Return Structure Pointer
  return s;
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

void delete() {
  for (int i = 0; i < )

  // Assert Pointer Validity
  assert((s) && (s->str));

  // Free String Memory
  free(s->str);
  s->str = NULL;

  // Free Structure Memory
  free(s);
  s = NULL;
}

// End Memory Management Functions-----------------------------------------------------------------------------------------------------------------------------------------

string * clone(string * s) {
  // Assert Pointer Validity
  assert((s) && (s->str));

  // Return Duplicate String Structure
  return (cstring(strdup(s->str)));
}

string * substr(string * s, int i) {
  // Assert Pointer Validity
  assert((s) && (s->str));

  // Range Check Index
  if ((i <= 0) || (i >= s->len)) {
    return (NULL);
  }

  // Substring From ith Character
  return (cstring(strdup(s->str + i)));
}

string * substrn(string * s, int i, int j) {
  // Assert Pointer Validity
  assert((s) && (s->str));

  // Range Check Indices
  if ((i >= 0) && (j <= s->len) && (i < j)) {
    // Duplicate String
    char * dup = strdup(s->str + i);
    dup[j - i] = 0;

    // Return Substring
    return (cstring(dup));
  } else {
    return (NULL);
  }
}

// End String Duplication Functions----------------------------------------------------------------------------------------------------------------------------------------

void insert(string * s, char * c, int ins) {
  // Assert Pointer Validity
  assert((s) && (s->str) && (c));

  // Range Check Index
  if ((ins < 0) || (ins > s->len)) {
    return;
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
      for (int i = 0; i < req_len; i++) {
        for (int j = s->len; j > ins; j--) {
          s->str[j + i] = s->str[j + i - 1];
        }
      }
    }

    // Copy Request String To Structure
    for (int i = 0; i < req_len; i++) {
      s->str[ins + i] = c[i];
    }

    // Adjust Structure Length
    s->len += req_len;
  } else {
    // Extend Memory
    int new_cap = req_mem + CSTRING_ALLOC;
    char * new_str = (char *) calloc(sizeof(char), new_cap);

    // Copy Old Memory Contents To New Memory
    for (int i = 0; i < s->len; i++) {
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
}

void append(string * s, char * c) {
  insert(s, c, s->len);
}

void prepend(string * s, char * c) {
  insert(s, c, 0);
}

void concat(string * s1, string * s2) {
  append(s1, s2->str);
}

// End String Manipulation Functions---------------------------------------------------------------------------------------------------------------------------------------

char get(string * s, int i) {
  // Assert Pointer Validity
  assert((s) && (s->str));

  // Range Check Index
  if ((i < 0) || (i >= s->len)) {
    return (CSTRING_ERROR);
  }

  // Return ith Character In String
  return (s->str[i]);
}

char rem(string * s, int rem) {
  // Assert Pointer Validity
  assert((s) && (s->str));

  // Range Check Index
  if ((rem < 0) || (rem >= s->len)) {
    return (CSTRING_ERROR);
  }

  // Store Removed Character For Return
  char rem_c = s->str[rem];

  // Left Shift String
  for (int i = rem; i < s->len; i++) {
    s->str[i] = s->str[i + 1];
  }

  // Update String Length
  s->len -= 1;

  // Return Removed Character
  return (rem_c);
}

void set(string * s, int i, char c) {
  // Assert Pointer Validity
  assert((s) && (s->str));

  // Range Check Index
  if ((i < 0) || (i >= s->len)) {
    return;
  }

  // Set ith Character In String
  s->str[i] = c;
}

// End String Access Functions---------------------------------------------------------------------------------------------------------------------------------------------
