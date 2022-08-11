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

# install rokus-amt-dose project
WORKDIR /software/rokus-amt-dose
COPY data/* data/
COPY include/*.hh include/
COPY include/rapidxml/*.hpp include/rapidxml/
COPY  macro/*.mac macro/
COPY output/*.cc output/
COPY scripts/*.sh scripts/
COPY src/*.cc src/
COPY CMakeLists.txt .
COPY rocus.cc .

# start rokus-amt-dose
COPY run.sh run.sh
RUN chmod +x run.sh

CMD ./run.sh