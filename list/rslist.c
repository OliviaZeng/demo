#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int data;
    struct node *next;
}Node;

Node *head = NULL;

Node * create(int data)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (NULL == node)
        return NULL;
    node->data = data;
    node->next = NULL;
    return  node;
}

Node * create_head(int data)
{
    if (NULL == (head = create(data)))
        return NULL;

    head->next = head;
    return head;
}

void add(int data, int n)
{
    Node *tail = NULL
    Node *node = NULL;
    if (NULL == head) {
        create_head(data);
        exit(1);
    }
    tail = head;

    if (i=0; i<n; i++) {
        node = (Node *)malloc(sizeof(Node));
        if (NULL == node)
            exit(1);

        node->data = data;
        node->next = head;
        tail->next = node;
        tail = node;
    }
}

void printf(void)
{
    if (NULL == head)
        return;

    Node *p = head->next;
    while (p && p != head) {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

int length(void)
{
    int len = 0;
    Node *p = NULL;
    if (NULL == head)
        return -1;

    p = head->next;
    while (p && p != head) {
        ++len;
        p = p->next;
    }

    return len;
}

int insert(int pos, int data)
{
    int len = 0;
    if (-1 == (len=length()))
        return -1;

    if (pos > 0 && pos <= len+1) {
        Node *tmp = head;
        Node *node = create(data);
        if (NULL == node)
            return -1;

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

int delete(int pos)
{
    int len = 0;
    Node *next = NULL;
    Node *pre = NULL;
    if (-1 == (len=length()))
        return -1;

    pre = head;
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


int main(int argc, char *argv[])
{
    if (NULL == create_head(1)) {
        printf("create head err\n");
        exit(1);
    }


}
