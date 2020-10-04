#! /bin/bash

./build.sh --release

for img_name in $(seq 1 $1)
do
  ./measure-flops-img.sh $img_name $2 &
done