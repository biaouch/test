-Wall -pedantic -Wextra -std=c99

for ( ; *p; ++p) *p = tolower(*p); //lower string

# define err(...) do{\
  fprintf(stderr, ##__VA_ARGS__);\
  cleanUp(); exit(EXIT_FAILURE);\
} while(0)

void free2DArray(void** array, int row){
    for (row -= 1; row >= 0; row--)
        free( array[row] );
     free(array);
}

for (int i = 0, i < im, ++i)
    for (int j = 0, j < im, ++j)
