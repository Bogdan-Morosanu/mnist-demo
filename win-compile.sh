#!/bin/bash

# first copy our repo
cp -r /repo /repo-linux
cd /repo-linux

# change all line endings
echo "changing line endings..."
git ls-files | xargs dos2unix

# and now build
cd /repo-build
rm -rf CMake*
cmake /repo-linux/src
make
