#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NSET (-1)
#define ERROR (0)
#define ALLOC (15)

// End Includes and Definitions----------------------------------------------------------------------------------------------------------------------------------------------

typedef struct string {
  char * str;
  int cap;
  int len;
} string;

// End String Structure------------------------------------------------------------------------------------------------------------------------------------------------------

string * cstring(char * init_c) {
  // Calculate Memory Requirements
  size_t init_size = ALLOC;
  size_t req_size = NSET;

  // Extend Memory For Init String
  if (init_c) {
    req_size = strlen(init_c);
    init_size += req_size;
  }

  // Initialize Structure
  string * s = (string *) calloc(sizeof(string), 1);

  // Set Structure Members
  s->str = (char *) calloc(sizeof(char), init_size);
  s->cap = init_size;
  s->len = 0;

  // Copy String To Structure
  if (init_c) {
    strcpy(s->str, init_c);
    s->len = req_size;
  }

  // Return Structure Pointer
  return s;
}

// End String Initializer----------------------------------------------------------------------------------------------------------------------------------------------------

int len(string * s) {
  // Assert Pointer Validity
  assert(s);

  // Return String Length
  return (s->len);
}

char * str(string * s) {
  // Assert Pointer Validity
  assert(s);

  // Return String Length
  return (s->str);
}

// End Field Access Functions------------------------------------------------------------------------------------------------------------------------------------------------

void clear(string * s) {
  // Assert Pointer Validity
  assert((s) && (s->str));

  // Reset Contents Of String
  memset(s->str, 0, s->cap);

  // Reset String Length
  s->len = 0;
}

void delete(string * s) {
  // Assert Pointer Validity
  assert((s) && (s->str));

  // Free String Memory
  free(s->str);
  s->str = NULL;

  // Free Structure Memory
  free(s);
  s = NULL;
}

// End Memory Management Functions-------------------------------------------------------------------------------------------------------------------------------------------

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
      for (int i = s->len; i > ins; i--) {
        s->str[i] = s->str[i - 1];
      }
    }

    // Copy Request String To Structure
    for (int i = 0, j = 0; i < req_len; i++, j++) {
      s->str[ins + i] = c[j];
    }

    // Adjust Structure Length
    s->len += req_len;
  } else {
    // Extend Memory
    int new_cap = req_mem + ALLOC;
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

char get(string * s, int i) {
  // Assert Pointer Validity
  assert((s) && (s->str));

  // Range Check Index
  if ((i < 0) || (i >= s->len)) {
    return (ERROR);
  }

  // Return ith Character In String
  return (s->str[i]);
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

char rem(string * s, int rem) {
  // Assert Pointer Validity
  assert((s) && (s->str));

  // Range Check Index
  if ((rem < 0) || (rem >= s->len)) {
    return (ERROR);
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

char * strsub(string * s, int i) {
  // Assert Pointer Validity
  assert((s) && (s->str));

  // Range Check Index
  if ((i <= 0) || (i >= s->len)) {
    return (NULL);
  }

  // Substring From ith Character
  return (strdup(s->str + i));
}

char * strnsub(string * s, int i, int j) {
  // Assert Pointer Validity
  assert((s) && (s->str));

  // Range Check Indices
  if ((i >= 0) && (j < s->len) && (i < j)) {
    // Duplicate String
    char * dup = strdup(s->str + i);
    dup[j - i] = '\0';

    // Return Substring
    return (dup);
  } else {
    return (NULL);
  }
}

// End String Functions------------------------------------------------------------------------------------------------------------------------------------------------------
