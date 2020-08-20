//
// Created by a6735 on 2020/8/2.
//

#ifndef LZUI_LZ_CONFIG_H
#define LZUI_LZ_CONFIG_H

#include "lzui/lz_mem.h"

#define LCD_WIDTH  320
#define LCD_LENGTH 240

#define CACHE_WIDTH  320
#define CACHE_LENGTH 240

#define CACHE_SIZE   CACHE_LENGTH * CACHE_WIDTH

#define BEST_CACHE_LENGTH_BLACK LCD_LENGTH/CACHE_LENGTH
#define BEST_CACHE_WIDTH_BLACK  LCD_WIDTH/CACHE_WIDTH

#define lz_malloc lz_mem_mallc
#define lz_free   lz_mem_free

#endif //LZUI_LZ_CONFIG_H
