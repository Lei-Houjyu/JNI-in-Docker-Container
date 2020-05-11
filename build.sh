gcc -o libShimLayer.so ShimLayer.c -lpthread -lrt -lJMagick -fPIC -shared -I/build/linux-x86_64-normal-server-release/jdk/include/ -I/build/linux-x86_64-normal-server-release/jdk/include/linux/
sudo cp libShimLayer.so /usr/lib/
