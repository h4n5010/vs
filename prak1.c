#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#include "prakt1.h"

pid_t waitpid(pid_t pid, int *status, int ops);
key_t sem_key;
int sem_id;
int sem_num;
struct sembuf semaphore;

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
	if((sem_id = semget(sem_key, 1, IPC_CREAT|0666)) < 0){
		perror("Error in semget");
		exit(1);
	}
	else{
		printf("Sem_ID: %d\n", sem_id);
	}
	
	/* Intialize semaphore */
	if(semctl(sem_id, sem_num, SETVAL, 1)<0){
		perror("Error in semctl");
		exit(1);
	}
}


void create_process_fork(){

	int pid[NUMBER_OF_PROCESSES];
	int status;
	
	init_sem();

	for (int process = 0; process < NUMBER_OF_PROCESSES; process++){
		switch(pid[process] = fork()) {
			case -1:
				perror("Fork failed!!");
				exit(1);
			case 0:
				/* child process */
				P(0);
				printf("child process %d (%d) entered critical state!\n", process, getpid());
				sleep(5);
				printf("child process %d (%d) leaved critical state!\n", process, getpid());
				V(0);
				sleep(5);
			default: 
				/* father */
				waitpid(pid[process], &status, 0);

		}
	}
	printf("father process has stopped!\n");

}

