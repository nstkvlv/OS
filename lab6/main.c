#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

int f;
char name1[] = "1fifo";
char name2[] = "2fifo";
size_t size = 6;

bool isStringWithOneNumber(char str[]) {
    char numbers[] = "123456789";
    int num = 0;
    for(int i = 0; i < strlen(str); i++) {
        for(int j = 0; j < strlen(numbers); j++) {
            if (str[i] == numbers[j]) {
                num++;
            }
        }
    }
    return num == 1 ? true : false;
}

void handler() {
    printf("Handler work\n");
    printf("Child pid: %d \n", getpid());

    f = open(name1, O_RDWR);
    if (f < 0) {
        printf("Can\'t open FIFO1 for reading\n");
        return;
    }
    
    char resStr1[50];
    char resStr2[50];
    char resStr3[50];
    char resStr4[50];
    char resStr5[50];

    read(f, resStr1, size);
    read(f, resStr2, size);
    read(f, resStr3, size);
    read(f, resStr4, size);
    read(f, resStr5, size);
    
    close(f);
    printf("Child read 5 strings\n");
    printf("%s\n", resStr1);
        
    if(mkfifo(name2, 0666) < 0) {
        printf("Can't create FIFO %d \n ", errno);
        return;
    }

    f = open(name2, O_RDWR);
    if (f < 0) {
        printf("Can\'t open FIFO2 for reading\n");
        return;
    }
    
    if (isStringWithOneNumber(resStr1)) {
        write(f, resStr1, strlen(resStr1));
    }

    if (isStringWithOneNumber(resStr2)) {
        write(f, resStr2, strlen(resStr2));
    }

    if (isStringWithOneNumber(resStr3)) {
        write(f, resStr3, strlen(resStr3));
    }
    if (isStringWithOneNumber(resStr4)) {
        write(f, resStr4, strlen(resStr4));
    }

    if (isStringWithOneNumber(resStr5)) {
        write(f, resStr5, strlen(resStr5));
    }

    printf("Child (pid: %d) send strings with one number \n", getpid());
    printf("Child exit \n\n");
    close(f);
    kill(getppid(), SIGUSR1);
}

int main() {
    (void)umask(0);

    int p0 = fork();
    if (p0 < 0) {
        printf("Can't fork child \n");
        return -1;

    } else if (p0 > 0) { //parent process
        if(mkfifo(name1, 0666) < 0) {
            printf("Can't create FIFO %d \n ", errno);
            return -1;
        }
        
        char str1[50];
        printf("Eneter str1(5 symbols): ");
        scanf("%s", str1);
        str1[5] = 0;

        char str2[50];
        printf("Eneter str2(5 symbols): ");
        scanf("%s", str2);
        str2[5] = 0;

        char str3[50];
        printf("Eneter str3(5 symbols): ");
        scanf("%s", str3);
        str3[5] = 0;

        char str4[50];
        printf("Eneter str4(5 symbols): ");
        scanf("%s", str4);
        str4[5] = 0;

        char str5[50];
        printf("Eneter str5(5 symbols): ");
        scanf("%s", str5);
        str5[5] = 0;

        printf("\nParent pid: %d\n", getpid());
        f = open(name1, O_RDWR);
        if (f < 0) {
            printf("Can\'t open FIFO1 for writing\n");
            return -1;
        }

        write(f, str1, size);
        write(f, str2, size);
        write(f, str3, size);
        write(f, str4, size);
        write(f, str5, size);

        printf("%d\n", size);
        printf("%s\n", str1);
        
        printf("Parent (pid: %d) send 5 strings \n\n", getpid());

        close(f);
        kill(p0, SIGUSR1);
        waitpid(p0, NULL, 0);
        
        f = open(name2, O_RDWR);
        if (f < 0) {
            printf("Can\'t open FIFO2 for reading\n");
            return -1;
        }
        
        printf("Parent pid: %d \n", getpid());
        printf("Answer: \n");
        
        for(int i = 0; i <= 5; i++) {
            char str[size];
            read(f, str, size);
            printf("%s",str);
        }
        
        close(f);
        printf("\nParent (pid: %d) exit\n", getpid());
    } else { // child process
        (void)signal(SIGUSR1, handler);
        pause();
    }
}
