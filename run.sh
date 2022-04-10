#!/bin/bash
cmake ..
make
for ((i=0; i<10; i++))
do
./project3
done
