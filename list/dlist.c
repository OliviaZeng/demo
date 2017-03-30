#include <stdio.h>
#include <stdlib.h>

#define HEAD 1
#define TAIL 0

typedef struct node {
    struct node *pre;
    struct node *next;
    int value;
}Node;

typedef struct list {
    Node *head;
    Node *tail;
    unsigned long len;
}List;

typedef struct itor {
    Node *next;
    Node *pre;
    int direction;
}Itor;


List *create(void)
{
    List *list = (List *)malloc(sizeof(List));
    if (NULL == list)
        return NULL;

    list->head = NULL;
    list->tail = NULL;
    list->len  = 0;
    return list;
}

List *add_node_head(List *list, int data)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (NULL == node)
        return NULL;
    node->value = data;
    if (0 == list->len) {
        list->head = list->tail = node;
        node->pre = node->next = NULL;
    } else {
        node->pre = NULL;
        node->next = list->head;
        list->head->pre = node;
        list->head = node;
    }
    list->len++;
    return list;
}

List *add_node_tail(List *list, int data)
{
    Node *node =  (Node *)malloc(sizeof(Node));
    if (NULL == node)
        return NULL;
    node->value = data;
    if (0 == list->len) {
        list->head = list->tail = node;
        node->pre = node->next = NULL; 
    } else {
        node->next = NULL;
        node->pre = list->tail;
        list->tail->next = node;
        list->tail = node;
    }
    list->len++;
    return list;
}

void list_del_node(List *list, Node *node)
{
    if (node->pre) {
        node->pre->next = node->next;
    } else {
        list->head = node->next;
    }

    if (node->next) {
        node->next->pre = node->pre;
    } else {
        list->tail = node->pre;
    }
    free(node);
    list->len--;
}

void list_release(List *list)
{
    if (NULL == list)
        return;
    int len;
    Node *cur = NULL;
    Node *next = NULL;
    cur = list->head;
    len = list->len;
    while (len--) {
        next = cur->next;
        free(cur);
        cur = next;
    }
    free(list);
}

Itor *list_get_itor(List *list, int direction)
{
    Itor *itor = (Itor *)malloc(sizeof(Itor));
    if (NULL == itor)
        return NULL;
    if (NULL == list)
        return NULL;

    if (HEAD == direction)
        itor->next = list->head;
    else
        itor->next = list->tail;
    itor->direction =  direction;
    return itor;
}

void list_free_itor(Itor *itor)
{
    if (NULL == itor)
        return;
    else
        free(itor);
}

Node *get_next_node(Itor *itor)
{
    if (NULL == itor)
        return NULL;

    Node *node = itor->next;
    if (NULL != node) {
        if (HEAD == itor->direction)
            itor->next = node->next;
        else
            itor->next = node->pre;
    }
    return node;
}

Node *list_serch_key(List *list, int key)
{
    if (NULL == list)
        return NULL;

    Itor *itor = list_get_itor(list, HEAD);
    if (NULL == itor)
        return NULL;

    Node *node = NULL;

    while (NULL != (node=get_next_node(itor))) {
        if (key == node->value)
            break;
    }
    list_free_itor(itor);
    return node;
}

int main(int argc, char *argv[])
{
    int i;
    return 0;
    
}
