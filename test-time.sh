#! /bin/bash

./build.sh --release

echo "Creating test images..."
for img_name in $(seq 1 $1)
do
  benchmark_img/convert_to_bmp.sh $img_name $2
done
echo "Test images created!"

exec 3>&1 4>&2


if [ "$(whoami)" = "boschetto" ]; then
    echo 1 | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo
fi

export TIMEFORMAT=%R
for img_name in $(seq 1 $1)
do
  for img_size in $(seq 1 $2)
  do
    if [ "$(whoami)" = "boschetto" ]; then
      time=$( { time taskset 0x1 cmake-build-release/team033 benchmark_img/"$img_name"_"$img_size".bmp compressed_img/"$img_name"_"$img_size".bmp results.csv 2>&4 1>/dev/null; } 2>&1 )
    else
      time=$( { time cmake-build-release/team033 benchmark_img/"$img_name"_"$img_size".bmp compressed_img/"$img_name"_"$img_size".bmp results.csv 2>&4 1>/dev/null; } 2>&1 )
    fi
    echo "$img_name"_"$img_size.bmp,$[$img_size*256],$time" | tee -a time.csv
  done
done

if [ "$(whoami)" = "boschetto" ]; then
    echo 0 | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo
fi

sha512sum -c checksums/reference.sha512sum --ignore-missing

#rm benchmark_img/*.bmp
