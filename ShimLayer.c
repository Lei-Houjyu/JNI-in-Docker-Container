#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <stdbool.h>

#include "ShimLayer.h"

#define SIZE 1024
#define JNI_NUM 7


JNIEXPORT void JNICALL Java_ShimLayer_invoke(JNIEnv *env, jclass jc) {
  printf("Invoke Successfully!\n");
}

JNIEXPORT void JNICALL Java_ShimLayer_run(JNIEnv *env, jclass jc) {
  int order;
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
  env_->fd_string = shm_open("/shmem-string", O_CREAT | O_RDWR, 0666);
  env_->fd_long = shm_open("/shmem-long", O_CREAT | O_RDWR, 0666);

  if (env_->fd_method < 0 || env_->fd_string < 0 || env_->fd_long < 0) {
    perror("Reader: shm_open failed!\n");
    return;
  }

  env_->data_method =
      (char *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, env_->fd_method, 0);
  env_->data_string =
      (char *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, env_->fd_string, 0);
  env_->data_long =
      (long *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, env_->fd_long, 0);

  env_->need_ipc = true;
  for (int i = 0; i < JNI_NUM; i++) {
    if (sem_wait(env_->sem_id_jvm) < 0) {
      perror("Reader: sem_wait failed!\n");
    }

    // Actually, we should resolve the method name to find
    // the corresponding native method.
    printf("Method name: %s %ld %ld %ld\n", env_->data_method, env_->data_long[0], env_->data_long[1], env_->data_long[2]);
    scanf("%d", &order);
    long ret = 0;
    if (strcmp(env_->data_method, "magick.Magick.init()V") == 0) {
    	printf("1\n");
    	Java_magick_Magick_init(env, env_->data_long[0]);
    	printf("2\n");
    } else if (strcmp(env_->data_method, "magick.ImageInfo.init()V") == 0) {
    	Java_magick_ImageInfo_init(env, env_->data_long[0]);
    } else if (strcmp(env_->data_method, "magick.ImageInfo.setFileName(Ljava/lang/String;)V") == 0) {
    	Java_magick_ImageInfo_setFileName(env, env_->data_long[0], env_->data_long[1]);
    } else if (strcmp(env_->data_method, "magick.MagickImage.readImage(Lmagick/ImageInfo;)V") == 0) {
    	Java_magick_MagickImage_readImage(env, env_->data_long[0], env_->data_long[1]);
    } else if (strcmp(env_->data_method, "magick.MagickImage.scaleImage(II)Lmagick/MagickImage;") == 0) {
    	ret = (long)Java_magick_MagickImage_scaleImage(env, env_->data_long[0], env_->data_long[1], env_->data_long[2]);
    } else if (strcmp(env_->data_method, "magick.MagickImage.setFileName(Ljava/lang/String;)V") == 0) {
    	Java_magick_MagickImage_setFileName(env, env_->data_long[0], env_->data_long[1]);
    } else if (strcmp(env_->data_method, "magick.MagickImage.writeImage(Lmagick/ImageInfo;)Z") == 0) {
    	ret = (long)Java_magick_MagickImage_writeImage(env, env_->data_long[0], env_->data_long[1]);
    }

    scanf("%d", &order);
    strcpy(env_->data_method, "X");
    env_->data_long[0] = ret;
    printf("Finished %s %ld\n", env_->data_method, env_->data_long[0]);
    if (sem_post(env_->sem_id_container) < 0) {
      perror("Reader: sem_post failed!\n");
    }
    printf("post sem_id_container\n");
  }
  env_->need_ipc = false;

  if (sem_close(env_->sem_id_jvm) != 0) {
    perror("Writer: sem_close failed!\n");
  }

  munmap(env_->data_method, SIZE);
  munmap(env_->data_string, SIZE);
  munmap(env_->data_long, SIZE);

  close(env_->fd_method);
  close(env_->fd_string);
  close(env_->fd_long);
}
