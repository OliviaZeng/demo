#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int data;
    struct node *next;
}Node, List;

List *init(void)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (NULL == node)
        return NULL;

    node->data = 0;
    node->next = node;
    return node;
}


void print(List *list)
{
    if (NULL == list)
        return;

    Node *p = list->next;
    while (p && p != list) {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

int length(List *list)
{
    int len = 0;
    Node *p = NULL;
    if (NULL == list)
        return -1;

    p = list->next;
    while (p && p !=list) {
        ++len;
        p = p->next;
    }

    return len;
}

int insert(List *list, int pos, int data)
{
    int len = 0;
    if (-1 == (len=length(list)))
        return -1;

    if (pos > 0 && pos <= len+1) {
        Node *tmp = list;
        Node *node = (Node *)malloc(sizeof(Node));
        if (NULL == node)
            return -1;
        node->data = data;
        node->next = NULL;

        while (1) {
            pos--;
            if (0 == pos)
                break;
            tmp = tmp->next;
        }

        node->next = tmp->next;
        tmp->next = node;
        return 0;
    } else {
        return -1;
    }
}

int delete(List *list, int pos)
{
    int len = 0;
    Node *next = NULL;
    Node *pre = NULL;
    if (-1 == (len=length(list)))
        return -1;

    pre = list;
    if (pos > 0 && pos < len+1) {
        while (1) {
            pos--;
            if (0 == pos)
                break;
            pre = pre->next;
        }
        next = pre->next->next;
        free(pre->next);
        pre->next = next;
        return 0;
    } else {
        return -1;
    }
    
}

int find(List *list, int pos, int *data)
{
    int len = 0;
    if (-1 == (len=length(list)))
        return -1;

    if (pos > 0 && pos <= len) {
        Node *cur = list;
        while (pos--) {
            cur = cur->next;
        }
        *data = cur->data;
        return 0;
    } else {
        return -1;
    }
}

int modify(List *list, int pos, int data)
{
    int len = 0;
    if (-1 == (len=length(list)))
        return -1;

    if (pos > 0 && pos <= len) {
        Node *cur = list;
        while (pos--)
            cur = cur->next;
        cur->data = data;
        return 0;
    } else {
        return -1;
    }
}


int main(int argc, char *argv[])
{
    int i = 0, data = 0;
    List *list = init();
    if (NULL == list) {
        printf("create head err\n");
        exit(1);
    }

    insert(list, 1, 1);
    insert(list, 1, 2);
    insert(list, 1, 3);
    insert(list, 1, 4);
    insert(list, 1, 5);
    insert(list, 1, 6);
    print(list);
    modify(list, 5,123);
    print(list);
    if (-1 != (i=find(list, 4, &data)))
        printf("i=%d\n", data);
    delete(list, 3);
    print(list);
    return 0;
}
