#!/bin/bash
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
cd scripts
rm -r distance.txt
done
