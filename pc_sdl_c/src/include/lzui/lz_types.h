//
// Created by a6735 on 2020/8/2.
//

#ifndef LZUI_LZ_TYPES_H
#define LZUI_LZ_TYPES_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

#if __STDC_VERSION__ >= 199901L  // If c99 or newer, use stdint.h to determine arch size
#include <stdint.h>
#endif


// If __UINTPTR_MAX__ or UINTPTR_MAX are available, use them to determine arch size
#if defined(__UINTPTR_MAX__) && __UINTPTR_MAX__ > 0xFFFFFFFF
#define LV_ARCH_64

#elif defined(UINTPTR_MAX) && UINTPTR_MAX > 0xFFFFFFFF
#define LV_ARCH_64

// Otherwise use compiler-dependent means to determine arch size
#elif defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__) || defined (__aarch64__)
#define LV_ARCH_64

#endif


/**********************
 *      TYPEDEFS
 **********************/

/**
 * LVGL error codes.
 */
enum {
	LV_RES_INV = 0, /*Typically indicates that the object is deleted (become invalid) in the action
                       function or an operation was failed*/
	LV_RES_OK,      /*The object is valid (no deleted) after the action*/
};
typedef uint8_t lv_res_t;

#if __STDC_VERSION__ >= 199901L
// If c99 or newer,  use the definition of uintptr_t directly from <stdint.h>
typedef uintptr_t lz_uintptr_t;

#else

// Otherwise, use the arch size determination
#ifdef LV_ARCH_64
typedef uint64_t lz_uintptr_t;
#else
typedef uint32_t lz_uintptr_t;
#endif

#endif


#define M_PI_1					3.14159265358979323846	/* pi */
#define M_PI_2					1.57079632679489661923	/* pi/2 */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //LZUI_LZ_TYPES_H
