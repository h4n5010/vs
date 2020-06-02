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

// Globales Defines
#define HOME "/home/pi/vs"
#define NUMBER_OF_WRITERS 2
#define NUMBER_OF_READERS 5
#define READER 0
#define WRITER 1
#define MUTEX 2
#define ITERATIONS 3

pid_t waitpid(pid_t pid, int *status, int ops);
key_t sem_key;
int sem_id;
int sem_num;
//int reader;
struct sembuf semaphore;


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
void initApp(){

    // Create unique semaphore key
    if((sem_key = ftok(HOME, '1')) < 0){
        perror("Error in ftok\n");
        exit(1);
    }
    else{
        printf("Sem_key: %d\n", sem_key);
    }

    // Open semaphore group and creates one
    if((sem_id = semget(sem_key, 3, IPC_CREAT|0666)) < 0){
        perror("Error in semget\n");
        exit(1);
    }
    else{
        printf("Sem_ID: %d\n", sem_id);
    }

    /*
    // Setup all semaphores in the semaphore group
    for(int i = 0; i < 2; i++){
        if(semctl(sem_id, i, SETVAL, 1)<0){
            perror("Error in semctl\n");
            exit(1);
        }
    }*/

    // Create Reader/Counter semaphore
    if(semctl(sem_id, 0, SETVAL, 1) < 0) {
	perror("Error in semctl (Reader/Counter)\n");
	exit(1);
    }
    // Create Writer Semaphore
    if(semctl(sem_id, 1, SETVAL, 1) <0){
	perror("Error in semctl (Writer)\n");
	exit(1);
    }
    // Create Mutex for atomic access to semaphores
    if(semctl(sem_id, 2, SETVAL, 1) < 0) {
	perror("Error in semctl (Mutex)ß\n");
	exit(1);
    }

}

int main(){
	initApp();
	int id = 0;
	int reader = 0;

	// Fork 5 Reader and 2 writer processes
	for(int i = 0; i < (NUMBER_OF_WRITERS + NUMBER_OF_READERS); i++) {
		if((id = fork()) == -1){ // Error path
			perror("Error in fork()\n");
			exit(1);
		}
		else if(id == 0){ // Child process
			id = i + 1;			
			break;
		}
		else{ // Father Process
			continue;
		}
	}
	printf("debug 1 __ %d\n", id);	
	if(id <= 5) { // Reader Process
		for(int j = 0; j < ITERATIONS; j++){
			printf("debug 2 __ %d\n", id);
			P(MUTEX);
			reader++;
			printf("debug 3 __ %d \n", id);
			if(reader == 1){
				P(WRITER);
				printf("debug 4 __ %d \n", id);
			}	
			sleep(1);
			printf("check %d\n", id);	
			V(MUTEX);
			printf("check %d\n", id);	

			printf("Read %d ----- %d\n", semctl(sem_id, READER, GETVAL, 0), id);
			sleep(1);

			printf("debug 5 __ %d\n", id);

			P(MUTEX);
			reader--;
			printf("debug 6 __ %d\n", id); 
			if(reader == 0){
				V(WRITER);
			}
			V(MUTEX);
		}
	}
	if(id > 5 && id < 8) {
		srand(id);
		for(int j = 0; j < ITERATIONS; j++){
			printf("debug 7 __ %d\n",id);
			P(WRITER);
			printf("debug 8 __ %d\n", id);
			int randoom = rand() % 10;
			semctl(sem_id, READER, SETVAL, randoom);
			printf("Write: %d --------- %d\n", randoom, id);
			sleep(1);
			V(WRITER);
		}
	}

	return 0;
}
