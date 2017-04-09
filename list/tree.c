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
        return;

    Tree *left = mirror(tree->left);
    Tree *right = mirror(tree->right);

    tree->left = left;
    tree->right = right;
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
