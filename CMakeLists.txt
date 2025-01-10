cmake_minimum_required(VERSION 3.30)
project(repasoPunteros)

set(CMAKE_CXX_STANDARD 17)

#habilitar OpemMp
find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CXX_CMAKE_FLAGS} -fopenmp")


# TENER MÁS DE UN EJECUTABLE EN UN PROYECTO:
add_executable(repasoPunteros main.cpp)
#add_executable(repasoPunteros punteros.cpp)


if(OpenMP_CXX_FOUND)
    target_link_libraries(repasoPunteros PUBLIC OpenMP::OpenMP_CXX)
endif()
