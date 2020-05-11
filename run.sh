# echo "initializing semaphores..."
./init-sem $1
# echo "running..."
/home/lhy/shimlayer-jdk8u/build/linux-x86_64-normal-server-release/jdk/bin/java ShimLayer $1
# echo "finish!"

