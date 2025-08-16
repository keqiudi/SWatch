#ifndef __PAGE_MANAGERA_H
#define __PAGE_MANAGERA_H

#include "lvgl.h"


#define MAX_PAGES 8

typedef struct
{
	void (*init)(void);
	void (*deinit)(void);
	lv_obj_t **page_obj;
}page_t;


typedef struct
{
	page_t *pages[MAX_PAGES];
	int top;
}page_stack_t;


void pages_init();

page_t* get_current_page(page_stack_t* page_stack);

void page_load(page_t* new_page);

void page_back();



#endif 