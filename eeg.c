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
# define TABLE_COLS 18
# define OUTPUT_FILE "X.noff"

typedef unsigned int uint;
typedef char* string;

static int (*data)[TABLE_COLS] = NULL;
static int min[TABLE_COLS];

static uint count = 0;
static uint bufferSize = 16;

void cleanUp() {
  free(data);
}
void read(FILE* f) {
  data = malloc(sizeof(*data) * bufferSize);
  if (!data)
      err(ERR_MALLOC);

  for (;; ++count) {
    if (count == bufferSize) {
      void *tmpPtr = realloc(data, sizeof(*data) * (bufferSize *= 2)); //temp pointer
      if (!tmpPtr) //check realloc, keep ptr to data
        err(ERR_MALLOC);
      data = tmpPtr;
    }

    for (uint j = 0; j < TABLE_COLS; ++j)
    {
      if (fscanf(f, "%d ", &data[count][j]) == -1)
        return;
      //printf("%d ", data[count][j]);
    }
  }
}
void offset() {
  for (uint i = 0; i < count; i++) {
     for (uint j = 4; j < TABLE_COLS; j++) {
        if (i == 0)
          min[j] = data[i][j];
        else
          min[j] = (data[i][j] <  min[j]) ? data[i][j] : min[j];
     }
  }
  for (uint i = 0; i < count; i++) {
   for (uint j = 4; j < TABLE_COLS; j++) {
        data[i][j] -= min[j];
        //printf("%d ", data[i][j]);
   }
  }
}
void write(FILE* f, int min, int max) {
  for (uint i = 0; i < count; i++) {
    for (uint j = 0; j < TABLE_COLS; j++) {
        if (data[i][0] < min || data[i][0] > max)
          break;
        fprintf(f, (j == TABLE_COLS - 1) ? "%d\n" : "%d ", data[i][j]);
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    printf(HELP);
    exit(0);
  }
  if (argc != 4) {
    printf(ERR_ARGC);
    exit(1);
  }

  FILE* f = fopen(argv[1], "rb");
  if (!f)
    err(ERR_OOPEN, argv[1]);
  read(f);
  fclose(f);
  offset();

  FILE* f2 = fopen(OUTPUT_FILE, "w"); //wb pro unix compatible
  if (!f2)
    err(ERR_OOPEN, OUTPUT_FILE);
  write(f2, atoi(argv[2]), atoi(argv[3]));
  fclose(f2);

  //výpis offsetù
  printf("Offsety(1-%d):\n", TABLE_COLS - 4);
  for (int i = 4; i < TABLE_COLS; ++i)
    printf(" %d", min[i]);

  cleanUp();
}
