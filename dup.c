# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <string.h>
# include <ctype.h>
#
# define HELP "Popis programu"
# define ERR_ARGC "Wrong number of arguments"
# define ERR_MALLOC "Out of memory!"
# define ERR_OOPEN "Cant open file '%s'"
#
# define err(...) do{\
    fprintf(stderr, ##__VA_ARGS__);\
    cleanUp(); exit(EXIT_FAILURE);\
  } while(0)
#
# define MAX_ROW 256

typedef struct files {
  char *ifil;
  char *ofil;
  int rep;
}* files;

static int bufferSize = 16;

files *fi = NULL;
int count = 0;

void cleanUp() {
  if (fi) {
    for(int i = 0; i < count; ++i)
      if (fi[i]) {
        free(fi[i]->ifil);
        free(fi[i]->ofil);
      }
    free(fi);
  }
}

void read(FILE* f, int maxrows) {
  fi = malloc(sizeof(struct files) * bufferSize);
  if (!fi)
      err(ERR_MALLOC);

  for (;count < maxrows; ++count) {
    if (count == bufferSize) {
      void *tmpPtr = realloc(fi, sizeof(struct files) * (bufferSize *= 2)); //temp pointer
      if (!tmpPtr) //check realloc, keep ptr to data
        err(ERR_MALLOC);
      fi = tmpPtr;
    }
    //if (fscanf(f, "%s@%s:%d\n", fi[count]->ifil, fi[count]->ofil, &fi[count]->rep) == -1)
    //  return;

    fi[count]->rep = 5;
    strcpy(fi[count]->ifil, "cif.txt");
    strcpy(fi[count]->ofil, "cif2.txt");

    //printf("%s ", fi[count]->ifil);
  }
}

void modify() {
  for (int i = 0; i < count; i++) {
      if (strcmp(fi[i]->ifil, fi[i]->ofil) == 0) {
        char* tmp = strcat(fi[i]->ofil, ".res");
        if (!tmp)
          err(ERR_MALLOC);
        free(fi[i]->ofil);
        fi[i]->ofil = tmp;
      }
  }
}

void write() {
  int i, j, c;
  FILE *f, *f2;
  for (i = 0; i < count; i++) {
    f = fopen(fi[i]->ifil, "r");
    if (!f) err(ERR_OOPEN, fi[i]->ifil);
    f2 = fopen(fi[i]->ofil, "w");
    if (!f2) err(ERR_OOPEN, fi[i]->ofil);

    while ((c = getc(f)) != EOF) {
       if (c >= 64 && c <= 73)
          for (j = 0; j < fi[i]->rep; j++)
              putc(c, f2);
        else
          putc(c, f2);

    }
    fclose(f);
    fclose(f2);
  }
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf(ERR_ARGC);
    exit(1);
  }
  FILE* f = fopen(argv[1], "rb");
  if (!f)
    err(ERR_OOPEN, argv[1]);
  read(f, atoi(argv[2]));
  fclose(f);

  //write();

  //cleanUp();
  printf("%d radku zpracovano", count);
  return 0;
}
