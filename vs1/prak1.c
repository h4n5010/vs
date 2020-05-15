<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>


#include "prakt1.h"

pid_t waitpid(pid_t pid, int *status, int ops);


void create_process_fork(){

	int pid[NUMBER_OF_PROCESSES];
	int status;

	for (int process = 0; process < NUMBER_OF_PROCESSES; process++){
		switch(pid[process] = fork()) {
			case -1:
				perror("Fork failed!!");
				exit(1);
			case 0:
				/* child process */
				printf("child process %d (%d) entered critical state!\n", process, getpid());
				sleep(1);
				printf("child process %d (%d) leaved critical state!\n", process, getpid());
				sleep(1);
			default: 
				/* father */
				waitpid(pid[process], &status, 0);

		}
	}
	printf("father process has stopped!\n");

}
=======
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>


#include "prakt1.h"

pid_t waitpid(pid_t pid, int *status, int ops);
key_t sem_key;
int sem_id;
int sem_num;

void init_sem(){


	if((sem_key = ftok (HOME, '1')) < 0){
		perror("Error in ftok");
		exit(1);
	}

	if((sem_id = semget(sem_key, 1, IPC_CREAT|0666)) < 0){
		perror("Error in semget");
		exit(1);
	}

	if(semctl(sem_id, sem_num, SETVAL, 1)<0){
		perror("Error in semctl");
		exit(1);
	}
}


void create_process_fork(){

	int pid[NUMBER_OF_PROCESSES];
	int status;
	
	



	for (int process = 0; process < NUMBER_OF_PROCESSES; process++){
		switch(pid[process] = fork()) {
			case -1:
				perror("Fork failed!!");
				exit(1);
			case 0:
				/* child process */
				printf("child process %d (%d) entered critical state!\n", process, getpid());
				sleep(1);
				printf("child process %d (%d) leaved critical state!\n", process, getpid());
				sleep(1);
			default: 
				/* father */
				waitpid(pid[process], &status, 0);

		}
	}
	printf("father process has stopped!\n");

}
>>>>>>> 55592e7fba55266e52c703bdcfc27f06a40ae27b
