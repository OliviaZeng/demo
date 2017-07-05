#include <stdio.h>
#include <stdlib.h>

#define max(x, y) ((x)>(y)?(x):(y))
typedef struct node{
	int data;
	struct node *left;
	struct node *right;
}Node, Tree;

Tree *destroy(Tree *tree)
{
    if (NULL != tree) {
        destroy(tree->left);
        destroy(tree->right);
        free(tree);
    }
    return NULL;
}

Tree *insert(Tree *tree, int data)
{
    if (NULL == tree) {
        tree = (Tree *)malloc(sizeof(Tree));
        if (NULL == tree)
            return NULL;
        tree->data = data;
        tree->left = tree->right = NULL;
    } else if (data < tree->data) {
        tree->left = insert(tree->left, data);
    } else if (data > tree->data) {
        tree->right = insert(tree->right, data);
    }

    return tree;
}

Node *find_min_r(Tree *tree)
{
    if (NULL == tree)
        return NULL;
    if (NULL == tree->left)
        return tree;
    else
        return find_min_r(tree->left);
}

Node *find_min(Tree *tree)
{
    if (NULL == tree)
        return NULL;

    while (NULL != tree->left)
        tree = tree->left;
    return tree;
}

Node *find_max_r(Tree *tree)
{
    if (NULL == tree)
        return NULL;
    if (NULL == tree->right)
        return tree;
    else
        return find_max_r(tree->right);
}

Node *find_max(Tree *tree)
{
    if (NULL == tree)
        return NULL;

    while (NULL != tree->right)
        tree = tree->right;

    return tree;
}

Tree *delete(Tree *tree, int data)
{
    Node *node = NULL;
    if (NULL == tree)
        return NULL;

    if (data < tree->data) {
        tree->left = delete(tree->left, data);
    } else if (data > tree->data) {
        tree->right = delete(tree->right, data);
    } else if (tree->left && tree->right) {
        node = find_min(tree->right);
        tree->data = node->data;
        tree->right = delete(tree->right, tree->data);
    } else {
        node = tree;
        if (NULL == tree->left)
            tree = tree->right;
        else if (NULL == tree->right)
            tree = tree->left;
        free(node);
    }
    return tree;
}

Node *find(Tree *tree, int data)
{
    if (NULL == tree)
        return NULL;

    if (data < tree->data)
        return find(tree->left, data);
    else if (data > tree->data)
        return find(tree->right, data);
    else
        return tree;
}

/*
1. 求二叉树中的节点个数
递归解法：
（1）如果二叉树为空，节点个数为0
（2）如果二叉树不为空，二叉树节点个数 = 左子树节点个数 + 右子树节点个数 + 1
*/ 
int get_node_num(Tree *tree)
{
    if (NULL == tree)
        return 0;

    return get_node_num(tree->left) + get_node_num(tree->right) + 1;
}

/*
2. 求二叉树的深度
递归解法：
（1）如果二叉树为空，二叉树的深度为0
（2）如果二叉树不为空，二叉树的深度 = max(左子树深度， 右子树深度) + 1
*/
int get_depth(Tree *tree)
{
    if (NULL == tree)
        return 0;

    int depth_left = get_depth(tree->left);
    int depth_right = get_depth(tree->right);
    return depth_left > depth_right ? (depth_left+1) : (depth_right+1);
}

/*
3.前序遍历
前序遍历递归解法：
（1）如果二叉树为空，空操作
（2）如果二叉树不为空，访问根节点，前序遍历左子树，前序遍历右子树
*/ 
void pre_order_r(Tree *tree)
{
    if (NULL == tree)
        return;

    printf("%d ", tree->data);
    pre_order_r(tree->left);
    pre_order_r(tree->right);
}

/*
3.中序遍历
中序遍历递归解法
（1）如果二叉树为空，空操作。
（2）如果二叉树不为空，中序遍历左子树，访问根节点，中序遍历右子树
*/
void in_order_r(Tree *tree)
{
    if (NULL == tree)
        return ;

    in_order_r(tree->left);
    printf("%d ", tree->data);
    in_order_r(tree->right);
}

/*
3.后序遍历
后序遍历递归解法
（1）如果二叉树为空，空操作
（2）如果二叉树不为空，后序遍历左子树，后序遍历右子树，访问根节点
*/
void post_order_r(Tree *tree)
{
    if (NULL == tree)
        return ;
    post_order_r(tree->left);
    post_order_r(tree->right);
    printf("%d ", tree->data);
}

void pre_order(Tree *tree)
{
    Node *stack[30];   
    int num = 0;

    while (NULL != tree || num > 0) {
        while (NULL != tree) {
            printf("%d ", tree->data);
            stack[num++] = tree;
            tree =  tree->left;
        }
        num--;
        tree = stack[num];
        tree = tree->right;
    }
}

void in_order(Tree *tree)
{
    Node *stack[30];
    int num = 0;
    while (NULL != tree || num > 0) {
        while (NULL != tree) {
            stack[num++] = tree;
            tree = tree->left;
        }
        num--;
        tree = stack[num];
        printf("%d ", tree->data);
        tree = tree->right;
    }
}

void post_order(Tree *tree)
{
    Node *stack[30];
    int num = 0;
    Node *has_visited = NULL;
    while (NULL != tree || num > 0) {
        while (NULL != tree) {
            stack[num++] = tree;
            tree = tree->left;
        }
        tree = stack[num-1];
        if (NULL == tree->right || has_visited == tree->right) {
            printf("%d ", tree->data);
            num--;
            has_visited = tree;
            tree = NULL;
        } else {
            tree = tree->right;
        }
    }
}

/*
4.分层遍历二叉树（按层次从上往下，从左往右）
相当于广度优先搜索，使用队列实现。队列初始化，将根节点压入队列。当队列不为空，进行如下操作：弹出一个节点，访问，若左子节点或右子节点不为空，将其压入队列。
*/
void level_traverse(Tree *tree)
{
#if 0
    if (NULL == tree)
        return;

    Queue q;
    q.push(tree);
    while (!q.is_empty()) {
        Tree *node = q.front();
        q.pop();
        if (NULL != node->left)
            q.push(node->left);
        if (NULL != node->right)
            q.push(node->right);
    }
    return ;
#endif
}

/*
6. 求二叉树第K层的节点个数
递归解法：
（1）如果二叉树为空或者k<1返回0
（2）如果二叉树不为空并且k==1，返回1
（3）如果二叉树不为空且k>1，返回左子树中k-1层的节点个数与右子树k-1层节点个数之和
*/
int get_node_num_Kth_level(Tree *tree, int k)
{
    if (NULL == tree || k < 1)
        return 0;

    if (1 == k)
        return 1;

    int numleft = get_node_num_Kth_level(tree, k-1);
    int numright = get_node_num_Kth_level(tree, k-1);

    return numleft + numright;
}
/*
7. 求二叉树中叶子节点的个数
递归解法：
（1）如果二叉树为空，返回0
（2）如果二叉树不为空且左右子树为空，返回1
（3）如果二叉树不为空，且左右子树不同时为空，返回左子树中叶子节点个数加上右子树中叶子节点个数
*/
int get_leaf_node_num(Tree *tree)
{
    if (NULL == tree)
        return 0;
    if (NULL == tree->left && NULL == tree->right)
        return 1;
    int numleft = get_leaf_node_num(tree->left);
    int numright = get_leaf_node_num(tree->right);
    return numleft + numright;
}
/*
8. 判断两棵二叉树是否结构相同
不考虑数据内容。结构相同意味着对应的左子树和对应的右子树都结构相同。
递归解法：
（1）如果两棵二叉树都为空，返回真
（2）如果两棵二叉树一棵为空，另一棵不为空，返回假
（3）如果两棵二叉树都不为空，如果对应的左子树和右子树都同构返回真，其他返回假
*/
int is_same_tree(Tree *tree1, Tree *tree2)
{
    if (NULL == tree1 && NULL == tree2)
        return 1;
    if (NULL == tree1 || NULL == tree2)
        return 0;

    int resultleft = is_same_tree(tree1->left, tree2->left);
    int resultright = is_same_tree(tree1->right, tree2->right);
    return (resultleft && resultright);
}

/*
9. 判断二叉树是不是平衡二叉树
递归解法：
（1）如果二叉树为空，返回真
（2）如果二叉树不为空，如果左子树和右子树都是AVL树并且左子树和右子树高度相差不大于1，返回真，其他返回假
*/
int is_avl(Tree *tree, int *height)
{
    if (NULL == tree) {
        *height = 0;
        return 1;
    }

    int heightleft;
    int resultleft = is_avl(tree, &heightleft);
    int heightright;
    int resultright = is_avl(tree, &heightright);

    if (resultleft && resultright && abs(heightleft-heightright) <= 1) {
        *height = (heightleft > heightright ? heightleft : heightright) + 1;
        return 1;
    } else {
        *height = (heightleft > heightright ? heightleft : heightright) + 1;
        return 0;
    }
}
/*
10. 求二叉树的镜像
递归解法：
（1）如果二叉树为空，返回空
（2）如果二叉树不为空，求左子树和右子树的镜像，然后交换左子树和右子树
*/
Tree *mirror(Tree *tree)
{
    if (NULL == tree)
        return NULL;

    Tree *left = mirror(tree->left);
    Tree *right = mirror(tree->right);

    tree->left = right;
    tree->right = left;
    return tree;
}

/*
 * 11. 求二叉树中两个节点的最低公共祖先节点
递归解法：
（1）如果两个节点分别在根节点的左子树和右子树，则返回根节点
（2）如果两个节点都在左子树，则递归处理左子树；如果两个节点都在右子树，则递归处理右子树
*/

int find_node(Tree *tree, Tree *node)
{
    if (NULL == tree || NULL == node)
        return 0;

    if (tree == node)
        return 1;

    int found = find_node(tree->left, node);
    if (!found)
        found = find_node(tree->right, node);
    return found;
}

Tree *get_last_common_parent(Tree *tree, Tree *node1, Tree *node2)
{
    //if NULL == tree怎么办？？
    if (find_node(tree->left, node1)) {
        if (find_node(tree->right, node2))
            return tree;
        else
            return get_last_common_parent(tree->left, node1,node2);

    } else {
        if (find_node(tree->left, node2))
            return tree;
        else
            return get_last_common_parent(tree->right, node1,node2);
    }
}

/*
12.求二叉树中节点的最大距离
即二叉树中相距最远的两个节点之间的距离。
递归解法：
（1）如果二叉树为空，返回0，同时记录左子树和右子树的深度，都为0
（2）如果二叉树不为空，最大距离要么是左子树中的最大距离，要么是右子树中的最大距离，要么是左子树节点中到根节点的最大距离+右子树节点中到根节点的最大距离，同时记录左子树和右子树节点中到根节点的最大距离
*/
int get_max_distance(Tree *tree, int *maxleft, int *maxright)
{
    // maxLeft, 左子树中的节点距离根节点的最远距离  
    // maxRight, 右子树中的节点距离根节点的最远距离  
    if(NULL == tree)  
    {  
        *maxleft = 0;  
        *maxright = 0;  
        return 0;  
    }  
    int maxLL, maxLR, maxRL, maxRR;  
    int maxDistLeft, maxDistRight;  
    if(tree->left != NULL)  
    {  
        maxDistLeft = get_max_distance(tree->left, &maxLL, &maxLR);  
        *maxleft = max(maxLL, maxLR) + 1;  
    }  
    else  
    {  
        maxDistLeft = 0;  
        *maxleft = 0;  
    }  
    if(tree->right != NULL)  
    {  
        maxDistRight = get_max_distance(tree->right, &maxRL, &maxRR);  
        *maxright = max(maxRL, maxRR) + 1;  
    }  
    else  
    {  
        maxDistRight = 0;  
        *maxright = 0;  
    }  
    return max(max(maxDistLeft, maxDistRight), *maxleft+*maxright);  

}

/*13.由前序遍历序列和中序遍历序列重建二叉树
二叉树前序遍历序列中，第一个元素总是树的根节点的值。中序遍历序列中，左子树的节点的值位于根节点的值的左边，右子树的节点的值位
于根节点的值的右边。
递归解法：
（1）如果前序遍历为空或中序遍历为空或节点个数小于等于0，返回NULL。
（2）创建根节点。前序遍历的第一个数据就是根节点的数据，在中序遍历中找到根节点的位置，可分别得知左子树和右子树的前序和中序遍
*/
Tree * rebuild_binary_tree(int *pre, int *in, int nodenum)
{
    if (NULL == pre || NULL == in || nodenum < 0)
        return NULL;
    Tree *node = (Tree *)malloc(sizeof(Tree));
    // 前序遍历的第一个数据就是根节点数据 
    node->data = pre[0];
    node->left = NULL;
    node->right = NULL;
    // 查找根节点在中序遍历中的位置，中序遍历中，根节点左边为左子树，右边为右子树
    int rootPositionInOrder = -1;
    int i;
    for (i=0; i<nodenum; i++)
        if (in[i] == node->data) {
            rootPositionInOrder = i;
            break;
        }
    if (-1 == rootPositionInOrder) {
        printf("Invalid input.\n");
        exit(1);
    }

    // 重建左子树
    int nodeNumLeft = rootPositionInOrder;
    int * preLeft = pre + 1;
    int * inLeft = in;
    node->left = rebuild_binary_tree(preLeft, inLeft, nodeNumLeft);

    // 重建右子树
    int nodeNumRight = nodenum -nodeNumLeft -1;
    int *preRight = pre + 1 + nodeNumLeft;
    int *inRight  = in  + 1 + nodeNumLeft;
    node->right = rebuild_binary_tree(preRight, inRight, nodeNumRight);
    return node;
}



/*14.判断二叉树是不是完全二叉树
若设二叉树的深度为h，除第 h 层外，其它各层 (1～h-1) 的结点数都达到最大个数，第 h 层所有的结点都连续集中在最左边，这就是完全
二叉树。
有如下算法，按层次（从上到下，从左到右）遍历二叉树，当遇到一个节点的左子树为空时，则该节点右子树必须为空，且后面遍历的节点左
右子树都必须为空，否则不是完全二叉树。
*/
int is_complete_binarytree(Tree *tree)
{
#if 0
    if (NULL == tree)
        return 0;
    Queue q;
    q.push(tree);
    int must_have_no_child = 0;
    int result = 1;
    while (!q.is_empty()) {
        Tree *node = q->front;
        q.pop();
        if (must_have_no_child) { // 已经出现了有空子树的节点了，后面出现的必须为叶节点（左右子树都为空）
            if (NULL != tree->left || NULL != tree->right) {
                result = 0;
                break;
            }
        } else {
            if (NULL != tree->left && NULL != tree->right) {
                q.push(tree->left);
                q.push(tree->right);
            } else if (NULL != tree->left && NULL == tree->right) {
                must_have_no_child = 1;
                q.push(tree->left);
            } else if(NULL == tree->left && NULL != tree->right) {
                result = 0;
                break;
            } else {
                must_have_no_child = 1;
            }
        }
    }
    return result;
#endif
}

int main(int argc, char *argv[])
{
    Tree *tree = NULL;
    int i = 0, j = 0;;
    int data[] = {5,2,4,3,6,8,1,9,7};
    for (i=0; i<9; i++)
        tree = insert(tree, data[i]);

    printf("pre order\n");
    pre_order(tree);
    printf("\n");
    pre_order_r(tree);
    printf("\n");

    printf("in order\n");
    in_order(tree);
    printf("\n");
    in_order_r(tree);
    printf("\n");

    printf("post order\n");
    post_order(tree);
    printf("\n");
    post_order_r(tree);
    printf("\n");

    for (i=0; i<9; i+=2)
        tree = delete(tree, data[i]);

    printf("min=%d, max=%d, min_r=%d, max_r=%d\n", find_min(tree)->data, find_max(tree)->data, find_min_r(tree)->data, find_max_r(tree)->data);
}

//94. Binary Tree Inorder Traversal
//中序遍历
//recursive solution
void traverse(struct TreeNode* root, int** arr, int* returnSize)
{
    if(!root) return ;
    if(root->left)
        traverse(root->left, arr, returnSize);
    *returnSize += 1;
    *arr = (int*)realloc(*arr, sizeof(int)*(*returnSize));
    (*arr)[*returnSize-1] = root->val;
    if(root->right)
        traverse(root->right, arr, returnSize);
}
//AC - 0ms;
int* inorderTraversal(struct TreeNode* root, int* returnSize)
{
    if(!root) return NULL;
    *returnSize = 0;
    int* arr = (int*)malloc(sizeof(int));
    traverse(root, &arr, returnSize);
    return arr;
}

//stack solution
void collectLeftNodes(struct TreeNode* root, struct TreeNode*** stack, int* size)
{
    while(root)
    {
        *stack = (struct TreeNode**)realloc(*stack, sizeof(struct TreeNode*)*(*size+1));
        (*stack)[(*size)++] = root;
        root = root->left;
    }
}
//AC - 0ms;
int* inorderTraversal(struct TreeNode* root, int* returnSize)
{
    if(!root) return NULL;
    int* arr = (int*)malloc(sizeof(int));
    *returnSize = 0;
    struct TreeNode** stack = (struct TreeNode**)malloc(sizeof(struct TreeNode*));
    int size = 0; //the size of the stack index of top=size-1;
    collectLeftNodes(root, &stack, &size);
    while(size)
    {
        root = stack[size-1];
        size--;
        *returnSize += 1;
        arr = (int*)realloc(arr, sizeof(int)*(*returnSize));
        arr[*returnSize-1] = root->val; //collecting the leftmost;
        root = root->right; //preparing to collect the right children of the leftmost node;
        collectLeftNodes(root, &stack, &size);
    }
    return arr;
}

//AC - 0ms;
//non-recursion and non-stack;
int* inorderTraversal(struct TreeNode* root, int* returnSize)
{
    if(!root) return NULL;
    int* arr = (int*)malloc(sizeof(int));
    *returnSize = 0;
    while(root)
    {
        if(!root->left) 
        {
            *returnSize += 1;
            arr = (int*)realloc(arr, sizeof(int)*(*returnSize));
            arr[*returnSize-1] = root->val;
            root = root->right;
        }
        else
        {
            struct TreeNode *pre = root->left; //check whether the current has been traversed;
            while(pre->right && pre->right != root)
                pre = pre->right;
            if(!pre->right)//not traversed before, connect the current to the rightmost of the left child for later traversal;
            {
                pre->right = root;
                root = root->left;
            }
            else //traversed before and now it's time to record its value;
            {
                *returnSize += 1;
                arr = (int*)realloc(arr, sizeof(int)*(*returnSize));
                arr[*returnSize-1] = root->val;
                pre->right = NULL; //to reduce complexity of the connection;
                root = root->right;
            }
        }
    }
    return arr;
}

// 96. Unique Binary Search Trees 
//要求可行的二叉查找树的数量，其实二叉查找树可以任意取根，只要满足中序遍历有序的要求就可以,。从处理子问题的角度来看，选取一个结点为根，就把结点 切成左右子树，以这个结点为根的可行二叉树数量就是左右子树可行二叉树数量的乘积，所以总的数量是将以所有结点为根的可行结果累加起来
//公式：C0=1,Cn+1=Ci*Cn-i,n>=0
//熟悉卡特兰数的朋友可能已经发现了，这正是卡特兰数的一种定义方式，是一个典型的动态规划的定义方式（根据其实条件和递推式求解结果）。所以思路也很明确了，维护量res[i]表示含有i个结点的二叉查找树的数量。根据上述递推式依次求出1到n的的结果即可
//动态规划DP
int numTrees(int n) {
    if(n==0)
        return 0;
    int R[n+1];
    R[0] = 0;
    R[1] = 1;
    int i,j;
    for(i = 2; i <= n; i++)
    {
    	R[i]=2*R[i-1];
    	for(j = 2; j < i; j++)
            R[i]+=R[j-1]*R[i-j];
    }
    return R[n];    
}

//95. Unique Binary Search Trees II 
//要求可行的二叉查找树
//1. 每一次都在一个范围内随机选取一个结点作为根。
//2. 每选取一个结点作为根，就把树切分成左右两个子树，直至该结点左右子树为空
//
//但具体对于本题来说，采取的是自底向上的求解过程。
//1. 选出根结点后应该先分别求解该根的左右子树集合，也就是根的左子树有若干种，它们组成左子树集合，根的右子树有若干种，它们组成右子树集合。
//2. 然后将左右子树相互配对，每一个左子树都与所有右子树匹配，每一个右子树都与所有的左子树匹配。然后将两个子树插在根结点上。
//3. 最后，把根结点放入链表中。
//
struct TreeNode* push(struct TreeNode*** arr, int* returnSize, int a)
{
    struct TreeNode *t = NULL;
    if(a > -1) //only valid value can be allocated;
    {
        t = (struct TreeNode*)malloc(sizeof(struct TreeNode));
        t->left = t->right = NULL;
        t->val = a;
    }
    *returnSize += 1;
    *arr = (struct TreeNode**)realloc(*arr, sizeof(struct TreeNode*)*(*returnSize));
    (*arr)[*returnSize-1] = t;
    return t; //return this node for -> root;
}
struct TreeNode* generate(int begin, int end, int* returnSize)
{
    struct TreeNode** arr = (struct TreeNode**)malloc(sizeof(struct TreeNode*));
    if(begin >= end)
    {
        if(begin > end) 
            push(&arr, returnSize, -1);
        if(begin == end)
            push(&arr, returnSize, begin);
        return arr;
    }
    for(int i = begin; i <= end; i++) //try to set each value as root;
    {
        int count0=0, count1=0;
        struct TreeNode **arr0 = generate(begin, i-1, &count0);
        struct TreeNode **arr1 = generate(i+1, end, &count1);
        for(int j = 0; j < count0; j++) //try to use different node as left and right child;
            for(int k = 0; k < count1; k++)
            {
                struct TreeNode* t = push(&arr, returnSize, i);
                t->left = arr0[j];
                t->right = arr1[k];
            }
    }
    return arr;
}

//AC - 8ms;
struct TreeNode** generateTrees(int n, int* returnSize)
{
    *returnSize = 0;
    if(!n) return NULL;
    return generate(1, n, returnSize);
}

//98. Validate Binary Search Tree
//判断一个二叉树是否为二分查找树
bool isValidBST(struct TreeNode* root) {

    if (NULL == root)
        return true;

    if (NULL != root->left)
    {
        struct TreeNode* current = root->left;
        while (current->right != NULL)
        {
            current = current->right;
        }
        if (current->val >= root->val)
        {
            return false;
        }

    }

    if (NULL != root->right)
    {
        struct TreeNode* current = root->right;
        while (NULL != current->left)
        {
            current = current->left;
        }
        if ( current->val <= root->val)
        {
            return false;
        }
    }

    if ( !isValidBST(root->left))
    {
        return false;
    }

    if (!isValidBST(root->right))
    {
        return false;
    }

    return true;
}

//99. Recover Binary Search Tree 修正二叉查找树错误节点，不改变结构
//二叉查找树不合法，有两个节点的值被交换了，找出这两个节点并且不改变树的结构，使得二叉查找树合法，常数空间限制
void traverse(struct TreeNode* root, struct TreeNode** pre, struct TreeNode** first, struct TreeNode** second)
{
    if(!root) return ;
    traverse(root->left, pre, first, second);
    if((*pre) && (*pre)->val>root->val) //check whether the previous node is abnormal in in-order traversal;
    {
        if(!(*first)) //if *first is never used then it should be used to store the bigger; 
            *first=*pre; 
        *second = root; //the two swapped nodes can be adjacent and distant;
    }
    *pre = root; //store the previous node in in-order traversal;
    traverse(root->right, pre, first, second);
}
//AC - 20ms;
void recoverTree(struct TreeNode* root)
{
    struct TreeNode *first=NULL, *second=NULL, *pre=NULL;
    traverse(root, &pre, &first, &second);
    if(first&&second)
    {
        int t = first->val;
        first->val = second->val;
        second->val = t;
    }
}


//100. Same Tree
//考虑每个节点带的值
bool isSameTree(struct TreeNode* p, struct TreeNode* q) {
    // both of leaf node;
    if (p == NULL && q == NULL)
    {
        return true;
    }

    // different structure or value
    if ( (p == NULL && q != NULL)
    || (p != NULL && q == NULL)
    || (p != NULL && q != NULL && p->val != q->val) )
    {
        return false;        
    }
    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}

//101. Symmetric Tree
//判断一个二叉树是否是轴对称的
bool helper(struct TreeNode* Lnode ,struct TreeNode* Rnode ){

    if(Lnode==NULL && Rnode==NULL)
        return true;
    
    if(Lnode==NULL || Rnode==NULL)
        return false ;
    
    return ((Lnode->val==Rnode->val) && helper(Lnode->left,Rnode->right) && helper(Lnode->right,Rnode->left )) ;
}

bool isSymmetric(struct TreeNode* root) {

    if(root==NULL)
        return true;
    
   return helper(root->left ,root->right); 

}

//102. Binary Tree Level Order Traversal
void traverse(struct TreeNode *root, int depth, int ***arr, int **columnSizes, int *returnSize)
{
    if (!root) return;

    if (*returnSize < depth + 1) {
        *returnSize = depth + 1;
        
        /*
         * Should initialise the one more allocated space to NULL (or 0)
         */
        *arr = realloc(*arr, (depth + 1) * sizeof(int *));
        (*arr)[depth] = NULL;
    
        *columnSizes = realloc(*columnSizes, (depth + 1) * sizeof(int));
        (*columnSizes)[depth] = 0;
    }
    
    (*arr)[depth] = realloc((*arr)[depth], ((*columnSizes)[depth] + 1) * sizeof(int));
    (*arr)[depth][(*columnSizes)[depth]] = root->val;
    ++(*columnSizes)[depth];
    
    traverse(root->left, depth + 1, arr, columnSizes, returnSize);
    traverse(root->right, depth + 1, arr, columnSizes, returnSize);
}

int** levelOrder(struct TreeNode* root, int** columnSizes, int* returnSize) {
    int **arr;
    
    arr = NULL;
    *returnSize = 0;
    traverse(root, 0, &arr, columnSizes, returnSize);
    
    return arr;
}

//107. Binary Tree Level Order Traversal II
//    3
//   / \
//  9  20
//    /  \
//   15   7
//[[15,7],[9,20],[3]]
struct LinkedIntNode
{
    int val;
    struct LinkedIntNode *next;
};

struct LinkedInts
{
    int count;
    struct LinkedIntNode *head;
};

struct TreeTraversalResult
{
    int levelLimit;
    struct LinkedInts *levels;
};

void initTreeResult(struct TreeTraversalResult *r, int level)
{
    r->levelLimit = level;
    r->levels = malloc(sizeof(struct LinkedInts) * level);
    memset(r->levels, 0 , sizeof(struct LinkedInts) * level);
}

void addTreeNode(struct TreeTraversalResult *r, int val, int level)
{
    if(r->levelLimit < level + 1)
    {
        struct LinkedInts *newLevels = malloc(sizeof(struct LinkedInts) * (level + 10));
        memset(newLevels, 0, sizeof(struct LinkedInts) * (level + 10));
        memmove(newLevels, r->levels, sizeof(struct LinkedInts) * r->levelLimit);
        r->levelLimit = level + 10;
        r->levels = newLevels;
    }
    struct LinkedIntNode *node = malloc(sizeof(struct LinkedIntNode));
    node->val = val;
    node->next = r->levels[level].head;
    r->levels[level].head = node;
    r->levels[level].count++;
}

void traversal(struct TreeNode* node, int curLevel, struct TreeTraversalResult *r)
{
	addTreeNode(r, node->val, curLevel);
	if (node->left)
	{
		traversal(node->left, curLevel + 1, r);
	}
	if (node->right)
	{
		traversal(node->right, curLevel + 1, r);
	}
}
int** levelOrderBottom(struct TreeNode* root, int** columnSizes, int* returnSize) {
    if(root == NULL) return NULL;
    struct TreeTraversalResult result;
    initTreeResult(&result, 200);
    traversal(root, 0, &result);
    int i;
    for(i = 0; i < result.levelLimit; ++i)
    {
        if(result.levels[i].count == 0) break;
    }
    int **resultArray = malloc(sizeof(int *) * i);
    *columnSizes = malloc(sizeof(int) * i);
    *returnSize = i;
    int tmp;
    for(int j = i - 1, n = 0; j >= 0; --j, ++n)
    {
        (*columnSizes)[n] = tmp = result.levels[j].count;
        resultArray[n] = malloc(sizeof(int) * tmp);
        struct LinkedIntNode *node = result.levels[j].head;
        for(int k = tmp - 1; k >= 0; k--)
        {
            *(resultArray[n]+k) = node->val;
            node = node->next;
        }
    }
    return resultArray;
}



//103. Binary Tree Zigzag Level Order Traversal
//Zig-Zag 遍历，及第一行从左到有，第二行从右到左，第三行从左到右，依次类推。此题仍然可以在层次遍历后，对结果进行处理
/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *columnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
typedef struct TreeNode tn;
int** zigzagLevelOrder(tn* root, int** colS, int* retS) {
  if(root == NULL)
  {
    *retS = 0;
    return NULL; 
  }

  *retS = 0;
  int** ret = (int**)malloc(sizeof(int*) * 1000);
  tn* nodeQ[2000]; // a Queue to hold nodes traversed each level
  tn* temp;
  int level;
  *colS = (int*)malloc(sizeof(int) * 1000);
  memset(*colS, 0, sizeof(int)*1000);
  
  int first = 0, end = 0;  // idx of the Queue
  int numsNodePerLevel = 1, numsNodeCtr=0;
  int j = 0, l=0, m=0;
  bool order=false;
    
  nodeQ[end++] = root;
  while(end > first)
  {
    ret[l] = (int*)malloc(sizeof(int)* 1000);
    m= (order)? numsNodePerLevel-1 : 0;
    for(j=0; j<numsNodePerLevel && end > first; j++)
    {
      // de-Q
      temp = nodeQ[first++];
      if(order == false)
        ret[l][m++] = temp->val;   // left -> right
      else
        ret[l][(m--)] = temp->val; // right-> left
      
      if(temp->left != NULL)
      {
        nodeQ[end++] = temp->left; // en-Q
        numsNodeCtr++;
      }
      if(temp->right!=NULL)
      {
        nodeQ[end++] = temp->right; // en-Q
        numsNodeCtr++;
      }
    }

    (*colS)[l++] = numsNodePerLevel;
    
    numsNodePerLevel = numsNodeCtr;
    if(numsNodeCtr == 0)
      break;
    
    numsNodeCtr = 0;
    order ^= 1;     // alternate the order
  }

  *retS = l;
  return ret;
}

//104. Maximum Depth of Binary Tree 二叉树深度
//广度优先 迭代
int maxDepth(TreeNode *root)  
{  
    if(root == NULL)  
        return 0;  
      
    int res = 0;  
    queue<TreeNode *> q;  
    q.push(root);  
    while(!q.empty())  
    {  
        ++ res;  
        for(int i = 0, n = q.size(); i < n; ++ i)  
        {  
            TreeNode *p = q.front();  
            q.pop();  
              
            if(p -> left != NULL)  
                q.push(p -> left);  
            if(p -> right != NULL)  
                q.push(p -> right);  
        }  
    }  
      
    return res;  
}  

//108. Convert Sorted Array to Binary Search Tree
//二分查找树的题目，要把一个有序数组转换成一颗二分查找树。其实从本质来看，如果把一个数组看成一棵树（也就是以中点为根，左右为左右子树，依次下去）数组就等价于一个二分查找树。所以如果要构造这棵树，那就是把中间元素转化为根，然后递归构造左右子树。所以我们还是用二叉树递归的方法来实现，以根作为返回值，每层递归函数取中间元素，作为当前根和赋上结点值，然后左右结点接上左右区间的递归函数返回值。时间复杂度还是一次树遍历O(n)，总的空间复杂度是栈空间O(logn)加上结果的空间O(n)，额外空间是O(logn)，总体是O(n)
struct TreeNode* convert(int* nums,int start,int end)
{
     if(start > end)
          return NULL;
     else{
         int mid = (start+end)/2;
         struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
         node->val = nums[mid];
         node->left = convert(nums,start,mid-1);
         node->right = convert(nums,mid+1,end);
         return node;
     }
}


struct TreeNode* sortedArrayToBST(int* nums, int numsSize) {
        return convert(nums,0,numsSize-1);
}


//110. Balanced Binary Tree
int max(int a, int b)
{
     return (a >= b)? a: b;
}
int height(struct TreeNode* node)
{
    if(node == NULL)
        return 0;
    return 1 + max(height(node->left), height(node->right));
} 
bool isBalanced(struct TreeNode* root) {
    int lh;
    int rh;  
    if(root == NULL)
        return true; 
    lh = height(root->left);
    rh = height(root->right);

    if( abs(lh-rh) <= 1 && isBalanced(root->left) && isBalanced(root->right))
        return true;

    return false;
}

//111. Minimum Depth of Binary Tree 二叉树最小深度
int minDepth(struct TreeNode* root) {
    if( root == NULL ) return 0;
    int rDepth = minDepth( root->right ) + 1;
    int lDepth = minDepth( root->left ) + 1;
    if( root->right == NULL )
        return lDepth;
    else if( root->left == NULL )
        return rDepth;
    else
        return rDepth < lDepth ? rDepth : lDepth;
}

//112. Path Sum
//给定一个二叉树和一个值sum，判断是否存在一个从根节点到叶子节点的路径，使得路径上每个节点值之和等于sum
bool hasPathSum(struct TreeNode* root, int sum) {
    if (!root) 
        return false;
    if (!root->right && !root->left) 
        return sum==root->val;
    return hasPathSum(root->left, sum-root->val)||hasPathSum(root->right, sum-root->val);
}

//113. Path Sum II
//除了要判断是否有这样的一个path sum，还需要把所有的都可能性结果都返回
//DFS
typedef struct TreeNode tn;

int maxDepth(tn* root)
{
  if(root==NULL)        return 0;
  int lDep = maxDepth(root->left);
  int rDep = maxDepth(root->right);
  return (lDep> rDep)?lDep+1:rDep+1;
}

void helper(tn* node, int sum,
            int* cur, int curS,
            int** colS, int* retS, int** ret)
{
  /* term */
  if(node->val == sum &&
     !node->left && !node->right)
  {
    /* got a match */
    ret[*retS] = (int*)malloc(sizeof(int)* (curS+1));
    memcpy(ret[*retS], cur, sizeof(int) * curS);
    ret[*retS][curS] = node->val;
    (*colS)[*retS] = curS +1;
    (*retS)++;
    return;
  }

  cur[curS] = node->val;
  /* dfs */
  if(node->left)
    helper(node->left, sum-cur[curS], cur, curS+1, colS, retS, ret);

  if(node->right)
    helper(node->right, sum-cur[curS], cur, curS+1, colS, retS, ret);    
}

int** pathSum(tn* root, int sum, int** colS, int* retS)
{
  *retS = 0;
  if(root == NULL) return NULL;

  *colS = (int*)malloc(sizeof(int)*500);
  int** ret = (int**)malloc(sizeof(int*)*500);
  
  int cur[maxDepth(root)];
  helper(root, sum, cur, 0, colS, retS, ret);

  return ret;
}


//114. Flatten Binary Tree to Linked List 将二叉树展开成链表
void flatten(struct TreeNode* root) {
    struct TreeNode* leftt;
    while(root!=NULL){
        if(root->left!=NULL){
            leftt=root->left;
            while(leftt->right!=NULL) leftt=leftt->right;
            leftt->right=root->right;
            root->right=root->left;
            root->left=NULL;
        }
        root=root->right;
    }
}

//116. Populating Next Right Pointers in Each Node
//将一棵树的每一层维护成一个链表
//思路上很接近层序遍历Binary Tree Level Order Traversal，只是这里不需要额外维护一个队列。因为这里每一层我们会维护成一个链表，这个链表其实就是每层起始的那个队列，因此我们只需要维护一个链表的起始指针就可以依次得到整个队列了。接下来就是有左加左入链表，有右加右入链表，知道链表没有元素了说明到达最底层了。算法的复杂度仍然是对每个结点访问一次，所以是O(n)，而空间上因为不需要额外空间来存储队列了，所以是O(1)
//这道题是树的层序遍历Binary Tree Level Order Traversal的扩展，操作上会更加繁琐一些，因为是通过维护层链表来完成遍历，不过本质上还是一次广度优先搜索
//任何二叉树都支持
void connect(struct TreeLinkNode *root) {
    if(root == NULL)  
        return;  
    struct TreeLinkNode *lastHead = root;  
    struct TreeLinkNode *pre = NULL;  
    struct TreeLinkNode *curHead = NULL;  
    while(lastHead!=NULL)  
    {  
        struct TreeLinkNode *lastCur = lastHead;  
        while(lastCur != NULL)  
        {  
            if(lastCur->left!=NULL)  
            {  
                if(curHead == NULL)  
                {  
                    curHead = lastCur->left;  
                    pre = curHead;  
                }  
                else  
                {  
                    pre->next = lastCur->left;  
                    pre = pre->next;  
                }  
            }  
            if(lastCur->right!=NULL)  
            {  
                if(curHead == NULL)  
                {  
                    curHead = lastCur->right;  
                    pre = curHead;  
                }  
                else  
                {  
                    pre->next = lastCur->right;  
                    pre = pre->next;  
                }  
            }                  
            lastCur = lastCur->next;  
  
        }  
        lastHead = curHead;  
        curHead = NULL;  
    } 

}

// 使用递归，则空间为O(height)
 void helper(struct TreeLinkNode *root,struct TreeLinkNode* r){
    if(!root)   return;
    root->next = r;
    helper(root->left,root->right);
    helper(root->right,r?r->left:NULL);
}   
void connect(struct TreeLinkNode *root) {
    helper(root,NULL);
}


//124. Binary Tree Maximum Path Sum 二叉树最大路径和
//给出一棵二叉树，寻找一条路径使其路径和最大，路径可以在任一节点中开始和结束（路径和为两个节点之间所在路径上的节点权值之和）
//给出的代码有问题，待确认
int maxValue;
int maxPathDown(struct TreeNode* node, int maxValue)
{
    if (NULL == node) return 0;
    int left = MAX(0, maxPathDown(node->left));
    int right = MAX(0, maxPathDown(node->right));
    maxValue = MAX(maxValue, left+right+node->val);
    return MAX(left, right) + node->val;
}

int maxPathSum(struct TreeNode* root) {
    int maxValue = INT_MIN;
    maxValue = maxPathDown(root, maxValue);
    return maxValue;
}

//129. Sum Root to Leaf Numbers 根节点到叶子结点的所有权值和
int dfs(struct TreeNode* root, int num) {
    if (root == NULL) return 0; // if only one child node is NULL, do not count it
    if (root->left == NULL && root->right == NULL) return num * 10 + root->val; // if the node is leaf node, return the sum value
    return dfs(root->left, num * 10 + root->val) + dfs(root->right, num * 10 + root->val);
}

int sumNumbers(struct TreeNode* root) {
    return dfs(root, 0);
}


