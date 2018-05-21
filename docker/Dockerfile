FROM base/archlinux

# update and get build dependencies
RUN pacman -Syu --noconfirm && pacman -S base-devel boost git cmake eigen dos2unix --noconfirm

# setup the folder for our repo
RUN mkdir /repo

# copy source and build it
COPY . /repo
RUN mkdir /repo-build && cp /repo/win-compile.sh /repo-build
RUN cd /repo-build && cmake /repo/src && make

WORKDIR /repo-build
