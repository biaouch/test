# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <string.h>
# include <ctype.h>
#
# define INFOTEXT_USAGE "Sorts the input files into categories based on their first three bytes.\nUsage: %s file1 [file2 [ ... fileN]]\n"
# define ERRTEXT_MALLOC "Out of memory!"
# define ERRTEXT_FOPEN  "Unable to open the input file '%s'!\n"
# define ERRTEXT_FSIZE  "The input file '%s' is lesser than 3 bytes in size!\n"
#
# define err(...) do{\
    fprintf(stderr, ##__VA_ARGS__);\
    cleanUp(); exit(EXIT_FAILURE);\
  } while(0)

typedef unsigned int uint;
typedef char* string;

typedef struct HashTable {
  string* keys;
  uint*   values;
  uint maxLength;
  uint length;
}* HashTable;

HashTable fileTypes = NULL;

HashTable createTable(uint maxLength) {
  HashTable table = malloc(sizeof(struct HashTable));
  if(table) {
    table->length = 0;
    table->maxLength = maxLength;
    table->keys   = malloc(maxLength * sizeof(string));
    table->values = malloc(maxLength * sizeof(uint));
    if(!table->values) free(table->keys);
    if(!table->keys) {
      free(table);
      table = NULL;
    }
  } return table;
}

void freeTable(HashTable table) {
  for(uint i = 0; i != table->length;)
    free(table->keys[i++]);
  free(table->keys);
  free(table->values);
  free(table);
}

void addItem(HashTable table, string key, uint value) {
  if(table->length == table->maxLength) return;
  table->keys[  table->length] = key;
  table->values[table->length] = value;
  ++table->length;
}

void setItem(HashTable table, string key, uint value) {
  for(uint i = 0; i != table->length; ++i)
    if(!strcmp(table->keys[i], key)) {
      free(table->keys[i]);
      table->keys[i] = key;
      table->values[i] = value; break;
    }
}

uint* getItem(HashTable table, string key) {
  for(uint i = 0; i != table->length; ++i)
    if(!strcmp(table->keys[i], key)) return &(table->values[i]);
  return NULL;
}

void cleanUp() {
  if(fileTypes) freeTable(fileTypes);
}

void strLower(char* string) {
  for ( ; *string; ++string) *string = tolower(*string);
}

void getType(string fileName) {
  FILE* file = fopen(fileName, "rb");
  if(!file) err(ERRTEXT_FOPEN, fileName);
  fseek(file, 0L, SEEK_END);
  if(ftell(file) < 3) err(ERRTEXT_FSIZE, fileName);
  fseek(file, 0L, SEEK_SET);

  string type = malloc(4 * sizeof(string));
  if(!type) err(ERRTEXT_MALLOC);
  memset(type, 0, 4);
  fread(type, 3, 1, file); fclose(file);
  strLower(type);

  uint* pCurrValue = getItem(fileTypes, type);
  if(pCurrValue == NULL) {
    addItem(fileTypes, type, 1);
  } else setItem(fileTypes, type, *pCurrValue + 1);
}

void displayResults() {
  for(uint i = 0; i != fileTypes->length; ++i)
    printf("%s : %u\n", fileTypes->keys[i], fileTypes->values[i]);
}

int main(int argc, char* argv[]) {
  if(argc == 1) printf(INFOTEXT_USAGE);
  else {
    if(!(fileTypes = createTable(argc - 1))) err(ERRTEXT_MALLOC);
    for(int i = 1; i != argc; ++i) getType(argv[i]);
    displayResults();
  } cleanUp();
  return EXIT_SUCCESS;
}
