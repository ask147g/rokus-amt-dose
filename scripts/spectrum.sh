#!/bin/bash
cd ..
cmake ..
make -j8
echo "0" | ./rocus
cd output
echo ".q" | root spectrum_source.cc
echo ".q" | root spectrum_fantom.cc