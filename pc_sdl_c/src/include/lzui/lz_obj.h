//
// Created by a6735 on 2020/8/2.
//

#ifndef LZUI_LZ_OBJ_H
#define LZUI_LZ_OBJ_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

typedef enum {
	lui_true = 1,
	lui_false = 0,
} lz_bool;

typedef struct lz_point_t {
	int x;
	int y;
} lz_point_t;

typedef struct lz_size_t {
	uint16_t width;
	uint16_t length;
} lz_size_t;

typedef struct lz_layout_t {
	lz_point_t point;
	lz_size_t  size;
} lz_layout_t;

typedef struct lz_touch_val_t {
	struct lz_obj_t *obj;
	int abs_x;
	int abs_y;
	int rel_x;
	int rel_y;
	uint8_t falg;
	void (*event) (struct lz_touch_val_t *val);
} lz_touch_val_t;

typedef struct lz_obj_t {
	lz_layout_t      layout;
//	lz_point_t       rel_point;
//	lz_layout_t      rel_layout;//test
	struct lz_obj_t *father;
	struct lz_obj_t *brother;
	struct lz_obj_t *child;
	void * val;
	void (*design) (struct lz_obj_t * obj, lz_point_t *point);
	void (*event) (lz_touch_val_t *val);
	void (*delete_val)( struct lz_obj_t *obj );
	uint8_t event_flag;
} lz_obj_t;


lz_obj_t *lz_get_root(void);
lz_obj_t *lz_create_obj( int x, int y, int width, int length, void * val,
                           void (*design) (struct lz_obj_t * obj, lz_point_t *point) );
void lz_obj_set_delete_val_cb( lz_obj_t * obj, void (*cb) ( lz_obj_t * obj ) );
void lz_obj_set_x(lz_obj_t * obj, int x);
void lz_obj_set_y(lz_obj_t * obj, int y);
void lz_obj_set_width(lz_obj_t * obj, int width);
void lz_obj_set_length(lz_obj_t * obj, int length);
void lz_obj_set_event(lz_obj_t * obj, void (*event) (lz_touch_val_t *val));
void lz_obj_add_brother(lz_obj_t * obj, lz_obj_t * brother);
void lz_obj_add_child(lz_obj_t * obj, lz_obj_t * child);
void lz_obj_distroy(lz_obj_t ** obj);
void lz_obj_traverse(lz_obj_t * obj);
void lz_obj_coupoint(lz_obj_t * obj, lz_point_t * point);
void lz_obj_even(int x, int y, uint8_t flag);


#endif //LZUI_LZ_OBJ_H
