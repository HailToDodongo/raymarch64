cmake_minimum_required(VERSION 3.30)
project(n64raymarcher)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

include_directories(../libdragon/include)
include_directories(../tiny3d/src)

include_directories(src)

add_executable(n64raymarcher
        src/main.cpp
        src/text.cpp
        src/miMemory.cpp
        src/demos/demoRepeat.cpp
        src/demos/demoSync.cpp
        src/rdp/rdp.h
        src/rdp/rdp.cpp
        src/demos/demoVI.cpp
        src/demos/demoVIPong.cpp
        src/demos/raymarch.cpp)

set_property(TARGET n64raymarcher PROPERTY CXX_STANDARD 23)