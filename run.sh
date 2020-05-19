#!/bin/bash
# echo "initializing semaphores..."
# ./init-sem $1
# echo "running..."
/build/linux-x86_64-normal-server-release/jdk/bin/java ShimLayer $1 > output.txt
# echo "finish!"

