cmake_minimum_required(VERSION 3.17)
project(SDL2Duell)

set(CMAKE_CXX_STANDARD 14)

include_directories(. ../../Downloads/SDL2-devel-2.0.14-VC/SDL2-2.0.14/include)
link_directories(../../Downloads/SDL2-devel-2.0.14-VC/SDL2-2.0.14/lib/x64)

add_executable(SDL2Duell
        Duell.cpp
        Duell.h
        Globals.h
        Player.cpp
        Player.h
        Sprite.cpp
        Sprite.h
        Utils.cpp
        Utils.h)
target_link_libraries(SDL2Duell SDL2 SDL2main SDL2_mixer SDL2_image)