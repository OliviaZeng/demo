#include <stdio.h>
#include <stdlib.h>

#define max(x, y) (x)>(y)?(x):(y)

typedef struct node {
    int data;
    struct node *left;
    struct node *right;
    int height;
}Node, Tree;

Tree *destory(Tree *tree)
{
    if (NULL != tree) {
        destory(tree->left);
        destory(tree->right);
        free(tree);
    }
    return NULL;
}

int height(Node *node)
{
    if (NULL == node)
        return -1;
    else
        return node->height;
}
/* LL 
 * 关联左儿子
 * 子右变父左
 * 父变子右
 * 父高度 = max(左树，右树)   + 1
 * 子高度 = max(左树，新右树) + 1
 */
Node *singel_rotate_with_left(Node *root)
{
    Node *lchild = root->left;
    root->left = lchild->right;
    lchild->right = root;
    root->height = max(height(root->left), height(root->right)) + 1;
    lchild->heitht = max(height(lchild->left), root->height) + 1;
    return lchild;
}

/* RR
 * 关联右儿子
 * 子左变父右
 * 父变子左
 * 父高度 = max(左树，右树)   + 1
 * 子高度 = max(新左树，右树) + 1
 */
Node *single_rotate_with_right(Node *root)
{
    Node *rchild = root->right;
    root->right = rchild->left;
    rchild->left = root;
    root->height = max(height(root->left), height(root->right)) + 1;
    rchild->height = max(height(root->height, rchild->right)) + 1;
    return rchild;
}
/* LR
 * 以左子为根进行RR
 * 以root为根进行LL
 */
Node *double_rotate_with_left(Node *root)
{
    root->left = single_rotate_with_right(root->left);
    return single_rotate_with_left(root);
}

/* RL
 * 以右子为根进行LL
 * 以root为根进行RR
 */
Node *double_rotate_with_right(Node *root)
{
    root->right = single_rotate_with_left(root->right);
    return single_rotate_with_right(root);
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
        if (height(tree->left)-height(tree->right) == 2)
            if (data < tree->left->data)
                tree = single_rotate_with_left(tree);
            else
                tree = double_rotate_with_left(tree);
    } else if (data > tree->data) {
        tree->right = insert(tree->right, data);
        if (height(tree->right)-height(tree->left) == 2)
            if (data > tree->right->data)
                tree = single_rotate_with_right(tree);
            else
                tree = double_rotate_with_right(tree);
    }

    tree->height = max(height(tree->left), height(tree->right)) + 1;
    return tree;
}

Tree *delete(Tree *tree, int data)
{
    
}

Tree *find(Tree *tree, int data)
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

Tree *find_min_r(Tree *tree)
{
    if (NULL == tree)
        return NULL;

    if (NULL == tree->left)
        return tree;
    else
        return find_min_r(tree->left);
}

Tree *find_min(Tree *tree)
{
    if (NULL == tree)
        return NULL;
    while (tree->left)
        tree = tree->left;
    return tree;
}


