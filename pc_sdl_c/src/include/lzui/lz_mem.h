//
// Created by a6735 on 2020/8/2.
//

#ifndef LZUI_LZ_MEM_H
#define LZUI_LZ_MEM_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stddef.h>
#include "lzui/lz_types.h"

#define debug_pt(...) //printf(__VA_ARGS__)

void *lz_mem_mallc(unsigned int size);
void lz_mem_free(void *pv);
void trace_heap(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //LZUI_LZ_MEM_H
