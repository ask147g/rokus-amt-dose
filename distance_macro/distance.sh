#!/bin/bash
dist=(0 70 140 210 280 350 420 490 560 630 700 770 840 910 980)
for ((i=0; i<15; i++))
do
echo ${dist[i]} > distance.txt
cd ..
cmake ..
make -j8
for ((j=0; j<10; j++))
do
./rocus
done
cd distance_macro
rm -r distance.txt
done
