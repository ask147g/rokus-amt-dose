#!/bin/bash
cd ..
cmake ..
make -j8
echo "3" | ./rocus
cd output
echo ".q" | root plane_plot.cc