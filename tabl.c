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
# define MAX_STRLEN 1024

typedef char* string;

typedef struct row {
  char (*strings)[MAX_STRLEN];
  struct row *next;
}* row;

static row firstRow = NULL;
static row lastRow = NULL;
static int *maxWord = NULL;
static int tableCols;

void cleanUp() {
  row cRow = firstRow, tmp;
  while (cRow) {
    tmp = cRow->next;
    free(cRow->strings);
    free(cRow);
    cRow = tmp;
  }
  firstRow = NULL;
  lastRow = NULL;
  free(maxWord);
}

int countCols(FILE* f) {
  int c, i = 1; //sloupce = èárky + 1
  while ((c = getc(f)) != '\n' && c != EOF) {
     if (c == ',')
        ++i;
  }
  fseek(f, 0, SEEK_SET);
  return i;
}

row addRow() {
  row ptr = malloc(sizeof(struct row));
  if (!ptr)
    err(ERR_MALLOC);
  ptr->next = NULL;
  ptr->strings = malloc(tableCols * MAX_STRLEN);
  if (!ptr->strings)
    err(ERR_MALLOC);

  if (lastRow) {
    lastRow->next = ptr;
    lastRow = ptr;
  }
  else {
    firstRow = lastRow = ptr;
  }

  if (!maxWord) {
    maxWord = calloc(tableCols, sizeof(int));
    if (!maxWord)
      err(ERR_MALLOC);
  }

  return ptr;
}

void readTable(FILE* f) {
  tableCols = countCols(f);
  int c, i = 0, j = 0;

  row cRow = addRow();
  while ((c = getc(f)) != EOF) {
     if (c == ',') {
        cRow->strings[i][j] = '\0';
        ++i;
        j = 0;
        continue;
      }
     if (c == '\n') {i = 0; j = 0; cRow = addRow(); continue;}
     cRow->strings[i][j++] = c;
     if (j > maxWord[i])
        maxWord[i] = j;
  }
}

void printTable() {
  row cRow = firstRow;
  while (cRow) {
    for (int j = 0; j < tableCols; ++j)
      printf("| %*s ", maxWord[j], cRow->strings[j]);
    printf("|\n");
    cRow = cRow->next;
  }
}
int main(int argc, char* argv[]) {
  if (argc != 2)
    err(ERR_ARGC);

  FILE* f = fopen(argv[1], "r");
  if (!f)
    err(ERR_OOPEN, argv[1]);
  readTable(f);
  fclose(f);

  printTable();

  cleanUp();
  return 0;
}
