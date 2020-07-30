#cmake_minimum_required(VERSION 3.2)
#project(pcSdl CXX C) # 这里可以不写CXX和C，但不能只写一个，否则只会编译指定语言
#
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99 -lmingw32")
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
#
#include_directories(${PROJECT_SOURCE_DIR}/include  src/include )
#link_directories(${PROJECT_SOURCE_DIR}/lib)
##set(SOURCE_FILES "src/bsp/*.*" "src/user/*.*" "src/lzui/*.*")
##add_executable(pcSdl ${SOURCE_FILES})
#
#target_link_libraries(pcSdl SDL2main SDL2)
#

cmake_minimum_required(VERSION 3.3)
project(LZUI)

# C Project
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Werror -lmingw32")

# C++ Project
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

include_directories(${PROJECT_SOURCE_DIR}/include src/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

file(GLOB_RECURSE SOURCES "src/bsp/*.*" "src/user/*.*" "src/lzui/*.*")

set(SOURCE_FILES src/user/main.c )
add_executable(LZUI ${SOURCES} ${SOURCE_FILES})

target_link_libraries(LZUI SDL2main SDL2)
