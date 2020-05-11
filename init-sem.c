#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <string.h>

#define LEN 64

int main(int argc, char* argv[]) {
	if (argc == 0) {
		printf("argument missing!\n");
		return 0;
	}

    char id_jvm[LEN], id_container[LEN], id_method[LEN], id_string[LEN], id_long[LEN];
    sprintf(id_jvm,       "/sem-jvm-%s",       argv[0]);
    sprintf(id_container, "/sem-container-%s", argv[0]);
    sprintf(id_method,    "/sem-method-%s",    argv[0]);
    sprintf(id_string,    "/sem-string-%s",    argv[0]);
    sprintf(id_long,      "/sem-long-%s",      argv[0]);

	sem_unlink(id_jvm);
	sem_unlink(id_container);

	sem_t * sem_id;
	int value;

	sem_id = sem_open(id_jvm, O_CREAT, 0666, 0);
	// sem_getvalue(sem_id, &value);
	// printf("/sem-jvm %d\n", value);
	sem_close(sem_id);


	sem_id = sem_open(id_container, O_CREAT, 0666, 0);
	// sem_getvalue(sem_id, &value);
	// printf("/sem-container %d\n", value);
	sem_close(sem_id);

	int fd_method = shm_open(id_method, O_CREAT | O_RDWR, 0666);
    int fd_string = shm_open(id_string, O_CREAT | O_RDWR, 0666);
    int fd_long   = shm_open(id_long, O_CREAT | O_RDWR, 0666);

    int r0 = ftruncate(fd_method, SIZE);
    int r1 = ftruncate(fd_string, SIZE);
    int r2 = ftruncate(fd_long, SIZE);

    close(fd_method);
    close(fd_string);
    close(fd_long);

	system("sudo chmod 666 /dev/shm/*");
}