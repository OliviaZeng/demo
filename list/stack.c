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


void init(Stack *stack)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (NULL == node)
        exit(1);

    stack->top = stack->bottom = node;
    stack->top->next = NULL;
}

void push(Stack *stack, int data)
{
    if (NULL == stack)
        return;

    Node *node = (Node *)malloc(sizeof(Node));
    if (NULL == node)
        return;

    node->data = data;
    node->next = stack->top;
    stack->top = node;
}

int pop(Stack *stack, int *data)
{
    if (NULL == stack)
        return -1;

    if (1 == is_empty(stack))
        return -1;

    Node *node = stack->top;
    *data = node->data;
    stack->top = node->next;
    free(node);
    node = NULL;
    return 0;
}

int is_empty(Stack *stack)
{
    if (NULL == stack)
        return -1;
    if (stack->top == stack->bottom)
        return 1;
    else
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
    if (1 == is_empty(stack))
        return;

    Node *node = stack->top;
    Node *next = next;

    while (node != stack->bottom) {
        next = node->next;
        free(node);
        node = next;
    }
    stack->top = stack->bottom;
}

int main(int argc, char *argv[])
{
    Stack s;
    Stack *stack = &s;
    int i = 0;
    init(stack);
    push(stack, rand()%20);
    push(stack, rand()%20);
    push(stack, rand()%20);
    push(stack, rand()%20);
    push(stack, rand()%20);
    print(stack);
    pop(stack, &i);
    print(stack);
    return 0;
}
