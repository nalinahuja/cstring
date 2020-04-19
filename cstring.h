#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define ERROR (0)
#define ALLOC_SIZE (15)

// End Includes and Definitions----------------------------------------------------------------------------------------------------------------------------------------------

typedef struct string {
  char * str;
  int cap;
  int len;
} string;

// End String Structure------------------------------------------------------------------------------------------------------------------------------------------------------

string * new_string(void) {
  // Initialize String Instance
  string * s = (string *) malloc(sizeof(string));

  // Set Structure Members
  s->str = (char *) calloc(sizeof(char), ALLOC_SIZE);
  s->cap = ALLOC_SIZE;
  s->len = 0;

  // Return Structure
  return s;
}

// End String Initializer----------------------------------------------------------------------------------------------------------------------------------------------------

void clear(string * s) {
  assert(s);

  // Reset Length Member
  s->len = 0;
}

void delete(string * s) {
  assert(s);

  // Free String Memory
  free(s->str);
  s->str = NULL;

  // Free Structure Memory
  free(s);
  s = NULL;
}

// End Memory Management Functions----------------------------------------000------------------------------------------------------------------------------------------------

int len(string * s) {
  assert(s);

  // Return String Length
  return (s->len);
}

void append(string * s, char * c) {
  assert(s);

  // Calculate Length Of Request
  int req_len = strlen(c);

  // Enough Memory
  if (s->cap >= s->len + req_len) {
    // Copy Request To Structure
    for (int i = 0; i < req_len; i++) {
      s->str[s->len + i] = c[i];
    }

    // Adjust Structure Length
    s->len += req_len;
  }

  // Insufficient Memory
  else {
    // Calculate New Memory Requirements
    int new_cap = s->len + req_len + ALLOC_SIZE;
    char * new_str = (char *) calloc(sizeof(char), new_cap);

    // Copy String To New Memory
    for (int i = 0; i < s->len; i++) {
      new_str[i] = s->str[i];
    }

    // Clear Old Memory
    free(s->str);
    s->str = NULL;

    // Adjust Structure Members
    s->cap = new_cap;
    s->str = new_str;

    // Recursively Add Characters

    append(s, c);
  }
}

void prepend(string * s, char * c) {
  // implement
}

char get(string * s, int i) {
  assert(s);

  // Range Check Index
  if ((i < 0) || (i >= s->len)) {
    return (ERROR);
  } else {
    return (s->str[i]);
  }
}

void put(string * s, int i, char c) {
  // implement
}

void remc(string * s, int i) {
  // implement
}

void remr(string * s, int str, int end) {
  // implement
}

char * strsub(string * s, int str) {
  assert(s);

  // Range Check Indices
  if ((str <= 0) || (str >= s->len)) {
    return NULL;
  }

  // Duplicate String
  char * dup = strdup(s->str + str);
  return dup;
}

char * strnsub(string * s, int str, int end) {
  assert(s);

  // Range Check Indices
  if ((str >= 0) && (end < s->len) && (str < end)) {
    // Duplicate String
    char * dup = strdup(s->str + str);

    // Set String Termination
    *(dup + end) = '\0';
    return dup;
  } else {
    return NULL;
  }
}

// End String Functions------------------------------------------------------------------------------------------------------------------------------------------------------
