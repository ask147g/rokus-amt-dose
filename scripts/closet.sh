#!/bin/bash
cd ..
cmake ..
make -j8
echo "6" | ./rocus
cd output
echo ".q" | root closet_plot.cc