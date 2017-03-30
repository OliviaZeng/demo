#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    struct node *next;
    int data;
}Node;

typedef struct queue {
    struct node *front;
    struct node *rear;
}Queue;

Queue *init(void)
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    if (NULL == queue)
        return NULL;

    Node *node = (Node *)malloc(sizeof(Node));
    if (NULL == node)
        return NULL;

    node->data = -1;
    node->next = NULL;
    queue->front = queue->rear = node;
    return queue;
}

int is_empty(Queue *queue)
{
    if (NULL == queue || NULL == queue->front || NULL == queue->rear)
        return -1;
    if (queue->front == queue->rear)
        return 1;
    else
        return 0;
}

int destory(Queue *queue)
{
    if (-1 == is_empty(queue))
        return -1;

    Node *p = queue->front;
    Node *tmp = NULL;
    while (p) {
        tmp = p->next;
        free(p);
        p = tmp;
    }

    free(queue);
    return 0;
}

int insert(Queue *queue, int data)
{
    if (-1 == is_empty(queue))
        return -1;

    Node *node = (Node *)malloc(sizeof(Node));
    if (NULL == node)
        return -1;
    node->data = data;
    node->next = NULL;
    queue->rear->next = node;
    queue->rear = node;

    return 0;
}

int delete(Queue *queue, int *data)
{
    if (0 != is_empty(queue) || NULL == data)
        return -1;

    Node *node = queue->front->next;
    *data = node->data;
    queue->front->next = node->next;
    if (node == queue->rear)
        queue->rear = queue->front;
    free(node);
    return 0;
}

void print(Queue *queue)
{
    if (0 != is_empty(queue))
        return;
    Node *node = queue->front->next;
    while (NULL != node) {
        printf("%d ", node->data);
        node = node->next;
    }
    printf("\n");
}

void clear(Queue *queue)
{
    if (0 != is_empty(queue))
        return;

    Node *node = queue->front->next;
    Node *tmp = NULL;

    while (node) {
        tmp = node->next;
        free(node);
        node = tmp;
    }
    queue->rear = queue->front;
    queue->front->next = NULL;
}

int main(int argc, char *argv[])
{
    int i = 0;
    int data = 0;
    Queue *queue = init();
    if (NULL == queue)
        exit(1);

    for (i=0; i<10; i++)
        insert(queue, rand() % 20);
    print(queue);
    delete(queue, &data);
    print(queue);
    insert(queue, 21);
    print(queue);
    clear(queue);
    if (is_empty(queue))
        printf("is empty\n");
    if (0 == destory(queue))
        printf("destory succ\n");
    return 0;

    
}
