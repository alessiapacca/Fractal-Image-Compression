#! /bin/bash

for filename in ${pwd}*.ppm; do
    convert $filename ${filename%.*}.png
    rm $filename
done
