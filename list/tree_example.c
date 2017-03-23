/*
 * Description:
 *           二叉搜索树的相关操作（创建，插入节点，前、中、后序递归和非递归遍历二叉树）
 * Author  :FinL
 * Language: C
 * Date    : 2010-08-29
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
	int data;
	struct Node *leftchild;
	struct Node *rightchild;
}Node;


/*
 初始化一棵二叉树排序树。
*/
void InitBinaryTree(Node**root,int elem)
{
	*root=(Node*)malloc(sizeof(Node));
	if(!(*root))
	{
		printf("Memory allocation for root failed.\n");
		return;
	}
	(*root)->data=elem;
	(*root)->leftchild=NULL;
	(*root)->rightchild=NULL;
}

/*
 向二叉树排序树中插入结点。
*/
void InsertNode(Node *root,int elem)
{
	Node *newnode=NULL;
	Node *p=root,*last_p=NULL;

	newnode=(Node*)malloc(sizeof(Node));
	if(!newnode)
	{
		printf("Memory allocation for newnode failed.\n");
		return;
	}
	newnode->data=elem;
	newnode->leftchild=NULL;
	newnode->rightchild=NULL;

	while(NULL!=p)
	{
		last_p=p;
		if(newnode->data)
		{
			p=p->leftchild;
		}
		else if(newnode->data>p->data)
		{
			p=p->rightchild;
		}
		else
		{
			printf("Node to be inserted has existed.\n");
			free(newnode);
			return;
		}
	}
	p=last_p;
	if(newnode->data)
	{
		p->leftchild=newnode;
	}
	else
	{
		p->rightchild=newnode;
	}
}

/*
 创建一棵二叉树排序树。
*/
void CreatBinarySearchTree(Node **root,int data[],int num)
{
	int i;

	for(i=0;i<num; i++)
	{
		if(NULL==*root)
		{
			InitBinaryTree(root,data[i]);
		}
		else
		{
			InsertNode(*root,data[i]);
		}
	}
}


/*
 前序遍历二叉树，递归方法。
*/
void PreOrderRec(Node *root)
{
	if(NULL!=root)
	{
		printf("%d  ",root->data);
		PreOrderRec(root->leftchild);
		PreOrderRec(root->rightchild);
	}
}


/*
 前序遍历二叉树，非递归方法。
*/
void PreOrderNoRec(Node *root)
{
	Node *p=root;
	Node *stack[30];
	int num=0;
	while(NULL!=p||num>0)
	{
		while(NULL!=p)
		{
			printf("%d  ",p->data);
			stack[num++]=p;
			p=p->leftchild;
		}
		num--;
		p=stack[num];
		p=p->rightchild;
	}
	printf("\n");
}


/*
 中序遍历二叉树，递归方法。
*/
void InOrderRec(Node *root)
{
	if(NULL!=root)
	{
		InOrderRec(root->leftchild);
		printf("%d  ",root->data);
		InOrderRec(root->rightchild);
	}
}

/*
 中序遍历二叉树，非递归方法，使用栈。
*/
void InOrderNoRec(Node *root)
{
	Node *p=root;
	int num=0;
	Node *stack[30];
	while(NULL!=p||num>0)
	{
		while(NULL!=p)
		{
			stack[num++]=p;
			p=p->leftchild;
		}
		num--;
		p=stack[num];
		printf("%d  ",p->data);
		p=p->rightchild;
	}
	printf("\n");
}

/*
 后序遍历二叉树，递归方法。
*/
void PostOrderRec(Node *root)
{
	if(NULL!=root)
	{
		PostOrderRec(root->leftchild);
		PostOrderRec(root->rightchild);
		printf("%d  ",root->data);
	}
}

/*
 后序遍历二叉树，非递归方法。
*/
void PostOrderNoRec(Node *root)
{
	Node *p=root;
	Node *stack[30];
	int num=0;
	Node *have_visited=NULL;

	while(NULL!=p||num>0)
	{
		while(NULL!=p)
		{
			stack[num++]=p;
			p=p->leftchild;		
		}
		p=stack[num-1];
		if(NULL==p->rightchild||have_visited==p->rightchild)
		{
			printf("%d  ",p->data);
			num--;
			have_visited=p;
			p=NULL;
		}
		else
		{
			p=p->rightchild;
		}
	}
	printf("\n");
}

int main()
{
	Node *root=NULL;
	int num=0;
	int data[]={5,2,4,0,0,5,0,0,3,6,8,0,10,0,7,0,9};
	num=sizeof(data)/sizeof(int);

	CreatBinarySearchTree(&root,data,num);

	printf("This is Preorder traversal.\n");
	PreOrderNoRec(root);
	PreOrderRec(root);
	printf("\n");

	printf("This is Inorder traversal.\n");
	InOrderNoRec(root);
	InOrderRec(root);
	printf("\n");

	printf("This is Postorder traversal.\n");
	PostOrderNoRec(root);
	PostOrderRec(root);
	printf("\n");

	return 0;

}
