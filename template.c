# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <string.h>
# include <ctype.h>
#
# define INFOTEXT_USAGE "Popis programu"
# define ERR_MALLOC "Out of memory!"
# define ERR_OOPEN "Cant open file '%s'"
#
# define err(...) do{\
    fprintf(stderr, ##__VA_ARGS__);\
    cleanUp(); exit(EXIT_FAILURE);\
  } while(0)

typedef unsigned int uint;
typedef char* string;

void cleanUp() {

}

int main(int argc, char* argv[]) {

}
