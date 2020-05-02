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
  int fd_parameter = shm_open("/shmem-parameter", O_CREAT | O_RDWR, 0666);

  if (fd_method < 0 || fd_parameter < 0) {
    perror("Reader: shm_open failed!\n");
    return;
  }

  char *data_method =
      (char *)mmap(0, SIZE, PROT_READ, MAP_SHARED, fd_method, 0);
  char *data_parameter =
      (char *)mmap(0, SIZE, PROT_READ, MAP_SHARED, fd_parameter, 0);

  for (int i = 0; i < JNI_NUM; i++) {
  	printf("access counter: %d\n", ((struct JNIEnv_ *)env)->count++);
    if (sem_wait(sem_id_jvm) < 0) {
      perror("Reader: sem_wait failed!\n");
    }

    printf("Method name: %s\n", data_method);

    if (strcmp(data_method, "magick.Magick.init()V") == 0) {
    	Java_magick_Magick_init(env, jc);
    } else if (strcmp(data_method, "magick.ImageInfo.init()V") == 0) {

    } else if (strcmp(data_method, "magick.ImageInfo.setFileName(Ljava/lang/String;)V") == 0) {
    	char *str;
    	sscanf(data_parameter, "%s", str);
    	printf("Parameter: %s\n", str);
    } else if (strcmp(data_method, "magick.MagickImage.readImage(Lmagick/ImageInfo;)V") == 0) {
    	
    } else if (strcmp(data_method, "magick.MagickImage.scaleImage(II)Lmagick/MagickImage;") == 0) {
    	long cols, rows;
    	sscanf(data_parameter, "%ld %ld", &cols, &rows);
    	printf("Parameter: %ld %ld\n", cols, rows);
    } else if (strcmp(data_method, "magick.MagickImage.setFileName(Ljava/lang/String;)V") == 0) {
    	char *str;
    	sscanf(data_parameter, "%s", str);
    	printf("Parameter: %s\n", str);
    } else if (strcmp(data_method, "magick.MagickImage.writeImage(Lmagick/ImageInfo;)Z") == 0) {
    	
    }

    if (sem_post(sem_id_container) < 0) {
      perror("Reader: sem_post failed!\n");
    }
  }

  if (sem_close(sem_id_jvm) != 0) {
    perror("Writer: sem_close failed!\n");
  }

  munmap(data_method, SIZE);
  munmap(data_parameter, SIZE);

  close(fd_method);
  close(fd_parameter);
}
