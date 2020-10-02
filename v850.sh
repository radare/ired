#!/bin/sh -x

CC="docker run --rm --volume $(pwd):$(pwd) -w $(pwd) dkulacz/gcc-v850-elf-toolchain:latest"
TOOLCHAIN_PREFIX="v850-elf-"
FLAGS="-mv850e3v5 -mloop -mrh850-abi"
$CC ${TOOLCHAIN_PREFIX}gcc ${FLAGS} ired.c -o ired_v850
$CC ${TOOLCHAIN_PREFIX}size --format=berkeley -o ired_v850.elf
