#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
FILE *fptr;
int *column;
int main(int argc, char *argv[]){
    fptr = fopen(argv[1], "r");

    if (fptr == NULL){
        printf("Error opening file\n");
        return 1;
    }

    char c;
    while ((c = fgetc(fptr)) != EOF){
        int num = scan_line_for_treasure(fptr);
    }

    fclose(fptr);
    return 0;
}

int scan_line_for_treasure(FILE *fptr){
    char buffer[1001];//length of each line
    if(fgets(buffer, sizeof(buffer), fptr) == NULL){
      return 0;  // end of file or error
    }

    for(int i = 0; buffer[i] != '\0'; i++){
      if(buffer[i] == '1'){
            *column = i;
            return 1;
      }
    }

    *column = 0; //nothing found
    return 0;
}