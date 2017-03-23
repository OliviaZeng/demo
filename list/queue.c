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


void init(Queue *queue)
{
    if (NULL == queue)
        return;
    queue->front = queue->rear = (Node *)malloc(sizeof(Node));
    if (NULL == queue->front)
        exit(1);
    queue->front->next = NULL;
}

int destory(Queue *queue)
{
    if (NULL == queue)
        return -1;

    while (queue->front) {
        queue->rear = queue->front->next;
        free(queue->front);
        queue->front = queue->rear;
    }
    free(queue);
    return 0;
}

void insert(Queue *queue, int data)
{
    if (NULL == queue)
        return ;
    Node *node = (Node *)malloc(sizeof(Node));
    if (NULL == node)
        return ;

    node->data = data;
    node->next = NULL;
    queue->rear->next = node;
    queue->rear = node;
}

int delete(Queue *queue)
{
    if (NULL == queue)
        return -1;
    if (queue->front == queue->rear)
        return -1;

    int data = 0;
    Node *node = queue->front->next;
    data = node->data;
    queue->front->next = node->next;
    if (queue->rear == node)
        queue->rear = queue->front;
    free(node);
    return data;
}

int is_empty(Queue *queue)
{
    if (NULL == queue)
        return -1;
    if (queue->front == queue->rear)
        return 1;
    else
        return 0;
}

void print(Queue *queue)
{
    if (NULL == queue)
        return ;
    Node *node = queue->front->next;
    while (NULL != node) {
        printf("%d ", node->data);
        node = node->next;
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    Queue q;
    Queue *queue = &q;
    int i = 0;

    init(queue);
    for (i=0; i<10; i++)
        insert(queue, rand() % 20);
    print(queue);
    delete(queue);
    print(queue);
    insert(queue, 21);
    print(queue);
    return 0;

    
}
