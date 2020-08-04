//
// Created by a6735 on 2020/8/2.
//

#include "lzui/lz_obj_lable.h"
#include "lzui/lz_draw.h"

static void lui_text_design (struct _lui_obj_t * obj, lui_point_t *point);
//static void lui_text_event(lui_touch_val_t *val);
static int text_utf8_to_unicode(uint16_t * unicode, char *utf8);

extern const struct font_t mac_font_20_english;

lui_obj_t * lui_create_lable(int x,int y) {
	lui_text_t * obj_text = lui_malloc(sizeof(lui_text_t));
	obj_text->color = lui_color_888_to_565(lui_color_red);
	obj_text->font_val = mac_font_20_english;
	sprintf(obj_text->text,"123456.VA");
	lui_obj_t * obj = lui_create_obj(x,y,50,50,obj_text,lui_text_design);
	lui_obj_set_event(obj,NULL);
	return obj;
}

static void lui_text_design (struct _lui_obj_t * obj, lui_point_t *point) {
	lui_text_t * obj_text = obj->val;
	uint8_t type = 0;
	uint16_t adr = 0;
	int ax = point->x;
	int ay = point->y;
	char * text = (char *)obj_text->text;
	while( *text ) {
		if(*text == '\r') {
			text++;
		} else if(*text == ' ') {
//			display_fill(ax+1, ay+1, display_val_v.english.w,display_val_v.english.h,display_val_v.back_color);
//			if(ax > (320 - display_val_v.english.w) ) {
//				ay += display_val_v.english.h;
//				ax = x;
//			} else {
//				ax += display_val_v.english.w - 1;
//			}
			text += 1;
		} else if(*text == '\n') {
			ay += obj_text->font_val.w;
			ax = point->x;
			text++;
		} else {
			type = text_utf8_to_unicode(&adr, text);
			if(type == 0) {
				uint8_t i = 0;
				for(; i < obj_text->font_val.num; i++) {
					if(obj_text->font_val.text_val[i].text[0] == text[0]) {
						uint8_t * addr = (uint8_t *)(obj_text->font_val.base_adr+i*obj_text->font_val.size);
						lui_draw_font(ax, ay,obj_text->font_val.w, obj_text->font_val.h,obj_text->color, addr);
						break;
					}
				}

				text += 1;
				if(ax > (320 - obj_text->font_val.w) ) {
					ay += obj_text->font_val.h;
					ax = point->x;
				} else {
					ax += obj_text->font_val.w - 1;
				}
			} else {
//				uint8_t i = 0;
//				for(; i < display_val_v.china.num; i++) {
//					if( (display_val_v.china.text_val[i].text[0] == text[0]) &&
//					    (display_val_v.china.text_val[i].text[1] == text[1]) &&
//					    (display_val_v.china.text_val[i].text[2] == text[2]) ) {
//						lcd_open_window(ax, ay, display_val_v.china.w,display_val_v.china.h);
//						uint8_t * addr = (uint8_t *)(display_val_v.china.base_adr+i*display_val_v.china.size);
//						for(uint32_t add = 0; add < (display_val_v.china.size); add++) {
//							uint16_t color_t = display_val_v.back_color;
//							if(addr[add] != 0) {
//								if(addr[add] == 0xff) {
//									color_t = display_val_v.text_color;
//								} else {
//									color_t = lui_color_alpha_blend(display_val_v.text_color, display_val_v.back_color, (addr[add]) );
//								}
//							}
//							display_write_color_565(color_t);
//						}
//						break;
//					}
//				}
//
//				text += 3;
//				if(ax > (320 - display_val_v.china.w) ) {
//					ay += display_val_v.china.h;
//					ax = x;
//				} else {
//					ax += display_val_v.china.w + 2;
//				}
			}
		}
	}
}

//static void lui_text_event(lui_touch_val_t *val) {
//
//}

static int text_utf8_to_unicode(uint16_t * unicode, char *utf8) {
	if (0 == (*utf8 & 0x80)) {
		/*
		 * single-byte char
		 */
		*unicode = *utf8 - ' ';
	} else {
		/*
		 * 3-byte char (chinese char)
		 */
		if ( (*utf8 & 0xf0) == 0xe0 ) {
			unsigned char code[2];
			code[0] = ((utf8[0] & 0x0f) <<4) + ((utf8[1] & 0x3c) >>2);
			code[1] = ((utf8[1] & 0x03) <<6) + (utf8[2] & 0x3f);
			*unicode =  ((code[0] << 8) | code[1]) - 19968; //- 19968;
			return 1;
		} else {
			/*
			 * ? not chaina
			 */
			return 2;
		}
	}
	return 0;
}
