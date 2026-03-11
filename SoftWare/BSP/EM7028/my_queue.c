#include "my_queue.h"


// ��ʼ������
void initQueue(Queue *queue) 
{
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
}

// �ж϶����Ƿ�Ϊ��
bool isQueueEmpty(Queue *queue) 
{
    return queue->size == 0;
}

// �ж϶����Ƿ�����
bool isQueueFull(Queue *queue) 
{
    return queue->size == QUEUE_SIZE;
}

// ���
void enqueue(Queue *queue, unsigned long item) 
{
    if (isQueueFull(queue)) 
    {
        return;
    }
    queue->rear = (queue->rear + 1) % QUEUE_SIZE;
    queue->data[queue->rear] = item;
    queue->size++;
}

// ����
uint32_t dequeue(Queue *queue) 
{
    if (isQueueEmpty(queue)) 
    {
        return 0;
    }
    unsigned long item = queue->data[queue->front];
    queue->front = (queue->front + 1) % QUEUE_SIZE;
    queue->size--;
    return item;
}

