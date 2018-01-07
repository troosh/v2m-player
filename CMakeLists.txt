CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(tinyplayer)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O4 -ffast-math -msse -D_DEBUG")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O4 -ffast-math -msse -D_DEBUG")

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
FIND_PACKAGE(SDL2 REQUIRED)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR})

ADD_EXECUTABLE(tinyplayer
    tinyplayer.cpp
    v2mplayer.cpp
    v2mconv.cpp
    synth_core.cpp
    ronan.cpp
    sounddef.cpp
)

TARGET_LINK_LIBRARIES(tinyplayer
    ${SDL2_LIBRARY}
)
