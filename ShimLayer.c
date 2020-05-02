#include <stdio.h>
#include "ShimLayer.h"

JNIEXPORT void JNICALL Java_ShimLayer_run(JNIEnv *env, jclass jc) {
    printf("Hello Native\n");
}