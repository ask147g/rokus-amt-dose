cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(Simulation)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

file(GLOB MACRO_FILES "*.mac")

file(GLOB dataFile "data/*.xml") 
file(COPY ${dataFile} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)

file(GLOB root "spectrum.cc") 
file(COPY ${root} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB root "spectrum_fantom.cc") 
file(COPY ${root} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB script "*.sh") 
file(COPY ${script} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB script "distance_macro/distance.sh") 
file(COPY ${script} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/distance_macro)

file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(rocus rocus.cc ${sources} ${headers})
target_link_libraries(rocus ${Geant4_LIBRARIES})

add_custom_target(Simulation DEPENDS rocus)
