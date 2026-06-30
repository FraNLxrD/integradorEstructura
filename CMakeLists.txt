cmake_minimum_required(VERSION 4.2)
project(integrador)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(integrador
        abb.hpp
        camino.hpp
        jugadores.hpp
        logicaJuego.hpp
        main.cpp
        pilaAvance.hpp)
