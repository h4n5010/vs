#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

#include "main.h"

#define PHILOSOPHEN 5

pid_t waitpid(pid_t pid, int *status, int ops);
key_t sem_key;
int sem_id;
int sem_num;
struct sembuf semaphore;

typedef struct {
    int id;
    int essen_zeit;
    int denk_zeit;
    int gabel[2];
} philosoph[PHILOSOPHEN];

/* Leave the sempahore */
void V(int sem_num){
    semaphore.sem_num=sem_num;
    semaphore.sem_op=1;
    semaphore.sem_flg=~(IPC_NOWAIT|SEM_UNDO);

    if(semop(sem_id, &semaphore, 1)){

        perror("Error in semop V()");
        exit(1);
    }
}

/* Enter the sempahore */
void P(int sem_num){
    semaphore.sem_num = sem_num;
    semaphore.sem_op = -1;
    semaphore.sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

    if(semop(sem_id, &semaphore, 1)){

        perror("Error in semop P()");
        exit(1);
    }
}


/* Initializes Semaphores */
void init_sem(){

    /* Create unique semaphore key */
    if((sem_key = ftok(HOME, '1')) < 0){
        perror("Error in ftok");
        exit(1);
    }
    else{
        printf("Sem_key: %d\n", sem_key);
    }

    /* Open semaphore group and creates one */
    if((sem_id = semget(sem_key, 5, IPC_CREAT|0666)) < 0){
        perror("Error in semget");
        exit(1);
    }
    else{
        printf("Sem_ID: %d\n", sem_id);
    }

    /* Intialize semaphore */
    for(int i = 0; i < 5; i++){
        if(semctl(sem_id, i, SETVAL, 1)<0){
            perror("Error in semctl");
            exit(1);
        }
    }
}


int main(){

    //init_sem();
    srand(time(NULL));
    int nthreads, tid;

    philosoph leute;

    for(int i = 0; i < PHILOSOPHEN; i++){
        leute[i].essen_zeit = rand() % 11; // random time between 0 and 10
        leute[i].id = i;
        do {
            int time = rand() % 11;
            leute[i].denk_zeit = time;
        } while (time == leute[i].essen_zeit);
        leute[i].gabel[0] = i;
        if (i == 0){
            leute[i].gabel[1] = 4;
        } else {
            leute[i].gabel[1] = i + 1;
        }
    }


}


