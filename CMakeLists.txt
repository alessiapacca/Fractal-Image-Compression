cmake_minimum_required(VERSION 3.14)
project(team033 C)

set(CMAKE_C_STANDARD 11)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_C_COMPILER /usr/local/bin/gcc-9)
else()
    set(CMAKE_C_COMPILER "gcc")
endif()

set(CMAKE_C_COMPILER icc)

include_directories(src/utils)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -MP -MD")

set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3 -march=native -fp-model fast")

add_executable(team033
        src/utils/bmp.c
        src/utils/bmp.h
        src/utils/imageIO.c
        src/utils/imageIO.h
        src/compression/imagetransform.c
        src/compression/imagetransform.h
        src/main.c
        src/compression/imageCompressor.c
        src/compression/imageCompressor.h
        src/utils/encoder.c
        src/utils/encoder.h
        src/benchmark/tsc_x86.h
        src/benchmark/benchmark.h src/benchmark/benchmark.c)

target_link_libraries(team033 m) #LINK MATH.H

