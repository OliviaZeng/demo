#include <stdio.h>
#include <stdlib.h>

typedef struct BinaryTreeNode {
    int data;
    struct BinaryTreeNode *left;
    struct BinaryTreeNode *right;
}Tree;

int get_node_num(BinaryTreeNode *tree)
{
    if (NULL == tree)
        return 0;

    return get_node_num(tree->left) + get_node_num(tree->right) + 1;
}

int get_depth(BinaryTreeNode *tree)
{
    if (NULL == tree)
        return 0;

    int depth_left = get_depth(tree->left);
    int depth_right = get_depth(tree->right);
    return depth_left > depth_right ? (depth_left+1) : (depth_right+1);
}

void pre_order_traverse(BinaryTreeNode *tree)
{
    if (NULL == tree)
        return;

    printf("%d ", tree->data);
    pre_order_traverse(tree->left);
    pre_order_traverse(tree->right);
}

void in_order_traverse(BinaryTreeNode *tree)
{
    if (NULL == tree)
        return;

    in_order_traverse(tree->left);
    printf("%d ", tree->data);
    in_order_traverse(tree->right);
}

void post_order_traverse(BinaryTreeNode *tree)
{
    if (NULL == tree)
        return;

    post_order_traverse(tree->left);
    post_order_traverse(tree->right);
    printf("%d ", tree->data);
}
