FROM base/archlinux

# update and get build dependencies
RUN pacman -Syu --noconfirm
RUN pacman -S base-devel boost git cmake gtest clang eigen --noconfirm

# setup the folder for our repo
RUN mkdir /repo && mkdir /repo/build

# copy source and build it
COPY src /repo/src
RUN cd /repo/build && cmake ../src && make

WORKDIR /repo/build
