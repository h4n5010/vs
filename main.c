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

// Sem_nums for the reader/writer semaphores
#define READER 0
#define WRITER 1
#define MUTEX 2

// Iterations
#define ITERATIONS 3

key_t sem_key;
int sem_id;
int sem_num;
struct sembuf semaphore;
int temp = 0; // Temp variable for incrementing the reader semaphore

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

    // Create Reader/Counter semaphore
    if(semctl(sem_id, 0, SETVAL, 0) < 0) {
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
	// Init 3 semaphores
	initApp();
	
	// Process IDs
	int id = 0;

	// Fork 5 Reader and 2 writer processes
	for(int i = 0; i < (NUMBER_OF_WRITERS + NUMBER_OF_READERS) - 1; i++) {
		if((id = fork()) == -1){ // Error path
			perror("Error in fork()\n");
			exit(1);
		}
		else if(id == 0){ // Child process
			id = i + 1;			
			break;
		}
		else{ // Father Process
			id = 7;
			continue;
		}
	}
	
	if(id <= 5) { // Reader Process

		for(int j = 0; j < ITERATIONS; j++){
			// Belege die Mutex, um den Readerzaehler zu erhoehen
			P(MUTEX);
			temp = semctl(sem_id, READER, GETVAL, 0);
			temp++;
			semctl(sem_id, READER, SETVAL, temp);
			
			// Belege die Writer Semaphore, falls es sich um den ersten Reader handelt
			if(semctl(sem_id, READER, GETVAL, 0) == 1){
				P(WRITER);
			}

			// Mutex kann verlassen werden, da Readerzaehler fertig beschrieben wurde	
			V(MUTEX);
			
			// Beginne des Lesevorgang, kann parallel zu anderen Reader Prozessen erfolgen
			printf("%d: Ich lese...\n", id);
			sleep(1);
			printf("%d: Ich bin fertig mit lesen!\n", id);

			// Mutex betreten, da der Readerzaehler neu beschrieben wird
			P(MUTEX);

			temp = semctl(sem_id, READER, GETVAL, 0);
			temp--;
			semctl(sem_id, READER, SETVAL, temp);
			
			// Falls es sich um den letzten Reader handelt, kann die Writer Semaphore freigegeben werden
			if(semctl(sem_id, READER, GETVAL, 0) == 0){
				V(WRITER);
			}

			// Mutex kann verlassen werden, da Readerzaehler fertig beschrieben wurde
			V(MUTEX);
		}
		// 3 Zyklen abgeschlossen, Reader Prozess fertig
		printf("%d: Terminiert!\n", id);
	}


	if(id > 5) { // Writer process

		for(int j = 0; j < ITERATIONS; j++){
			// WRITER Semaphore belegen, damit keine lesenden Prozesse aktiv sind und schreiben
			P(WRITER);
			printf("%d: Ich schreibe...\n", id);
			sleep(1);
			printf("%d: Ich bin fertig mit schreiben\n", id);

			// Schreiben abgeschlossen und WRITER Semaphore wieder freigeben
			V(WRITER);
		}
		// 3 Zyklen abgeschlossen, Writer Prozess fertig
		printf("%d: Terminiert!\n", id);
	}

	return 0;
}
