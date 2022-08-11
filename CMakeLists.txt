cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(ROCUS-AMT)
find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

file(GLOB rapid_headers ${PROJECT_SOURCE_DIR}/include/rapidxml/*.hpp)

file(GLOB MACRO_FILES "macro/*.mac")
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR}/macro)

file(GLOB dataFile "data/*.xml") 
file(COPY ${dataFile} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)

file(GLOB dataFile "data/*.csv") 
file(COPY ${dataFile} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)

file(GLOB script "scripts/*.sh") 
file(COPY ${script} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/scripts)

file(GLOB root "output/*.cc") 
file(COPY ${root} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/output)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output/plane)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output/plane/distance)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output/closet)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output/spectrum)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output/distance)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output/figure)

add_executable(rocus rocus.cc ${sources} ${headers} ${rapid_headers})
target_link_libraries(rocus ${Geant4_LIBRARIES})

add_custom_target(ROCUS-AMT DEPENDS rocus)