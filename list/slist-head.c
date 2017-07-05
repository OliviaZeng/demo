#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int data;
    struct node *next;
}Node, List;

List *init(void)
{
    List *list = (Node *)malloc(sizeof(Node));
    if (NULL == list)
        return NULL;
    list->data = 0;
    list->next = NULL;
    return list;
}

int is_empty(List *list)
{
    if (NULL == list)
        return -1;
    else
        return NULL == list->next;
}

int destroy(List *list)
{
    if (-1 == is_empty(list))
        return -1;

    Node *node = list;
    Node *tmp = NULL;
    while (NULL != node) {
        tmp = node->next;
        free(node);
        node = tmp;
    }
    return 0;
}

int clear(List *list)
{
    if (-1 == is_empty(list))
        return -1;

    Node *node = list->next;
    Node *tmp = NULL;
    while (NULL != node) {
        tmp = node->next;
        free(node);
        node = tmp;
    }
    list->next = NULL;
    return 0;
}

int insert(List *list, int data)
{
    if (-1 == is_empty(list))
        return -1;

    Node *node = (Node *)malloc(sizeof(Node));
    if (NULL == node)
        return -1;

    node->data = data;
    node->next = list->next;
    list->next = node;
    return 0;
}

int delete(List *list, int data)
{
    if (-1 == is_empty(list))
        return -1;
    Node *pre = list;
    Node *cur = list->next;

    while (NULL != cur) {
        if (data == cur->data)
            break;
        pre = cur;
        cur = cur->next;
    }

    pre->next = cur->next;
    free(cur);
    return 0;
}

Node *find(List *list, int data)
{
    if (-1 == is_empty(list))
        return NULL;
    Node *node = list->next;
    while (NULL != node && node->data != data)
        node = node->next;
    return node;
}

Node *reverse(List *list)
{
    Node *new = NULL;
    Node *tmp = NULL;
    Node *p = list;
    while (NULL != p) {
        tmp = p->next;
        p->next = new;
        new = p;
        p = tmp;
    }
    //list = new;
    return new;
}

int is_exits_loop(List *list)
{
    Node *fast = list;
    Node *slow = list;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
            break;
    }
    return !(NULL == fast || NULL == fast->next);
}

Node *find_loop_port(List *list)
{
    if (NULL == list || NULL == list->next || NULL == list->next->next)
        return NULL;
    Node *fast = list;
    Node *slow = list;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
            break;
    }

    if (NULL == fast || NULL == fast->next)
        return NULL;

    slow = list;
    while (slow != fast) {
        slow = slow->next;
        fast = fast->next;
    }
    return slow;
}

Node *getIntersectionNode(Node *headA, Node *headB) {
    if (NULL == headA->next || NULL == headB->next)
        return NULL;
    Node *a = headA->next;
    Node *b = headB->next;
    int lena=0, lenb=0;

    while (a || b) {
        if (a) {
            a = a->next;
            lena++;
        }
        if (b) {
            b = b->next;
            lenb++;
        }
    }
    a = headA->next;
    b = headB->next;
    while (a && b) {
        printf("a=%d b=%d, lena=%d, lenb=%d\n", a->data, b->data, lena, lenb);
        if (lena > lenb) {
            a = a->next;
            lena--;
        } else if (lena < lenb) {
            b = b->next;
            lenb--;
        } else {
            a = a->next;
            b = b->next;
        }

        if (NULL!=a && NULL!=b && a->data == b->data)
            break;
    }
    if (a && b)
        return a;
    else
        return NULL;
}

int main(int argc, char *argv[])
{

    List *l1 = init(); 
    List *l2 = init(); 
    insert(l1, 9);
    insert(l1, 8);
    insert(l1, 3);
    insert(l1, 2);
    insert(l1, 1);

    insert(l2, 9);
    insert(l2, 8);
    insert(l2, 4);

    getIntersectionNode(l1, l2);

    return 0;
}
