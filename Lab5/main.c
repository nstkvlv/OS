#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdbool.h>

int main() {
    int i = 0, lineAmount = 0;
    char stringToRead[2000], str[2000][200];
    if(read(0, stringToRead, 2000) <= 0) {
        printf("Can't read.\n");
    };
    char *line = strtok(stringToRead, "\n");
    while (line != NULL) {
        strcpy(str[lineAmount], line);
        line = strtok(NULL, "\n");
        lineAmount++;
    }
    
    for (i = 0; i < lineAmount; i++) {
        if (str[i][1] == 'r' && str[i][2]=='w' && str[i][3]=='x') {
            str[i][1] = 'B';
            str[i][2] = 'C';
            str[i][3] = 'E';
        }
        if (str[i][4] == 'r' && str[i][5]=='w' && str[i][6]=='x') {
            str[i][4] = 'B';
            str[i][5] = 'C';
            str[i][6] = 'E';
        }
        if (str[i][7] == 'r' && str[i][8]=='w' && str[i][9]=='x') {
            str[i][7] = 'B';
            str[i][8] = 'C';
            str[i][9] = 'E';
        }
        printf("%s \n", str[i]);
    }
}
