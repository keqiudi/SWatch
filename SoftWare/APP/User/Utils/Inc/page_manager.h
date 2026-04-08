#ifndef __PAGE_MANAGERA_H
#define __PAGE_MANAGERA_H

#include "lvgl.h"


#define MAX_PAGES 8

typedef struct
{
	lv_obj_t **page_obj;
	void (*init)(void);
	void (*deinit)(void);
	void (*resume)(void);
	void (*pause)(void);
	//uint8_t is_created; 
}page_t;


typedef struct
{
	page_t *pages[MAX_PAGES];
	int top;
}page_stack_t;


void pages_init();

void page_load(page_t* new_page);

void page_back();

void page_back_bottom();

page_t* get_top_page();

#endif 