#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<semaphore.h>
#include <pthread.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    sem_t *sem1, *sem2;
    int f;

    if((sem1 = sem_open("semaphore1", O_RDWR|O_CREAT, 0666,1)) == SEM_FAILED) {
        printf("ERROR sem_open");
        return 1;
    }

    if((sem2 = sem_open("semaphore2", O_RDWR|O_CREAT, 0666,0)) == SEM_FAILED) {
        printf("ERROR sem_open");
        return 1;
    }
    // sem_unlink("semaphore1");

    char symbol;
    while(symbol != '0') {
        printf("Send: ");
        scanf("%s", &symbol);
        sem_wait(sem1);

        f = open("file", O_WRONLY | O_CREAT, 0666);
        
        if(f < 0) {
            printf("Can\'t open file\n");
            exit(-1);
        }
        pthread_mutex_lock(&mutex);
        write(f, &symbol, sizeof(symbol));
        close(f);
        pthread_mutex_unlock(&mutex);

        sem_post(sem1);
        sem_post(sem2);
    }

    sem_close(sem1);
    sem_close(sem2);
    return 0;
}

