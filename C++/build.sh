# !/bin/bash

# 跟踪显示所有命令
set -x

SOURCE_DIR=`pwd`

BUILD_DIR=${BUILD_DIR:-../build}

mkdir -p $BUILD_DIR \
    && cd $BUILD_DIR \
    && cmake $SOURCE_DIR \
    && make $*
