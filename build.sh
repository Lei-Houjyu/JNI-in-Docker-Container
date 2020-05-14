/build/linux-x86_64-normal-server-release/jdk/bin/javac ShimLayer.java
gcc -o init-sem init-sem.c -lpthread -lrt
gcc -o libShimLayer.so ShimLayer.c -lpthread -lrt -lJMagick -fPIC -shared -I/build/linux-x86_64-normal-server-release/jdk/include/ -I/build/linux-x86_64-normal-server-release/jdk/include/linux/
cp libShimLayer.so /usr/lib/
