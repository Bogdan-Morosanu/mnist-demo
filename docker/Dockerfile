FROM base/archlinux

# update and get build dependencies
RUN pacman -Syu --noconfirm
RUN pacman -S base-devel boost git cmake gtest clang eigen --noconfirm

# go to repo directory
WORKDIR /repo