FROM archlinux

RUN yes | pacman -Syu gcc make cmake git root qt5-base

# install geant4
RUN mkdir software && mkdir software/geant4 && cd software/geant4 && \
git clone https://github.com/Geant4/geant4.git -b geant4-11.0-release

WORKDIR /software/geant4/geant4

RUN mkdir build && cd build && \
cmake -DGEANT4_INSTALL_DATA=ON \
-DCMAKE_INSTALL_PREFIX=/software/geant4/geant4-install \
-DGEANT4_USE_QT=ON \
-DGEANT4_USE_RAYTRACER_X11=ON .. && \
make -j8 && make install

