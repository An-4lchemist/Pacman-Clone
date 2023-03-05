cmake_minimum_required(VERSION 3.21)
project(Pacman C)

set(CMAKE_C_STANDARD 17)

add_executable(Pacman main.c)

target_link_libraries(Pacman -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf)
