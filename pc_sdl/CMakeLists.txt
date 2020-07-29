cmake_minimum_required(VERSION 3.2)
project(pcSdl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lmingw32")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

include_directories(${PROJECT_SOURCE_DIR}/include  src/include )
link_directories(${PROJECT_SOURCE_DIR}/lib)

set(SOURCE_FILES src/user/main.cpp src/bsp/display_sdl.cpp src/include/bsp/display_sdl.h src/include/user/config.h)
add_executable(pcSdl ${SOURCE_FILES})

target_link_libraries(pcSdl SDL2main SDL2)