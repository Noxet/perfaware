#!/bin/bash

inp=$2
if [[ -z  "${inp}" ]]; then
    echo "No file found matching: $inp"
    exit 1
fi

# select debug or release
if [[ $1 = "d" ]]; then
    file=(out/*$inp*_deb)
elif [[ $1 = "r" ]]; then
    file=(out/*$inp*_rel)
fi

base="${file%.*}"
# shift advances the argument pointer, to that "$@" points to the unused ones
shift
shift

"./$base" "$@"
