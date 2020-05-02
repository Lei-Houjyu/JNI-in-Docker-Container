# JNI-in-Docker-Container

```
$ javac ShimLayer.java
$ gcc gcc ShimLayer.c -o libShimLayer.so -shared
  -I/usr/lib/jvm/java-8-openjdk-amd64/include/
  -I/usr/lib/jvm/java-8-openjdk-amd64/include/linux/
$ sudo cp libShimLayer.so /usr/lib/
$ java ShimLayer
```
