#! /bin/bash

./build.sh --release

echo "Creating test images..."
for img_name in $(seq 1 $1)
do
  benchmark_img/convert_to_bmp.sh $img_name $2
done
echo "Test images created!"

for img_name in $(seq 1 $1)
do
  for img_size in $(seq 1 $2)
  do
      cmake-build-release/team033 benchmark_img/"$img_name"_"$img_size".bmp compressed_img/"$img_name"_"$img_size".bmp results.csv
  done
done

sha512sum compressed_img/*.bmp > checksums/reference.sha512sum