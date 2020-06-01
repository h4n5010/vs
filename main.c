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

pid_t waitpid(pid_t pid, int *status, int ops);
key_t sem_key;
int sem_id;
int sem_num;
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
	int id;

	// Fork 5 Reader and 2 writer processes
	for(int i = 0; i < (NUMBER_OF_WRITERS + NUMBER_OF_READERS); i++) {
		if((id = fork()) == -1){ // Error path
			perror("Error in fork()\n");
			exit(1);
		}
		else if(id == 0){ // Child process
			break;
		}
		else{ // Father Process
			continue;
		}
	}
	printf("Hello\n");	
	/*while(true) {

	}*/


	return 0;
}
