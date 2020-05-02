javac ShimLayer.java
javah ShimLayer
gcc -o libShimLayer.so ShimLayer.c -lpthread -lrt -lJMagick -fPIC -shared -I/home/lhy/shimlayer-jdk8u/build/linux-x86_64-normal-server-release/jdk/include/ -I/home/lhy/shimlayer-jdk8u/build/linux-x86_64-normal-server-release/jdk/include/linux/
sudo cp libShimLayer.so /usr/lib/
