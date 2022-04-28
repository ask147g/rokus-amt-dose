#!/bin/bash
#dist=(0 70 140 210 280 350 420 490 560 630 700 770 840 910 980)
init=0
amount=105
step=10
mc_time=10
for ((i=0; i<"${amount}"; i++))
do
distance=$(("${init}"+"${i}"*"${step}"))
echo "${distance}" > distance.txt
cd ..
cmake ..
#make -j8
for ((j=0; j<"${mc_time}"; j++))
do
echo "2" | ./rocus
done
cd macro
rm -r distance.txt
done
