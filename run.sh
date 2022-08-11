#!/bin/bash

source /software/geant4/geant4-install/bin/geant4.sh
source /software/geant4/geant4-install/share/Geant4-11.0.2/geant4make/geant4make.sh

mkdir build && cd build && cmake .. && make -j8

./rocus