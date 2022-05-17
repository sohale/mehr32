#!/usr/bin/env bash
set -eu
#set -eux # debug

REPOROOT=$(git rev-parse --show-toplevel)

# export PS1="docker>"

# docker run -it --rm --name conangcc11 -v $REPOROOT:/mehr32 conanio/gcc11-ubuntu16.04 /bin/bash

docker run -it --rm --name conangcc11 -v $REPOROOT:/mehr32 conanio/gcc11-ubuntu16.04 /bin/bash -c \
   -c " \
      cd /mehr32/reconstruction ;
      gcc mehr32-as-in-thesis-report.cpp ;
      bash
      "

# https://docs.conan.io/en/latest/howtos/run_conan_in_docker.html
