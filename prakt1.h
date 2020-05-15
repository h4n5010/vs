#define NUMBER_OF_PROCESSES 3 

void init_sem();
void create_process_fork();
void V(int sem_num);
void P(int sem_num);
static const char HOME[] = "/home/florian/vs/vs1/";

