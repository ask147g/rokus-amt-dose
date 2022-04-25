#!/bin/bash
cmake ..
make -j8
echo "0" | ./rocus
echo ".q" | root spectrum.cc
echo ".q" | root spectrum_fantom.cc