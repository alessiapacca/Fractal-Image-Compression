#! /bin/bash

img_name=$1

for img_size in $(seq 1 $2)
do
  cmake-build-release/team033 benchmark_img/"$img_name"_"$img_size".bmp compressed_img/"$img_name"_"$img_size".bmp results.csv > /dev/null
  echo "$img_name"_"$img_size.bmp,$[$img_size*256]"
done
