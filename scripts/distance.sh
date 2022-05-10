#!/bin/bash
init=0
amount=15
step=70
mc_time=10
cd ..
cd output/distance
echo "0" > distanceResult.csv
rm -r distanceResult.csv
cd ..
cd ..
cd scripts
for ((i=0; i<"${amount}"; i++))
do
distance=$(("${init}"+"${i}"*"${step}"))
echo "${distance}" > distance.txt
cd ..
cmake ..
make -j8
for ((j=0; j<"${mc_time}"; j++))
do
echo "2" | ./rocus
done
cd scripts
rm -r distance.txt
done
cd ..
cd output

echo ".q" | root distance_plot.cc