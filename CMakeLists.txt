cmake_minimum_required(VERSION 3.30)
project(repasoPunteros)

set(CMAKE_CXX_STANDARD 17)

# Ejercicio OMP y repaso punteros y repaso_omp
#[[#habilitar OpemMp
find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CXX_CMAKE_FLAGS} -fopenmp")

find_package(glfw3)

# TENER MÁS DE UN EJECUTABLE EN UN PROYECTO:
add_executable(repasoPunteros correc_E1E1.cpp)
#add_executable(repasoPunteros repaso_omp.cpp)
#add_executable(repasoPunteros punteros.cpp)


if(OpenMP_CXX_FOUND)
    target_link_libraries(repasoPunteros PUBLIC OpenMP::OpenMP_CXX glfw)
endif()]]


## Ejercicio MPI

#habilitar MPI
find_package(MPI)
if (MPI_FOUND)
    message(STATUS "MPI encontrado")
    message(STATUS "MPI Include path: ${MPI_CXX_VERSION}")
    message(STATUS "MPI Include path: ${MPI_INCLUDE_PATH}")
    message(STATUS "MPI Libraries: ${MPI_LIBRARIES}")
    message(STATUS "MPI Include path: ${MPI_CXX_COMPILER}")
    message(STATUS "MPI Include path: ${MPI_lINK_FLAGS}")
else ()
    message(FATAL_ERROR "MPI Not FOUND")
endif ()
include_directories(${MPI_INCLUDE_PATH})
add_executable(repasoPunteros correc_E1E2_mpi.cpp)
target_link_libraries(repasoPunteros ${MPI_LIBRARIES})
