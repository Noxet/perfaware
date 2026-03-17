#!/bin/bash

inp=$1
if [[ -z  "${inp}" ]]; then
    echo "No file found matching: $inp"
    exit 1
fi

file=(*$inp*)
base="${file%.*}"

mkdir -p out
pushd out

clang++ -g -Wall --std=c++23 ../$file -o ${base}_deb
clang++ -mavx2 -O3 -g -Wall --std=c++23 ../$file -o ${base}_rel

popd
