# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <string.h>
# include <ctype.h>
#
# define INFOTEXT_USAGE "Popis programu"
# define ERR_MALLOC "Out of memory!"
# define ERR_OOPEN "Cant open output file '%s'"
#
# define err(...) do{\
    fprintf(stderr, ##__VA_ARGS__);\
    cleanUp(); exit(EXIT_FAILURE);\
  } while(0)

typedef unsigned int uint;
typedef char* string;

static const string output = "files.txt";

typedef struct files {
  string* names;
  long* sizes;
  uint count;
  uint maxCount;
}*  files;

files table = NULL;

void freeTable(files f) {
  if (f) {
    free(f->names);
    free(f->sizes);
    free(f);
  }
}

void cleanUp() {
  freeTable(table);
}

files initTable(int maxCount) {
  files f = malloc(sizeof(struct files));
  if (f) {
    f->count = 0;
    f->maxCount = maxCount;
    f->names = malloc(maxCount * sizeof(string));
    f->sizes = malloc(maxCount * sizeof(long));
    if (!f->names || !f->sizes) {
      freeTable(f);
      f = NULL;
    }
  }
  return f;
}

void addFile(files f, string name) {
  FILE* fc;
  long size;
  if (!(fc = fopen(name, "rb"))) {
    size = -1;
  } else {
    fseek(fc, 0, SEEK_END);
    size = ftell(fc);
    fclose(fc);
  }
  f->names[f->count] = name;
  f->sizes[f->count] = size;
  ++f->count;
}

void printTable(files f) {
  FILE* fc;
  if (!(fc = fopen(output, "w")))
    err(ERR_OOPEN, output);
  for(uint i = 0; i < f->count; ++i)
  //    printf("%s = %d\n", f->names[i], f->sizes[i]);
  fprintf(fc, "%s = %d\n", f->names[i], f->sizes[i]);
  fclose(fc);
}

int main(int argc, char* argv[]) {
  if(argc == 1) {
    printf(INFOTEXT_USAGE);
    exit(EXIT_SUCCESS);
  }
  if (!(table = initTable(argc - 1)))
    err(ERR_MALLOC);
  for(int i = 1; i < argc; ++i)
    addFile(table, argv[i]);
  printTable(table);
  cleanUp();
  return EXIT_SUCCESS;
}
