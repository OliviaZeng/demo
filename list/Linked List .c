/*
Linked List
*/
//237. Delete Node in a Linked List 删除链表节点
//删除node，则node被赋值为next，此时node已消失而存在两个next,再删除一个next即可
void deleteNode(struct ListNode* node) {
    struct ListNode *next = node->next;
    *node = *next;
    free(next);
}

//203. Remove Linked List Elements 移除链表元素
//Given: 1 --> 2 --> 6 --> 3 --> 4 --> 5 --> 6, val = 6
//Return: 1 --> 2 --> 3 --> 4 --> 5
struct ListNode* removeElements(struct ListNode* head, int val) {
    struct ListNode node;
    struct ListNode *p = &node;
    struct ListNode *tmp = NULL;
    p->next = head;

    while (head) {
        if (head->val == val) {
            tmp = head;
            p->next = head->next;
            head = head->next;
            free(tmp);
        } else {
            head = head->next;
            p = p->next;
        }
    }
    return node.next;
}

//83. Remove Duplicates from Sorted List 删去有序链表中重复的元素
//Given a sorted linked list, delete all duplicates such that each element appear only once.
//Given 1->1->2, return 1->2.
//Given 1->1->2->3->3, return 1->2->3. 
struct ListNode* deleteDuplicates(struct ListNode* head) {
    struct ListNode *cur = head;
    struct ListNode *tmp = NULL;
    while (cur && cur->next) {
        if (cur->val != cur->next->val) {
            cur = cur->next;
        } else {
            tmp = cur->next;
            cur->next = cur->next->next;
            free(tmp);
        }
    }
    return head;
}

//82. Remove Duplicates from Sorted List II 把出现重复的元素全部删除
//要把前驱指针指向上一个不重复的元素中，如果找到不重复元素，则把前驱指针知道该元素，否则删除此元素。算法只需要一遍扫描，时间复杂度是O(n)，空间只需要几个辅助指针，是O(1)

struct ListNode* deleteDuplicates(struct ListNode* head) {
    if (NULL == head)
        return NULL;
    struct ListNode help;
    help.next = head;
    struct ListNode *pre = &help;
    struct ListNode *cur = head;
    while (cur) {
        while (cur->next && cur->val == cur->next->val)
            cur = cur->next;
        if (pre->next == cur)
            pre = pre->next;
        else
            pre->next = cur->next;
        cur = cur->next;
    }
    return help.next;
}

//206. Reverse Linked List 反转链表
struct ListNode* reverseList(struct ListNode* head) {
    struct ListNode *new = NULL;
    struct ListNode *tmp = NULL;
    struct ListNode *p   = head;
    
    while (p) {
        tmp = p->next;    
        p->next = new;
        new = p;
        p = tmp;
    }
    return new;
}

//141. Linked List Cycle 如何判断一个单链表中有环
//1.空链表不成环;2.一个节点自环;3.一条链表完整成环
//时间 O(N) 空间 O(1)
//附加题：3.如何将有环的链表变成单链表(解除环); 4.如何判断两个单链表是否有交点？如何找到第一个相交的节点
bool hasCycle(struct ListNode *head) {
    struct ListNode *fast = head;
    struct ListNode *slow = head;

    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
        if (fast == slow)
            break;
    }

    return !(NULL == fast || NULL == fast->next);
}

//142. Linked List Cycle II 如何找到环中第一个节点
struct ListNode *detectCycle(struct ListNode *head) {
    struct ListNode *fast = head;
    struct ListNode *slow = head;

    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
        if (fast == slow)
            break;
    }

    if (NULL == fast || NULL == fast->next)
        return NULL;

    fast = head;
    while (fast != slow) {
        fast = fast->next;
        slow = slow->next;
    }
    return fast;
}



//21. Merge Two Sorted Lists 合并两个排序链表
//在头节点前面加一个head节点, 是个struc Node类型而非指针，有点像锚
struct ListNode* mergeTwoLists(struct ListNode* l1, struct ListNode* l2) {
    struct ListNode head;
    struct ListNode *p = &head;
    head.next = NULL;
    
    while (l1 || l2) {
        if (l1 && l2) {
            if (l1->val < l2->val) {
                p = p->next = l1;
                l1 = l1->next;
            } else {
                p = p->next = l2;
                l2 = l2->next;
            }
        } else if (l1) {
            p = p->next = l1;
            l1 = l1->next;
        } else if (l2) {
            p = p->next = l2;
            l2 = l2->next;            
        }
    }
    return head.next;
}
//不采用锚
struct ListNode* mergeTwoLists(struct ListNode* l1, struct ListNode* l2) {
    if(l1 == NULL) return l2;
    if(l2 == NULL) return l1;
    
    struct ListNode *head = NULL;
    if(l1->val > l2->val) {
        head = l2;
        l2 = l2->next;
    } else {
        head = l1;
        l1 = l1->next;
    }
    struct ListNode *cur = head;
    while (l1 != NULL && l2 != NULL) {
        if (l1->val > l2->val) {
            cur->next = l2;
            l2 = l2->next;
        } else {
            cur->next = l1;
            l1 = l1->next;
        }
        cur = cur->next;
    }
    
    if(l1 != NULL)
        cur->next = l1;
    else
        cur->next = l2;
    
    return head;

}
//递归
struct ListNode* mergeTwoLists(struct ListNode* l1, struct ListNode* l2) {
    if (l1 == NULL)
        return l2;
    if (l2 == NULL)
        return l1;
    if (l1->val <= l2->val) {
        l1->next = mergeTwoLists(l1->next, l2);
        return l1;
    } else {
        l2->next = mergeTwoLists(l1, l2->next);
        return l2;
    }
}

//234. Palindrome Linked List 回文链表
//1、遍历整个链表，将链表每个节点的值记录在数组中，再判断数组是不是一个回文数组，时间复杂度为O（n），但空间复杂度也为O（n），不满足空间复杂度要求。
//2、利用栈先进后出的性质，将链表前半段压入栈中，再逐个弹出与链表后半段比较。时间复杂度O（n），但仍然需要n/2的栈空间，空间复杂度为O（n）。
//3、反转链表法，将链表后半段原地翻转，再将前半段、后半段依次比较，判断是否相等，时间复杂度O（n），空间复杂度为O（1）
//O(n) time O(1) space
bool isPalindrome(struct ListNode* head) {
    if (NULL == head || NULL == head->next)
        return true;
    struct ListNode *slow = head;
    struct ListNode *fast = head;

    while (fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }

    slow->next = reverseList(slow->next);
    slow = slow->next;
    
    while (slow) {
        if (head->val != slow->val)
            return false;
        slow = slow->next;
        head = head->next;
    }
    return true;
}

//160. Intersection of Two Linked Lists 求两个单链表（不存在环）相交的第一个节点，若不相交返回null
//Move cur1 (cur2) forward from headA (headB) and loop back to headB (headA), eventually cur1 and cur2 will meet at the intersection point or nullptr
//O(n+m) time O(1) space
struct ListNode *getIntersectionNode(struct ListNode *headA, struct ListNode *headB) {
    if (NULL == headA || NULL == headB)
        return NULL;

    struct ListNode *cur1 = headA, *cur2 = headB;
    while(cur1 != cur2){
        cur1 = cur1?cur1->next:headB;
        cur2 = cur2?cur2->next:headA;
    }
    return cur1;
}
//
//跳过较长的List前面的nodes，从长度相等的地方开始一个一个的比较，直到找到Intersection或者到末尾都没有Intersection
//O(n) time O(1) space
struct ListNode *getIntersectionNode(struct ListNode *headA, struct ListNode *headB) {
    if (NULL == headA || NULL == headB)
        return NULL;
    if (headA == headB)
        return headA;
    struct ListNode *a = headA;
    struct ListNode *b = headB;
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
    a = headA;
    b = headB;
    while (a && b) {
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

        if (NULL!=a && NULL!=b && a == b)
            break;
    }
    if (a && b)
        return a;
    else
        return NULL;   
}


//2. Add Two Numbers 
//利用两个指针分别遍历两个链表，并且用一个变量表示是否有进位。某个链表遍历结束之后再将另一个链表连接在结果链表之后即可，若最后有进位需要添加一位
//时间O(n) 空间(1)
struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    int carry = 0;
    int sum = 0;
    struct ListNode root;
    root.next = NULL;
    struct ListNode *p = &root;
    while (l1 || l2 || carry) {
        sum = 0;
        if (l1) {
            sum += l1->val;
            l1 = l1->next;
        }
        if (l2) {
            sum += l2->val;
            l2 = l2->next;
        }
        sum += carry;

        struct ListNode *node = (struct ListNode *)malloc(sizeof(struct ListNode));
        if (NULL == node)
            return NULL;
        
        carry = sum / 10;
        node->val = sum % 10;
        node->next = NULL;
        p->next = node;
        p = node;
    }

    return root.next; 
}

//445. Add Two Numbers II 
//不允许翻转链表，就利用stack
//时间 O(N) 空间 O(N)
struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    int sum = 0;
    int carry = 0;
    struct ListNode *list = NULL;
    struct ListNode *stack1 = NULL;
    struct ListNode *stack2 = NULL;

    while (l1) {
        struct ListNode *node = (struct ListNode *)malloc(sizeof(struct ListNode));
        if (NULL == node)
            return NULL;

        node->val = l1->val;
        node->next = stack1;
        stack1 = node;
        l1 = l1->next;
    }

    while (l2) {
        struct ListNode *node = (struct ListNode *)malloc(sizeof(struct ListNode));
        if (NULL == node)
            return NULL;

        node->val = l2->val;
        node->next = stack2;
        stack2 = node;
        l2 = l2->next;
    }

    while (NULL != stack1 || NULL != stack2 || carry) {
        sum = 0;
        if (NULL != stack1) {
            sum += stack1->val;
            stack1 = stack1->next;
        }

        if (NULL != stack2) {
            sum += stack2->val;
            stack2 = stack2->next;
        }

        struct ListNode *node = (struct ListNode *)malloc(sizeof(struct ListNode));
        if (NULL == node)
            return NULL;

        node->val = (sum + carry) % 10;
        node->next = list;
        carry = (sum + carry) / 10;
        list = node;
    }
    return list;
}

//328. Odd Even Linked List 链表内元素按奇偶位置重新排序
// 将偶数位置的节点放在所有奇数位置节点的后面,给出链表1->2->3->4->5->NULL，应返回链表1->3->5->2->4->NUL
// 先拆成奇偶两个链表，再讲偶链表头接在奇链表尾
// O(N)time, O(1)space
struct ListNode* oddEvenList(struct ListNode* head) {
    if (NULL != head) {
        struct ListNode *odd  = head;
        struct ListNode *even = head->next;
        struct ListNode *p    = head->next;
    
        while (NULL != odd->next && NULL != even->next) {
            odd->next = even->next;
            odd = odd->next;
            even->next = odd->next;
            even = even->next;
        }
        odd->next = p;
    }
    return head;    
}

//24. Swap Nodes in Pairs 交换序列中的结点
//使用唯一不变的空间，修改列表中的值。即当前节点与下一节点swap，移到下下个节点再重复以上过程。
struct ListNode* swapPairs(struct ListNode* head) {
    struct ListNode *cur = head;
    while (cur && cur->next) {
        cur->val       = cur->val ^ cur->next->val;
        cur->next->val = cur->val ^ cur->next->val;
        cur->val       = cur->val ^ cur->next->val;
        cur = cur->next->next;
    }
    return head;   
}
//使用唯一不变的空间，不能修改列表中的值。
struct ListNode* swapPairs(struct ListNode* head) {
    if(head == NULL || head->next == NULL) return head;
    
    struct ListNode *tmp = head->next;
    head->next = swapPairs(tmp->next);
    tmp->next = head;
    return tmp;
}

//19. Remove Nth Node From End of List 删除倒数第n个元素
//Given linked list: 1->2->3->4->5, and n = 2.
//After removing the second node from the end, the linked list becomes 1->2->3->5.
//fast先走n步，随后slow跟着移动，直到fast=NULL,此时slow是要删除节点。利用Pre是否为空来判断是否要删除head节点
struct ListNode *removeNthFromEnd(struct ListNode *head, int n) {
    if (NULL == head || n < 1)
        return NULL;
    struct ListNode* fast = head;
    struct ListNode* slow = head;
    struct ListNode* pre = NULL;
    struct ListNode node;
    node.next = head;

    while (n > 0) {
        fast = fast->next;
        n -= 1;
    }
    while (fast) {
        fast = fast->next;
        pre = slow;
        slow = slow->next;
    }
    if (NULL == pre) {
        node.next = head->next;
        free(head);
    } else {
        pre->next = slow->next;
        free(slow);
    }
    return node.next;
}

//先把整个list走一遍知道长度，然后再走 用总长度减去所要的点，得到位置就跳过。这样需要走两遍 用一个pointer 时间复杂度为O(2n)
struct ListNode* removeNthFromEnd(struct ListNode* head, int n) {
    if (NULL == head || n < 1)
        return NULL;
    struct ListNode *pre = head;
    struct ListNode *cur = head;
    int len = 0;
    while (cur) {
        ++len;
        cur = cur->next;
    }
    len -= n;

    if (0 == len) {
        head = head->next;
    } else if (0 > len) {
        return NULL;
    } else {
        cur = head;
        while (len--) {
            pre = cur;
            cur = cur->next;
        }
        pre->next = cur->next;
    }
    free(cur);
    return head; 
}

//86. Partition List 分区链表
//给定一个单链表和一个x，把链表中小于x的放到前面，大于等于x的放到后面，每部分元素的原始相对位置不变
//Given 1->4->3->2->5->2 and x = 3,
//return 1->2->2->4->3->5
//将list按X分成两段,小于的连接l1,大于的连接l2,最后合并l1和l2即可
struct ListNode* partition(struct ListNode* head, int x) {
    struct ListNode node1, node2;
    struct ListNode *l1 = &node1;
    struct ListNode *l2 = &node2;
    
    while (head) {
        if (head->val < x)
            l1 = l1->next = head;
        else
            l2 = l2->next = head;
        head = head->next;
    }

    l2->next = NULL;
    l1->next = node2.next;
    return node1.next;
}

//143. Reorder List 链表重排序
//给定一个链表，把最后一个结点插入到第1个结点后，倒数第二个结点插入到第2个结点后，倒数第三个结点插入到第3个结点后，以此类推
// Given {1,2,3,4,5,6}, reorder it to {1,6,2,5,4,3}
//先用快慢指针找到链表的中点，然后翻转链表后半部分，再和前半部分组合。需要注意的是把链表分成两半时，前半段的尾节点要置为NULL，翻转链表时也要把尾节点置为NULL
//O(n) time, O(1) in place
void reorderList(struct ListNode* head) {
    if (NULL == head || NULL == head->next)
        return head;

    struct ListNode *slow=head, *fast=head->next;
    struct ListNode *tmp = NULL;
    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }

    struct ListNode *head2 = slow->next;
    slow->next = NULL;

    fast = head2->next;
    head2->next = NULL;

    while (fast) {
        slow = fast->next;
        fast->next = head2;
        head2 = fast;
        fast = slow;
    }

    for (slow = head, fast = head2;slow;) {
        tmp = slow->next;
        slow->next = fast;
        slow = slow->next;
        fast = tmp;
    }    
}

//61. Rotate List 链表旋转k个元素
//倒着数k个node，从那开始到结尾和之前那部分对调
//Given 1->2->3->4->5->NULL and k = 2,
//return 4->5->1->2->3->NULL
//计算出链表元素个数，然后利用k mod 元素个数，求出实际需要的右移动次数。最后通过快慢指针来实现右移



struct ListNode* rotateRight(struct ListNode* head, int k) {
    if (NULL == head || k == 0)
        return head;

    struct ListNode *cur = head;
    int n = 1;
    int i;
    while (cur->next) {
        cur = cur->next;
        n++;
    }

    k = k%n;
    if (0 == k)
        return head;
    else
        k = n - k;
    cur->next = head;
    struct ListNode *new = head;

    for (i=0; i<k-1; i++)
        new = new->next;
    head = new->next;
    new->next = NULL;
    return head;
    
}

//25. Reverse Nodes in k-Group
//对链表中的元素按K个一组，反序后，形成新的链表。对于最后不满K个元素，保持原来的顺序输出
//Given this linked list: 1->2->3->4->5
//For k = 2, you should return: 2->1->4->3->5
//For k = 3, you should return: 3->2->1->4->5 
struct ListNode* reverseKGroup(struct ListNode* head, int k) {
    
    struct ListNode *current, *previous, *scout;
    int count;
    
    scout = current = head;
    previous = NULL;
    count = 0;

    while (scout) {
        
        ++count;

        if (k > 1 && count == k) {
            
            scout = scout->next;
            struct ListNode *first, *following;
            first = current;
            
            while (count-- > 1) {
                following = current->next;
                current->next = following->next;
                following->next = first;
                first = following;
            }
            
            if (previous)
                previous->next = first;
            else
                head = first;
                
            previous = current;
            current = scout;
            count = 0;
        } else {
            scout = scout->next;
        }
    }
    
    return head;
}

struct ListNode* reverseKGroup(struct ListNode* head, int k) {
    struct ListNode *new, *tmp, *p, *step, *newhead;
    new = tmp = NULL;
    newhead = p = step = head;
    int count = 0;

    while (step) {
        ++count;
        if (k > 1 && count == k) {
            while (count--) {
                tmp = p->next;
                p->next = new;
                new = p;
                p = tmp;
            }
            p = step;

        
        } else {
            step = step->next;
        }
    }
    return  newhead;
}

struct ListNode* reverse(struct ListNode *first, struct ListNode *last)
{
    struct ListNode *pre = last;
    struct ListNode *tmp = NULL;

    while (first != last) {
       tmp = first->next; 
       first->next = pre;
       pre = first;
       first = tmp;
    }
    return pre;
}
struct ListNode* reverseKGroup(struct ListNode* head, int k) {
    int i = 0;
    struct ListNode *new = NULL;
    struct ListNode *node = head;
    for (i=0; i<k; i++) {
        if (!node)
            return head;
        node = node->next;
    }

    new = reverse(head, node);
    head->next = reverseKGroup(node, k);
    return new;
}

//109. Convert Sorted List to Binary Search Tree 把一个有序链表转换成一棵二分查找树
//一种自底向上的方法，算法复杂度为O(N)。先递归构建左子树，在构建左子树的同时不断移动链表的头指针，链表的头指针永远是对应当前子树位置的。一直到左叶子节点，左叶子节点对应的就是链表的第一个元素，生成左叶子节点之后移动链表当前指针。
struct ListNode * current;
struct TreeNode * build_tree(int low,int up)
{
    if(low >= up ) return NULL;
    struct TreeNode * tmp = malloc(sizeof(struct TreeNode));
    int mid = low + ((up - low)>>1);
    tmp -> left = build_tree(low,mid);
    tmp -> val = current -> val;
    current = current -> next;
    tmp -> right = build_tree(mid+1,up);
    return tmp;
}

struct TreeNode* sortedListToBST(struct ListNode* head) {
     current = head;
    int len = 0;
    while(head) 
        len++, head = head->next;
    return build_tree(0,len);
}

//147. Insertion Sort List 用插入排序对一个链表进行排序
//新建一个头节点，遍历原来的链表，对原链表的每个节点找到新链表中适合插入位置的前指针，然后执行插入操作
struct ListNode* insertionSortList(struct ListNode* head) {
    struct ListNode result;
    result.next = NULL;
    struct ListNode *cur=head, *next=NULL, *pre=&result;
    while (cur) {
        next = cur->next;
        cur->next = NULL;
        if (!pre || !pre->next || pre->next->val>=cur->val)
            pre = &result;
        while (pre->next && pre->next->val < cur->val)
            pre = pre->next;
        cur->next = pre->next;
        pre->next = cur;
        cur = next;
    }
    return result.next;
}

//148. Sort List 空间复杂度为常数，时间复杂度为O(nlogn)的排序链表实现
// 归并排序法：递归地将当前链表分为两段，然后merge，分两段的方法是使用 fast-slow 法，用两个指针，一个每次走两步，一个走一步，知道快的走到了末尾，然后慢的所在位置就是中间位置，这样就分成了两段。merge时，把两段头部节点值比较，用一个 p 指向较小的，且记录第一个节点，然后 两段的头一步一步向后走，p也一直向后走，总是指向较小节点，直至其中一个头为NULL，处理剩下的元素。最后返回记录的头即可
struct ListNode *merge(struct ListNode *l1, struct ListNode *l2)
{
    struct ListNode dummyHead = {0}, *prev = &dummyHead;
    while (l1 && l2) {
        if (l1->val < l2->val) {
            prev->next = l1;
            l1 = l1->next;
        } else {
            prev->next = l2;
            l2 = l2->next;
        }
        prev = prev->next;
    }
    if (l1) prev->next = l1; else prev->next = l2;
    return dummyHead.next;
}

struct ListNode* sortList(struct ListNode* head) {
    if (NULL == head || NULL == head->next)
        return head;
    // step 1. cut the list to two halves
    struct ListNode *pre=NULL, *slow=head, *fast=head;
    while (NULL != fast && NULL != fast->next) {
        pre = slow;
        slow = slow->next;
        fast = fast->next->next;
    }
    pre->next = NULL;

    // step 2. sort each half
    struct ListNode *l1 = sortList(head);
    struct ListNode *l2 = sortList(slow);

    // step 3. merge l1 and l2
    return merge(l1, l2);
}


//138. Copy List with Random Pointer 
//深拷贝一个链表，链表除了含有next指针外，还包含一个random指针，该指针指向字符串中的某个节点或者为空
//构建新节点时，指针做如下变化，即把新节点插入到相应的旧节点后面
struct RandomListNode *copyRandomList(struct RandomListNode *head) {
	if(NULL==head) return head;

	struct RandomListNode *p=head;
	struct RandomListNode *pt;

	//copy
	while(NULL!=p){
		pt=p->next;
		p->next=malloc(sizeof(struct RandomListNode));
		p->next->label=p->label;
		p->next->next=pt;
		p->next->random=NULL; 
		p=pt;
	};

	//fix random pointer
	p=head;
	while(NULL!=p){
		if(NULL!=p->random){
			p->next->random=p->random->next;
		}
		p=p->next->next;
	};

	//separate
	struct RandomListNode *copyed_head=NULL;
	p=head;
	while(NULL!=p){
		if(NULL!=copyed_head){
			pt->next=p->next;
			pt=pt->next;
		}else{
			copyed_head=p->next;
			pt=copyed_head;
		}
		p->next=p->next->next;
		p=p->next;
	};

	return copyed_head;

}

//23. Merge k Sorted Lists 合并K个已排序的链表，并分析整个算法的复杂度
//分布式常见题型
//In this problem, input is linked lists; divide an conquer works good.
//But if we need to sort arrays or vectors, divide and conquer will spend too much extra space.
//So I think, maybe heap (or loser tree) is a better method.(when K is big enough)
//The time complexity is O(k) + O(nlgk). (divide and conquer is exactly n*lg(k), more comparison times)
//
//timeO(k) + O(nlgk)
void MIN_HEAP_SORT(struct ListNode **lists, int index_i,int size)
{
	int left = index_i*2 + 1;
	int right= index_i*2 + 2;
	if(left>=size)
		return;
	int min;
	if(right>=size)
		min = left;
	else
		min = lists[left]->val<lists[right]->val?left:right;
	if(lists[index_i]->val>lists[min]->val){
		struct ListNode *temp = lists[index_i];
		lists[index_i] = lists[min];
		lists[min] = temp;
		MIN_HEAP_SORT(lists,min,size);
	}
}

void BuildHeap(struct ListNode **lists,int size)
{
	for(int i=(size-1)/2;i>=0;--i){
		MIN_HEAP_SORT(lists,i,size);
	}
}

struct ListNode *mergeKLists(struct ListNode *lists[], int k) {
	if(k==0)
		return NULL;//1
	struct ListNode *head = (struct ListNode*)malloc(sizeof(struct ListNode));
	struct ListNode *int_max = (struct ListNode*)malloc(sizeof(struct ListNode));
	int_max->val = INT_MAX;
	int_max->next = NULL;
	struct ListNode *travel = head;
	for(int i=0;i<k;++i){
		if(lists[i]==NULL)
			lists[i] = int_max;
	}/*remove those NULL ptr*/
	BuildHeap(lists,k);
	while(lists[0]!=int_max){
		travel->next = lists[0];
		travel = lists[0];
		lists[0] = lists[0]->next;
		if(lists[0]==NULL)
			lists[0] = int_max;
		MIN_HEAP_SORT(lists,0,k);
	}
	travel->next = NULL;
	return head->next;
}

