#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdbool.h>

void thirdTask(int i, int lineAmount, char str[2000][200]) ;

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
    int owner = open("owner", O_WRONLY | O_CREAT | O_TRUNC, 0777);
    int group = open("group", O_WRONLY | O_CREAT | O_TRUNC, 0777);
    int other = open("other", O_WRONLY | O_CREAT | O_TRUNC, 0777);
    int nothing = open("nothing", O_WRONLY | O_CREAT | O_TRUNC, 0777);
    bool isInFile = false;
    for (i = 0; i < lineAmount; i++) {
        isInFile = false;
        if (str[i][1] == 'r' && str[i][2]=='w' && str[i][3]=='x') {
            write(owner, str[i], strlen(str[i]));
            write(owner, "\n", 1);
            isInFile = true;
        }
        if (str[i][4] == 'r' && str[i][5]=='w' && str[i][6]=='x') {
            write(group, str[i], strlen(str[i]));
            write(group, "\n", 1);
            isInFile = true;
        }
        if (str[i][7] == 'r' && str[i][8]=='w' && str[i][9]=='x') {
            write(other, str[i], strlen(str[i]));
            write(other, "\n", 1);
            isInFile = true;
        }
        if (!isInFile) {
            write(nothing, str[i], strlen(str[i]));
            write(nothing, "\n", 1);
            isInFile = true;
        }
    }
    close(owner);
    close(group);
    close(other);
    close(nothing);
}
