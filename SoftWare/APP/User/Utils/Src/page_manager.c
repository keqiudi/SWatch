


#include "page_manager.h"



page_stack_t page_stack; // ����ҳ���ջ



/* ------------------------------------ջ�Ļ�������------------------------------------ */

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


/* ------------------------------------ ҳ���������ӿ� ------------------------------------*/

void pages_init(page_t* page_home)
{
		page_stack_init(&page_stack); // ��ʼ��ҳ��ջ
		page_home->init(); //��ʼ����ҳ
		page_stack_push(&page_stack,page_home); // ��ҳ��ջ
		lv_disp_load_scr(*page_home->page_obj); //��ʾHomeҳ��
}

void page_load(page_t* new_page)
{
	//����ջ�Ƿ���
	if(page_stack.top>= MAX_PAGES)
	{
			return;
	}
	
	/* ��ǰҳ�洦�� */
	page_t* cur_page = page_stack_top(&page_stack);
	if(cur_page){
		 cur_page->pause(); //��ͣ��ǰҳ�涨ʱ����������
		 //cur_page->deinit(); // ʹ�ö���������ע�͵����������ʿ�ָ��
	}
	
	/* ��ҳ�洦�� */
	new_page->init(); // ��ҳ���ʼ��
	new_page->resume(); // ��ҳ��������ʱ����
	page_stack_push(&page_stack,new_page); // ��ҳ����ջ
	
	//lv_scr_load(*new_page->page_obj);
	lv_screen_load_anim(*new_page->page_obj, LV_SCREEN_LOAD_ANIM_MOVE_RIGHT, 200, 0, true); //������ҳ��(�ж���),�Զ������ͷž�screen
}


void page_back()
{	

  if(page_stack.top <= 1) 
	{
        //���ص���ҳ
      return;
  }
	
	/*��ǰҳ�洦��*/
	page_t* cur_page = page_stack_top(&page_stack);
	if(cur_page)
	{
		 cur_page->pause(); //��ͣ��ǰҳ�涨ʱ����������
		 //cur_page->deinit(); // ʹ�ö���������ע�͵����������ʿ�ָ��
	}
	page_stack_pop(&page_stack); // ������ǰҳ��
	
	/* ��ҳ�洦�� */
	page_t* previous_page = get_top_page(&page_stack);
	if(!previous_page){
		 return ;
	}
	previous_page->init();
	previous_page->resume();  // ������ǰҳ�涨ʱ����������
	
	//lv_scr_load(*previous_page->page_obj); // �������ͷž�ҳ��,��������ҳ��ͬʱ����ռ��heap���ߣ�ֻ����û�ж�������
	lv_screen_load_anim(*previous_page->page_obj, LV_SCREEN_LOAD_ANIM_MOVE_RIGHT, 200, 0, true); //������ҳ��(�ж���),�Զ������ͷž�screen
}


page_t* get_top_page()
{
	return page_stack_top(&page_stack);
}

