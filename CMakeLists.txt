cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(Simulation)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

file(GLOB MACRO_FILES "*.mac")

file(GLOB dataFile "data/*.txt") 
file(COPY ${dataFile} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)

file(GLOB script "run.sh") 
file(COPY ${script} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(project3 project3.cc ${sources} ${headers})
target_link_libraries(project3 ${Geant4_LIBRARIES})

add_custom_target(Simulation DEPENDS project3)
