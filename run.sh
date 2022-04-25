#!/bin/bash
cmake ..
make -j8
for ((i=0; i<10; i++))
do
./rocus
done
