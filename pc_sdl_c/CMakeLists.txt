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
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -lmingw32") #-Werror

# C++ Project
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

include_directories(${PROJECT_SOURCE_DIR}/include src/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

file(GLOB_RECURSE SOURCES "src/bsp/*.*" "src/user/*.*" "src/lzui/*.*")

set(SOURCE_FILES src/user/main.c src/lzui/lz_obj.c src/include/lzui/lz_obj.h src/lzui/lz_mem.c src/include/lzui/lz_mem.h src/include/lzui/lz_types.h src/include/lzui/lz_config.h src/lzui/lz_draw.c src/include/lzui/lz_draw.h src/lzui/lz_color.c src/include/lzui/lz_color.h src/lzui/lz_obj_bar.c src/include/lzui/lz_obj_bar.h src/lzui/lz_obj_button.c src/include/lzui/lz_obj_button.h src/lzui/lz_obj_lable.c src/include/lzui/lz_obj_lable.h src/lzui/lz_info.c src/lzui/lz_obj_png.c src/include/lzui/lz_obj_png.h src/lzui/lz_obj_chart.c src/include/lzui/lz_obj_chart.h src/lzui/lz_tick.c src/include/lzui/lz_tick.h src/lzui/lz_easing.c src/include/lzui/lz_easing.h src/lzui/lz_obj_pos.c src/include/lzui/lz_obj_pos.h src/lzui/lz_obj_mouse.c src/include/lzui/lz_obj_mouse.h src/lzui/lz_obj_soft_3d.c src/include/lzui/lz_obj_soft_3d.h)
add_executable(LZUI ${SOURCES} ${SOURCE_FILES})

target_link_libraries(LZUI SDL2main SDL2)
