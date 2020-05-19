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


#define PHILOSOPHS 5
#define HOME "/home/florian/vs"
#define ITERATIONS 1

pid_t waitpid(pid_t pid, int *status, int ops);
key_t sem_key;
int sem_id;
int sem_num;
struct sembuf semaphore;
int pid[5] = {0,0,0,0,0};


// Struct for all philsoph data
typedef struct {
    int id;
    int eating_time;
    int think_time;
    int fork[2];
} philosoph;

// Leave the sempahore
void V(int sem_num){
    semaphore.sem_num=sem_num;
    semaphore.sem_op=1;
    semaphore.sem_flg=~(IPC_NOWAIT|SEM_UNDO);

    if(semop(sem_id, &semaphore, 1)){

        perror("Error in semop V()\n");
        exit(1);
    }
}

// Enter the sempahore
void P(int sem_num){
    semaphore.sem_num = sem_num;
    semaphore.sem_op = -1;
    semaphore.sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

    if(semop(sem_id, &semaphore, 1)){

        perror("Error in semop P()\n");
        exit(1);
    }
}

// Initializes Semaphores and philosophes
void init_App(){

    // Create unique semaphore key
    if((sem_key = ftok(HOME, '1')) < 0){
        perror("Error in ftok\n");
        exit(1);
    }
    else{
        printf("Sem_key: %d\n", sem_key);
    }

    // Open semaphore group and creates one
    if((sem_id = semget(sem_key, 5, IPC_CREAT|0666)) < 0){
        perror("Error in semget\n");
        exit(1);
    }
    else{
        printf("Sem_ID: %d\n", sem_id);
    }

    for(int i = 0; i < 5; i++){
        if(semctl(sem_id, i, SETVAL, 1)<0){
            perror("Error in semctl\n");
            exit(1);
        }
    }
}

int main(){

    init_App();

    // Fork all philsophs, but only from father process
   /* for (int process = 0; process < PHILOSOPHS; process++){
        switch(fork()) {
            case -1:
                perror("Fork failed!\n");
                exit(1);
            case 0:
                // child process
                printf("Child process created!\n");
                break;
            default:
                break;
                // father
        }
    }*/
    int id = 1; // Init the id with a positive digit for the while loop
    int i = 0; // Index for the while loop
    while (id != 0 && i < PHILOSOPHS - 1){ // Somehow i need decrement the philosophs variable idk
        id = fork();
        if(id == -1) {
            printf("Error in fork!\n");
            exit(1);
        }
        else if (id == 0){
            printf("Child process, ID: %d created\n", i);

        }
        else{
            printf("Father process\n");
            i++;
        }
    }

    // Init the one philosoph of the current task
    philosoph p;

    // Init the random number generator with time seed
    srand(time(NULL) - i*2);
    // Init necessary variables for the philosoph "object"
    p.eating_time = (rand() % 10) + 1;
    p.think_time = (rand() % 10) + 1;
    printf("Philosoph %d has Eat time: %d\n", i, p.eating_time);
    printf("Philosoph %d has Think time: %d\n", i, p.think_time);

    // Assign 2 forks to the process
    p.fork[0] = i;
    printf("Philosoph %d: I selected fork %d!\n", i, p.fork[0]);
    p.fork[1] = ++i;
    if(i == PHILOSOPHS - 1){
        p.fork[1] = 0;
    }
    printf("Philosoph %d: I selected fork %d!\n", i, p.fork[1]);

    for(int j = 0; i < ITERATIONS; j++){
        sleep(p.think_time);
        P(p.fork[0]);
        printf("Took fork %d!\n", p.fork[0]);
        P(p.fork[1]);                                // Some kind of timer is needed, that fork[0] is dropped, if the process can't get fork[1]
        printf("Took fork %d!\n", p.fork[1]);
        printf("Philosoph %d: I start to eat!\n", i);
        sleep(p.eating_time);

        V(p.fork[0]);
        printf("Took fork %d!\n", p.fork[0]);
        V(p.fork[1]);
        printf("Took fork %d!\n", p.fork[1]);
        printf("Philosoph %d: I stopped eating!\n", i);
    }

    printf("Philosoph %d: I finished eating and thinking!\n", i);
    return 0;
}
