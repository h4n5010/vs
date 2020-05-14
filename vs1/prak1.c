#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <unistd.h>

#include "prak1.h"

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
