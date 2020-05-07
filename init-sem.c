#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <stdbool.h>


int main() {
	sem_unlink("/sem-jvm");
	sem_unlink("/sem-container");

	sem_t * sem_id;
	int value;

	sem_id = sem_open("/sem-jvm", O_CREAT, 0666, 0);
	sem_getvalue(sem_id, &value);
	// printf("/sem-jvm %d\n", value);
	sem_close(sem_id);


	sem_id = sem_open("/sem-container", O_CREAT, 0666, 0);
	sem_getvalue(sem_id, &value);
	// printf("/sem-container %d\n", value);
	sem_close(sem_id);

	system("sudo chmod 666 /dev/shm/*");
}