#include <stdio.h>
#include <stdlib.h>

typedef struct st {
  int a;
} s;

int main() {
  s **ptr = NULL; int count = 2;
  ptr = malloc(sizeof(s*) * count);

  ptr[0] = malloc(sizeof(struct st));
  ptr[1] = malloc(sizeof(struct st));

  ptr = realloc(*ptr, sizeof(s*) * ++count);

  ptr[2] = malloc(sizeof(struct st));

  free(ptr);
  return 0;
}
