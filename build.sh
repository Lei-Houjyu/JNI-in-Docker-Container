javac ShimLayer.java
javah ShimLayer
gcc -o libShimLayer.so ShimLayer.c -lpthread -lrt -lJMagick -fPIC -shared -I/usr/lib/jvm/java-8-openjdk-amd64/include/ -I/usr/lib/jvm/java-8-openjdk-amd64/include/linux/ -Wl,-Map=a.map
sudo cp libShimLayer.so /usr/lib/
