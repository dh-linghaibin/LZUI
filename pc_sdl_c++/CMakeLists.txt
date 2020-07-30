cmake_minimum_required(VERSION 3.2)
project(pcSdl CXX C) # 这里可以不写CXX和C，但不能只写一个，否则只会编译指定语言

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lmingw32")
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lmingw32")  #设置c++的编译选项
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")  #设置c的编译选项
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")


include_directories(${PROJECT_SOURCE_DIR}/include  src/include )
link_directories(${PROJECT_SOURCE_DIR}/lib)

set(SOURCE_FILES src/user/main.cpp src/bsp/display_sdl.cpp src/include/bsp/display_sdl.h src/include/user/config.h src/lzui/lzui_core.cpp src/include/lzui/lzui_core.h src/lzui/lzui_objx.cpp src/include/lzui/lzui_objx.h src/lzui/lzui_draw.cpp src/include/lzui/lzui_draw.h src/lzui/ugui.c)
add_executable(pcSdl ${SOURCE_FILES})

target_link_libraries(pcSdl SDL2main SDL2)