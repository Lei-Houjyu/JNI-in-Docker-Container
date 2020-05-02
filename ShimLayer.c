#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>

#include "ShimLayer.h"

#define SIZE 1024
#define JNI_NUM 7

JNIEXPORT void JNICALL Java_ShimLayer_invoke(JNIEnv *env, jclass jc) {
  printf("Invoke Successfully!\n");
}

JNIEXPORT void JNICALL Java_ShimLayer_run(JNIEnv *env, jclass jc) {
  sem_t *sem_id_jvm = sem_open("/sem-jvm", O_CREAT, 0666, 0);
  if (sem_id_jvm < SEM_FAILED) {
    perror("Reader: sem_open failed!\n");
  }

  sem_t *sem_id_container = sem_open("/sem-container", O_CREAT, 0666, 0);
  if (sem_id_container < SEM_FAILED) {
    perror("Reader: sem_open failed!\n");
  }

  int fd_method = shm_open("/shmem-method", O_CREAT | O_RDWR, 0666);
  if (fd_method < 0) {
    perror("Reader: shm_open failed!\n");
    return;
  }

  char* method_name =
      (char *)mmap(0, SIZE, PROT_READ, MAP_SHARED, fd_method, 0);

  for (int i = 0; i < JNI_NUM; i++) {    
    if (sem_wait(sem_id_jvm) < 0) {
      perror("Reader: sem_wait failed!\n");
    }

    printf("Method name: %s\n", method_name);

    if (strcmp(method_name, "magick.Magick.init()V") == 0) {
    	Java_magick_Magick_init(env, jc);
    }

    if (sem_post(sem_id_container) < 0) {
      perror("Reader: sem_post failed!\n");
    }
  }

  if (sem_close(sem_id_jvm) != 0) {
    perror("Writer: sem_close failed!\n");
  }

  munmap(method_name, SIZE);

  close(fd_method);

  return;
}
