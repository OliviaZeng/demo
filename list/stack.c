#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node *next;
}Node;

typedef struct stack {
    Node *top;
    Node *bottom;
}Stack;


Stack *init(void)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (NULL == stack)
        return NULL;

    Node *node = (Node *)malloc(sizeof(Node));
    if (NULL == node)
        exit(1);
    node->data = -1;
    node->next = NULL;
    stack->top = stack->bottom = node;
    return stack;
}

int is_empty(Stack *stack)
{
    if (NULL == stack || NULL == stack->top || NULL == stack->bottom)
        return -1;
    if (stack->top == stack->bottom)
        return 1;
    else
        return 0;
}

int destory(Stack *stack)
{
    if (-1 == is_empty(stack))
        return -1;

    Node *node = stack->top;
    Node *tmp = NULL;
    while (node) {
        tmp = node->next;
        free(node);
        node = tmp;
    }
    free(stack);
    return 0;
}

int push(Stack *stack, int data)
{
    if (-1 == is_empty(stack))
        return -1;

    Node *node = (Node *)malloc(sizeof(Node));
    if (NULL == node)
        return -1;

    node->data = data;
    node->next = stack->top;
    stack->top = node;
    return 0;
}

int pop(Stack *stack, int *data)
{
    if (0 != is_empty(stack) || NULL == data)
        return -1;

    Node *node = stack->top;
    *data = node->data;
    stack->top = node->next;
    free(node);
    return 0;
}

void print(Stack *stack)
{
    if (0 != is_empty(stack))
        return;

    Node *node = stack->top;
    while (node != stack->bottom) {
        printf("%d ", node->data);
        node = node->next;
    }
    printf("\n");
}

void clear(Stack *stack)
{
    if (0 != is_empty(stack))
        return;

    Node *node = stack->top;
    Node *next = NULL;

    while (node != stack->bottom) {
        next = node->next;
        free(node);
        node = next;
    }
    stack->top = stack->bottom;
    stack->bottom->next = NULL;
}

int main(int argc, char *argv[])
{
    int i = 0;
    Stack *stack = init();
    if (NULL == stack)
        exit(1);

    push(stack, rand()%20);
    push(stack, rand()%20);
    push(stack, rand()%20);
    print(stack);
    pop(stack, &i);
    print(stack);
    clear(stack);
    if (is_empty(stack))
        printf("is empty\n");
    if (0 == destory(stack))
        printf("destory succ\n");
    return 0;
}
