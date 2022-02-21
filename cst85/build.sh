#!/bin/bash

set -e

OUT_DIR="$1"
TOOLCHAIN_DIR="$2"

echo "^^^^^^^^^^^^^^^^^^^%%%%%%%%%%%%%%%%%%%################################"
function main(){
    ROOT_DIR=$(cd $(dirname "$0");pwd)
    if [ -z "${TOOLCHAIN_DIR}" ]; then
        make clean &&  make -j16 OUT_DIR_PATH=${OUT_DIR}
    else
        make clean &&  make -j16 OUT_DIR_PATH=${OUT_DIR} TOOLCHAIN_DIR_PATH=${TOOLCHAIN_DIR}
    fi
}

main "$@"
