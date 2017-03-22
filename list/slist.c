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

void add_from_head(int data)
{
    if (NULL == head) {
        head = create(data);
    } else {
        Node *node = create(data);
        if (NULL == node)
            return;
        node->next = head;
        head = node;
    }
}

void add_from_tail(int data)
{
    if (NULL == head) {
        head = create(data);
    } else {
        Node *node = create(data);
        if (NULL == node)
            return;
        Node *p = head;
        while (p->next)
            p = p->next;
        p->next = node;
    }
}

void delete_from_head(void)
{
    if (NULL == head)
        return;

    Node *p = head;
    head = head->next;
    free(p);
	p = NULL;
}

void delete_from_tail(void)
{
    if (NULL == head)
        return;

    Node *p = head;
    Node *pre = head;
    while (p->next) {
        pre = p;
        p = p->next;
    }
    if (p == head) {
        free(p);
		p = NULL;
		pre = NULL;
        head = NULL;
    } else {
        pre->next = NULL;
        free(p);
		p = NULL;
    }
}

Node *reverse(void)
{
    Node *new = NULL;
    Node *tmp =  NULL;
    Node *p = head;
    while (NULL != p) {
        tmp = p->next;
        p->next = new;
        new = p;
        p = tmp;
    }
    head = new;
    return new;
}

int is_exits_loop(void)
{
    Node *fast = head;
    Node *slow = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
            break;
    }
    return !(NULL == fast || NULL == fast->next);
}

Node *find_loop_port(void)
{

    if (NULL == head || NULL == head->next || NULL == head->next->next)
        return NULL;
    Node *fast = head;
    Node *slow = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
            break;
    }

    if (NULL == fast || NULL == fast->next)
        return NULL;

    slow = head;
    while (slow != fast) {
        slow = slow->next;
        fast = fast->next;
    }
    return slow;
}

void print(void)
{
    Node *p = head;
    while(p) {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

void destory(void)
{
    Node *p = head;
    while (p) {
        Node *tmp = p;
        p = p->next;
        free(tmp);
		tmp = NULL;
    }
    head = NULL;

}

void delete_by_num(int num)
{
	if (NULL == head)
		return;

	Node *cur = head;
	Node *pre = head;
	while (cur) {
		if (num == cur->data)
			break;
		pre = cur;
		cur = cur->next;
	}

	if (NULL = cur)
		return;

	if (head == cur) {
		head = head->next;
        free(cur);
		cur = NULL;
		pre = NULL;
	} else {
	    pre->next = cur->next;
	    free(cur);
		cur = NULL;
	}
}

void add_by_num(int num)
{
	if (NULL == head)
		return;

	Node *cur = head;
	Node *pre = head;
	while (cur) {
		if (num == cur->data)
			break;
		pre = cur;
		cur = cur->next;
	}

	if (NULL = cur) {
		add_from_tail(num);
		return;
	}

	if (head == cur) {
		add_from_tail(num);
	} else {
		Node *node = create(num);
		if (NULL = node)
			return;
		node->next = cur;
		pre->next = node;
	}
}


int main(int argc ,char *argv[])
{
    add_from_tail(1);
    add_from_tail(2);
    add_from_tail(3);
    add_from_tail(4);
    add_from_tail(5);
    add_from_tail(6);
    print();

    delete_from_head();
    delete_from_head();
    delete_from_head();
    print();

    reverse();
    print();

    return 0;
}
