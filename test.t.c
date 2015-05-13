#include <stdio.h>
#include <stdlib.h>

typedef struct korve {
  char* name;
  int nalepka;
}* korva;

int main() {
  korva *ptr = NULL; int count = 2;
  ptr = malloc(sizeof(korva) * count);

  ptr[0] = malloc(sizeof(struct korve));
  ptr[0]->name = "zmrde";
  ptr[0]->nalepka = count;

  ptr[1] = malloc(sizeof(struct korve));
  ptr[1]->name = "pyco";
  ptr[1]->nalepka = count + 10;

  ptr = realloc(ptr, sizeof(korva) * ++count);

  ptr[count - 1] = malloc(sizeof(struct korve));
  ptr[count - 1]->name = "realloc";
  ptr[count - 1]->nalepka = count + 20;

  for (int i = 0; i < count; i++)
    printf("%s (%d)\n", ptr[i]->name, ptr[i]->nalepka);

  for(;count >0; count--)
    free(ptr[count-1]);
  free(ptr);

  return 0;
}
