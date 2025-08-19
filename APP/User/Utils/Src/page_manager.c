


#include "page_manager.h"



page_stack_t page_stack; // 整个页面的栈



/* ------------------------------------栈的基础操作------------------------------------ */

static void page_stack_init(page_stack_t* page_stack)
{
		page_stack->top = 0;
}


static uint8_t page_stack_push(page_stack_t* page_stack,page_t* new_page)
{
	if(page_stack->top >= MAX_PAGES)
	{
		return 0;
	}

	page_stack->pages[page_stack->top++] = new_page;
	
	return 1;
}



static uint8_t page_stack_pop(page_stack_t* page_stack)
{
	if(page_stack->top <= 0)
	{
		return 0;
	}

	--page_stack->top;
	
	return 1;
}


static page_t* page_stack_top(page_stack_t* page_stack)
{
	 if(page_stack->top == 0)
	 {
		 return NULL;
	 }
	 
	 return page_stack->pages[page_stack->top-1];
}



static uint8_t page_stack_empty(page_stack_t* page_stack)
{
	return (page_stack->top == 0);
}


/* ------------------------------------ 页面管理对外接口 ------------------------------------*/

void pages_init(page_t* page_home)
{
		page_stack_init(&page_stack); // 初始化页面栈
		page_home->init(); //初始化主页
		page_stack_push(&page_stack,page_home); // 主页入栈
		lv_disp_load_scr(*page_home->page_obj); //显示Home页面
}

void page_load(page_t* new_page)
{
	//检查堆栈是否满
	if(page_stack.top>= MAX_PAGES)
	{
			return;
	}
	
	/* 当前页面处理 */
//	page_t* cur_page = page_stack_top(&page_stack);
//	if(cur_page)
//	{
//		 cur_page->pause(); //暂停当前页面定时器、动画等
//	}
	
	new_page->init(); // 新页面初始化
	page_stack_push(&page_stack,new_page); // 新页面入栈
	lv_screen_load_anim(*new_page->page_obj, LV_SCREEN_LOAD_ANIM_MOVE_RIGHT, 200, 0, true); //加载新页面(有动画),自动释放旧页面
}


void page_back()
{	

  if(page_stack.top <= 1) 
	{
        //最多回到首页
      return;
  }
	
	page_stack_pop(&page_stack); // 弹出当前页面
	
	page_t* previous_page = get_top_page(&page_stack);
	
	if(!previous_page){
		 return ;
	}
	previous_page->init();
	
	//prev->resume();  // 启动当前页面定时器、动画等
	lv_screen_load_anim(*previous_page->page_obj, LV_SCREEN_LOAD_ANIM_MOVE_RIGHT, 200, 0, true); //加载新页面(有动画),不删除旧screen
	
}


page_t* get_top_page(page_stack_t* page_stack)
{
		return page_stack_top(page_stack);
}

