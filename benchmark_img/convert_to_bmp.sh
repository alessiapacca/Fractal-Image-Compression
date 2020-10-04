#! /bin/bash

for i in $(seq 1 $2); do
   convert benchmark_img/original/"$1"_.png -resize $[$i*256]x$[$i*256]! -compress none BMP3:benchmark_img/"$1"_"$i".bmp
done
#rm $filename
#done
