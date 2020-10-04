#! /bin/bash

if [[ $* == *--release* ]]
then
  cmake --build "cmake-build-release" --target team033 -- -j 4
else
  cmake --build "cmake-build-debug" --target team033 -- -j 4
fi
