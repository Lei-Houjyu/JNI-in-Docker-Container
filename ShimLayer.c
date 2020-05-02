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
  struct JNIEnv_ *env_ = (struct JNIEnv_ *)env;

  env_->sem_id_jvm = sem_open("/sem-jvm", O_CREAT, 0666, 0);
  if (env_->sem_id_jvm < SEM_FAILED) {
    perror("Reader: sem_open failed!\n");
  }

  env_->sem_id_container = sem_open("/sem-container", O_CREAT, 0666, 0);
  if (env_->sem_id_container < SEM_FAILED) {
    perror("Reader: sem_open failed!\n");
  }

  env_->fd_method = shm_open("/shmem-method", O_CREAT | O_RDWR, 0666);
  env_->fd_parameter = shm_open("/shmem-parameter", O_CREAT | O_RDWR, 0666);

  if (env_->fd_method < 0 || env_->fd_parameter < 0) {
    perror("Reader: shm_open failed!\n");
    return;
  }

  env_->data_method =
      (char *)mmap(0, SIZE, PROT_READ, MAP_SHARED, env_->fd_method, 0);
  env_->data_parameter =
      (char *)mmap(0, SIZE, PROT_READ, MAP_SHARED, env_->fd_parameter, 0);

  for (int i = 0; i < JNI_NUM; i++) {
    if (sem_wait(env_->sem_id_jvm) < 0) {
      perror("Reader: sem_wait failed!\n");
    }

    printf("Method name: %s\n", env_->data_method);

    if (strcmp(env_->data_method, "magick.Magick.init()V") == 0) {
    	Java_magick_Magick_init(env, jc);
    } else if (strcmp(env_->data_method, "magick.ImageInfo.init()V") == 0) {

    } else if (strcmp(env_->data_method, "magick.ImageInfo.setFileName(Ljava/lang/String;)V") == 0) {
    	char *str;
    	sscanf(env_->data_parameter, "%s", str);
    	printf("Parameter: %s\n", str);
    } else if (strcmp(env_->data_method, "magick.MagickImage.readImage(Lmagick/ImageInfo;)V") == 0) {
    	
    } else if (strcmp(env_->data_method, "magick.MagickImage.scaleImage(II)Lmagick/MagickImage;") == 0) {
    	long cols, rows;
    	sscanf(env_->data_parameter, "%ld %ld", &cols, &rows);
    	printf("Parameter: %ld %ld\n", cols, rows);
    } else if (strcmp(env_->data_method, "magick.MagickImage.setFileName(Ljava/lang/String;)V") == 0) {
    	char *str;
    	sscanf(env_->data_parameter, "%s", str);
    	printf("Parameter: %s\n", str);
    } else if (strcmp(env_->data_method, "magick.MagickImage.writeImage(Lmagick/ImageInfo;)Z") == 0) {
    	
    }

    if (sem_post(env_->sem_id_container) < 0) {
      perror("Reader: sem_post failed!\n");
    }
  }

  if (sem_close(env_->sem_id_jvm) != 0) {
    perror("Writer: sem_close failed!\n");
  }

  munmap(env_->data_method, SIZE);
  munmap(env_->data_parameter, SIZE);

  close(env_->fd_method);
  close(env_->fd_parameter);
}
