#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<semaphore.h>
#include <pthread.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    sem_t *sem1,*sem2;
    int f;

    //sem_unlink("semaphore2");

    if((sem1 = sem_open("semaphore1", O_RDWR)) == SEM_FAILED) {
        printf("ERROR sem_open");
        return 1;
    }

    if((sem2=sem_open("semaphore2", O_RDWR|O_CREAT, 0666,1)) == SEM_FAILED) {
        printf("ERROR sem_open");
        return 1;
    }
    
    char symbol[2];
    while(symbol[0] != '0'){
    
        sem_wait(sem2);
        sem_wait(sem1);

        f = open("file",O_RDONLY);
        
        if(f < 0) {
            printf("Can\'t open file\n");
            exit(-1);
        }
        
        pthread_mutex_lock(&mutex);
        read(f, symbol, sizeof(symbol));
        printf("Get: %s \n", symbol);
        close(f);
        pthread_mutex_unlock(&mutex);

        sem_post(sem1);
        
        if(symbol[0] == '0') {
            break;
        }
        
        int times = rand()%10 + 1;
        for( int i = 0; i < times; i++) {
            printf("%s",symbol);
        }
        printf("\n");
    }
    
    sem_close(sem1);
    sem_close(sem2);
    return 0;
}
