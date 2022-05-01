#!/bin/bash
cmake ..
make -j8
cd ..
echo "0" | ./rocus
cd output
echo ".q" | root spectrum_source.cc
echo ".q" | root spectrum_fantom.cc