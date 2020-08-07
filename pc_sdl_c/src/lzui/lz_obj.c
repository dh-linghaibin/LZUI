//
// Created by a6735 on 2020/8/2.
//
#include "lzui/lz_obj.h"
#include "lzui/lz_config.h"

static lz_obj_t lui_root = {
		.layout.point.x = 0,
		.layout.point.y = 0,
		.layout.size.width = LCD_LENGTH,
		.layout.size.length = LCD_WIDTH,
		.father  = NULL,
		.brother = NULL,
		.child   = NULL,
		.design  = NULL,
};

/* touch  */
static lz_touch_val_t touch_val = {
		.obj = NULL,
		.abs_x = 0,
		.abs_y = 0,
		.rel_x = 0,
		.rel_y = 0,
		.falg  = 0,
		.event = NULL,
};

lz_layout_t f_layout;

static lz_obj_t * last_stack = NULL;
static lz_point_t stack_point;
static int tree_layer = 0;
static int last_tree_layer = 0;
static lz_layout_t mlayout[20];

lz_obj_t *lz_get_root(void) {
	return &lui_root;
}

lz_obj_t *lz_create_obj( int x, int y, int width, int length, void * val,
                           void (*design) (lz_obj_t * obj,lz_point_t *point) ) {
	lz_obj_t * obj;
	obj = (lz_obj_t *)lz_malloc(sizeof(lz_obj_t));
	if( obj == NULL ) {
		return NULL;
	}
	obj->layout.point.x = x;
	obj->layout.point.y = y;
	obj->layout.size.width = width;
	obj->layout.size.length = length;
	obj->brother = NULL;
	obj->child   = NULL;
	obj->father  = NULL;
	obj->design  = design;
	obj->val     = val;
	obj->event   = NULL;
	obj->event_flag = 0;
	return obj;
}

void lz_obj_set_event(lz_obj_t * obj, void (*event) (lz_touch_val_t *val)) {
	obj->event = event;
}

void lz_obj_set_x(lz_obj_t * obj, int x) {
	obj->layout.point.x = x;
}

int lui_obj_get_x(lz_obj_t * obj) {
	return obj->layout.point.x;
}


void lz_obj_set_y(lz_obj_t * obj, int y) {
	obj->layout.point.y = y;
}

void lz_obj_set_width(lz_obj_t * obj, int width) {
	obj->layout.size.width = width;
}

void lz_obj_set_length(lz_obj_t * obj, int length) {
	obj->layout.size.length = length;
}

static lz_obj_t * _lui_get_last_brother(lz_obj_t * obj) {
	if (obj->brother == NULL) {
		return obj;
	} else {
		return _lui_get_last_brother(obj->brother);
	}
}

static lz_obj_t * _lui_get_last_child(lz_obj_t * obj) {
	if (obj->child == NULL) {
		return obj;
	} else {
		return _lui_get_last_brother(obj->child);
	}
}

void lz_obj_add_brother(lz_obj_t * obj, lz_obj_t * brother) {
	lz_obj_t * t;
	t = _lui_get_last_brother(obj);
	brother->father = obj->father;
	t->brother = brother;
}

void lz_obj_add_child(lz_obj_t * obj, lz_obj_t * child) {
	lz_obj_t * t;
	t = _lui_get_last_child(obj);
	child->father = obj;
	if(obj->child == NULL) {
		t->child = child;
	} else {
		t->brother = child;
	}
}


void lui_obj_find_up(lz_obj_t * root, lz_obj_t * obj) {
	if (root == NULL) {
		return;
	} else {
		if(root->child == obj) {
			root->child = obj->brother;
			return;
		} else if(root->brother == obj) {
			root->brother = obj->brother;
			return;
		}
		lui_obj_find_up(root->child,obj);
		lui_obj_find_up(root->brother,obj);
	}
}

static void _lz_obj_distroy(lz_obj_t ** obj) {
	lz_obj_t *pl, *pr;
	if((*obj) == NULL) {
		return ;
	} else {
		pl = (*obj)->child;
		pr = (*obj)->brother;
		(*obj)->child = NULL;
		(*obj)->brother = NULL;
		lz_free((*obj)->val);
		lz_free(*obj);
		(*obj) = NULL;
		_lz_obj_distroy(&pl);
		_lz_obj_distroy(&pr);
	}
}

void lz_obj_distroy(lz_obj_t ** obj) {
	if((*obj) != NULL) {
		lui_obj_find_up(&lui_root,(*obj));
		_lz_obj_distroy(&((*obj)->child));
	}
}

void lz_obj_traverse(lz_obj_t * obj) {
	if (obj == NULL) {
		tree_layer --;
		return;
	} else {
		tree_layer++;
		if(last_tree_layer > tree_layer) {
			int i = last_tree_layer-tree_layer;
			while(i > 0) {
				i--;
				if(last_stack != NULL) {
					stack_point.x -= last_stack->layout.point.x;
					stack_point.y -= last_stack->layout.point.y;
					last_stack = last_stack->father;
				}
			}
			f_layout = mlayout[tree_layer-1];
		}
		last_tree_layer = tree_layer;
		if(obj->child != NULL) {
			if(obj->father == NULL) {
				tree_layer = 0;
				stack_point.x = 0;
				stack_point.y = 0;
				f_layout.point.x = 0;
				f_layout.point.y = 0;
				f_layout.size.width = LCD_WIDTH;
				f_layout.size.length = LCD_LENGTH;
				mlayout[0] = f_layout;
				last_stack = obj;
			} else {
				int g_y = f_layout.point.y + f_layout.size.length;
				int g_x = f_layout.point.x + f_layout.size.width;
				last_stack = obj;
				stack_point.x += last_stack->layout.point.x;
				stack_point.y += last_stack->layout.point.y;

				f_layout.size.width = last_stack->layout.size.width;
				int x_x = stack_point.x + f_layout.size.width;
				if(f_layout.point.x < stack_point.x) {
					f_layout.point.x = stack_point.x;
				}
				if(x_x > g_x) {
					int c_x = x_x - g_x;
					if(c_x >= f_layout.size.width) {
						f_layout.size.width = 0;
						// tree_layer --;
						// return;
					} else {
						f_layout.size.width -= c_x;
					}
				}

				f_layout.size.length = last_stack->layout.size.length;
				int x_y = stack_point.y + f_layout.size.length;
				if(f_layout.point.y < stack_point.y) {
					f_layout.point.y = stack_point.y;
				}
				if(x_y > g_y) {
					int c_y = x_y - g_y;
					if(c_y >= f_layout.size.length) {
						f_layout.size.length = 0;
						// tree_layer --;
						// return;
					} else {
						f_layout.size.length -= c_y;
					}
				}
				if(tree_layer >= 0 && tree_layer < 20) {
					mlayout[tree_layer] = f_layout;
				}
			}
		}
		if(obj->design != NULL) {
			lz_point_t point;
			if(last_stack == obj) {
				point.x = stack_point.x;
				point.y = stack_point.y;
			} else {
				point.x = stack_point.x + obj->layout.point.x;
				point.y = stack_point.y + obj->layout.point.y;
			}
			obj->design(obj,&point);
		}
		lz_obj_traverse(obj->child);
		lz_obj_traverse(obj->brother);
	}
}

void lz_obj_coupoint(lz_obj_t * obj, lz_point_t * point) {
	if(obj->father == NULL) {
		return;
	} else {
		point->x += obj->father->layout.point.x;
		point->y += obj->father->layout.point.y;
		lz_obj_coupoint(obj->father, point);
	}
}

void _lz_obj_even(lz_obj_t * obj, int x, int y, uint8_t flag) {
	if (obj == NULL) {
		tree_layer --;
		return;
	} else {
		tree_layer++;
		if(last_tree_layer > tree_layer) {
			int i = last_tree_layer-tree_layer;
			while(i > 0) {
				i--;
				if(last_stack != NULL) {
					stack_point.x -= last_stack->layout.point.x;
					stack_point.y -= last_stack->layout.point.y;
					last_stack = last_stack->father;
				}
			}
			f_layout = mlayout[tree_layer-1];
		}
		last_tree_layer = tree_layer;
		if(obj->child != NULL) {
			if(obj->father == NULL) {
				tree_layer = 0;
				stack_point.x = 0;
				stack_point.y = 0;
				f_layout.point.x = 0;
				f_layout.point.y = 0;
				f_layout.size.width = LCD_WIDTH;
				f_layout.size.length = LCD_LENGTH;
				mlayout[0] = f_layout;
				last_stack = obj;
			} else {
				int g_y = f_layout.point.y + f_layout.size.length;
				int g_x = f_layout.point.x + f_layout.size.width;
				last_stack = obj;
				stack_point.x += last_stack->layout.point.x;
				stack_point.y += last_stack->layout.point.y;

				f_layout.size.width = last_stack->layout.size.width;
				int x_x = stack_point.x + f_layout.size.width;
				if(f_layout.point.x < stack_point.x) {
					f_layout.point.x = stack_point.x;
				}
				if(x_x > g_x) {
					int c_x = x_x - g_x;
					if(c_x >= f_layout.size.width) {
						f_layout.size.width = 0;
					} else {
						f_layout.size.width -= c_x;
					}
				}

				f_layout.size.length = last_stack->layout.size.length;
				int x_y = stack_point.y + f_layout.size.length;
				if(f_layout.point.y < stack_point.y) {
					f_layout.point.y = stack_point.y;
				}
				if(x_y > g_y) {
					int c_y = x_y - g_y;
					if(c_y >= f_layout.size.length) {
						f_layout.size.length = 0;
					} else {
						f_layout.size.length -= c_y;
					}
				}
				mlayout[tree_layer] = f_layout;
			}
		}
		if(obj->design != NULL) {
			lz_point_t point;
			if(last_stack == obj) {
				point.x = stack_point.x;
				point.y = stack_point.y;
			} else {
				point.x = stack_point.x + obj->layout.point.x;
				point.y = stack_point.y + obj->layout.point.y;
			}
			if(obj->event != NULL) {
				if( ( ( x >= point.x && x <= (point.x + obj->layout.size.width) )
				      && ( y >= point.y && y <= (point.y + obj->layout.size.length) ) )
				    || obj->event_flag == 1 ) {
					touch_val.event = obj->event;
					touch_val.abs_x = x - point.x;
					touch_val.abs_y = y - point.y;
					touch_val.rel_x = x;
					touch_val.rel_y = y;
					touch_val.falg  = flag;
					touch_val.obj   = obj;
				}
			}
		}
		if(obj->event_flag == 0) {
			_lz_obj_even(obj->child,x,y,flag);
			_lz_obj_even(obj->brother,x,y,flag);
		}
	}
}

/* everything base time so need tick count even touch or sollow or other */
void lz_obj_even(int x, int y, uint8_t flag) {
	if((flag == 0 || flag == 1) && touch_val.event != NULL) {
		touch_val.abs_x = x - (touch_val.rel_x - touch_val.abs_x);
		touch_val.abs_y = y - (touch_val.rel_y - touch_val.abs_y);
		touch_val.rel_x = x;
		touch_val.rel_y = y;
		touch_val.falg  = flag;
	} else {
		touch_val.obj = NULL;
		touch_val.event = NULL;
		_lz_obj_even(&lui_root,x,y,flag);
	}
	if(touch_val.event != NULL) {
		if(touch_val.falg == 2) {
			touch_val.obj->event_flag = 1;
		}
		if(touch_val.falg == 0) {
			touch_val.obj->event_flag = 0;
		}
		touch_val.event(&touch_val);
	}
}
