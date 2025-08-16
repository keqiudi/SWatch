


#include "page_manager.h"



page_stack_t page_stack; // 整个页面的栈



/* 栈的基础操作 */

static void page_stack_init(page_stack_t* page_stack)
{
		page_stack->top = 0;
}


static uint8_t page_stack_push(page_stack_t* page_stack,page_t* new_page)
{
	if(page_stack->top >= MAX_PAGES)
	{
		return -1;
	}

	page_stack->pages[page_stack->top++] = new_page;
	
	return 0;
}



static uint8_t page_stack_pop(page_stack_t* page_stack)
{
	if(page_stack->top <= 0)
	{
		return -1;
	}
	
	page_stack->pages[--page_stack->top]->deinit();
	
	return 0;
}


static page_t* get_top_page(page_stack_t* page_stack)
{
	 if(page_stack->top == 0)
	 {
		 return NULL;
	 }
	 
	 return page_stack->pages[page_stack->top-1];
}



static uint8_t is_page_stack_empty(page_stack_t* page_stack)
{
	return (page_stack->top == 0);
}


/* 页面管理对外接口 */

void pages_init(page_t* page_home)
{
		page_stack_init(&page_stack);
		page_stack_push(&page_stack,page_home);
		page_home->init();
}


page_t* get_current_page(page_stack_t* page_stack)
{
		get_top_page(page_stack);
}


void page_load(page_t* new_page)
{
	//检查堆栈是否满
	if(page_stack.top>= MAX_PAGES-1) 
	{
			return;
	}
	
	//如果堆栈非空,将所在页面deinit
	if(page_stack.top> 0)
	{
		 /* 不用出栈,仅隐藏即可*/
		 page_stack.pages[page_stack.top - 1]->deinit();
	}
	
	//新页面入堆栈
	page_stack_push(&page_stack,new_page);
	new_page->init(); //初始化新页面
	lv_screen_load_anim(*new_page->page_obj,LV_SCREEN_LOAD_ANIM_MOVE_RIGHT,100,0,true); //添加动画加载页面,事件100ms
}


void page_back()
{
	
}



