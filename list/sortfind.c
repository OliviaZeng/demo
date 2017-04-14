#include <stdio.h>
#include <stdlib.h>


void bubble_sort(int a[], int n)
{
	int i, j, tmp;
	for (i=0; i<n-1; i++)
		for (j=0; j<n-1-i; j++) {
			if (a[j] > a[j+1]) {
				tmp = a[j];
				a[j] = a[j+1];
				a[j+1] = tmp;
			}
		}
}

void insert_sort(int a[], int n)
{
    int i, j, tmp;
    for (i=1; i<n; i++) {
        tmp = a[i];
        for (j=i; j>0&&a[j-1]>tmp; j--)
            a[j] = a[j-1];
        a[j] = tmp;
    }
}

void select_sort(int a[], int n)
{
	int i, j, min, tmp;
	for (i=0; i<n-1; i++) {
		min = i;
		for (j=i+1; j<n; j++)
			if (a[min]>a[j])
				min = j;
		if (min != i) {
			tmp = a[min];
			a[min] = a[i];
			a[i] = tmp;
		}
	}
}

void shell_sort(int a[], int n)
{
    int i, j, gap, tmp;
    for (gap = n/2; gap > 0; gap /= 2)
        for (i = gap; i < n; i++) {
            tmp = a[i];
            for (j = i; j>=gap; j-=gap)
                if (tmp < a[j-gap])
                    a[j] = a[j-gap];
                else
                    break;
            a[j] = tmp;
        }
}

//从最后一个非叶子结点开始，每次都是从父结点、左孩子、右孩子中进行比较交换，交换可能会引起孩子结点不满足堆的性质，所以每次交换之后需要重新对被交换的孩子结点进行调整
void heap_adjust(int a[], int i, int n)
{
    int child;
    int parent;
    for (parent=a[i]; 2*i+1<n; i=child) {
        //子结点的位置=2*（父结点位置）+1
        child = 2*i+1;
        if (child != n-1 && a[child+1]>a[child])   //调整为大根堆
        //if (child != n-1 && a[child+1]<a[child]) //调整为小根堆
            child++;
        //如果较大的子结点大于父结点那么把较大的子结点往上移动，替换它的父结点
        if (parent < a[child])   //调整为大根堆
        //if (parent > a[child]) //调整为小根堆
            a[i] = a[child];
        else
            break;
    }
    a[i] = parent;
}

/* 递增数组
 * 1.将原始数组调整为大根堆
 * 2.将最大值与数组最后项交换
 * 3.新数组调整为大根堆
 */
void heap_sort(int a[], int n)
{
    int i;
    //调整序列的前半部分元素，调整完之后第一个元素是序列的最大的元素
    for (i=n/2; i>=0; i--)
        heap_adjust(a, i, n);

    for (i=n-1; i>0; i--) {
        //把第一个元素和当前的最后一个元素交换，
        //保证当前的最后一个位置的元素都是在现在的这个序列之中最大的
        a[i] = a[0]^a[i];
        a[0] = a[0]^a[i];
        a[i] = a[0]^a[i];
        //不断缩小调整heap的范围，每一次调整完毕保证第一个元素是当前序列的最大值
        heap_adjust(a, 0, i);
    }
}

void merge(int a[], int b[], int low, int mid, int high)
{
    int i = low, j = mid + 1;
    int m = mid, n = high;
    int k = 0;

    while (i <= m && j <= n) {
        if (a[i] <= a[j])
            b[k++] = a[i++];
        else
            b[k++] = a[j++];
    }

    while (i <= m)
        b[k++] = a[i++];

    while (j <= n)
        b[k++] = a[j++];

    for (i=0; i<k; i++)
        a[low+i] = b[i];
}
void m_sort(int a[], int b[], int low, int high)
{
    int mid;
    if (low < high) {
        mid = (low+high) / 2;
        m_sort(a, b, low, mid);
        m_sort(a, b, mid+1, high);
        merge(a, b, low, mid, high);
    }
}
void merge_sort(int a[], int n)
{
    int *b = malloc(n*sizeof(int));
    if (NULL == b)
        return;
    m_sort(a, b, 0, n-1);
}


/*
 * 基本快速排序
 * 固定基元
 */
void quick_sort1(int a[], int left, int right)
{
    if (left >= right)
        return;

    int i = left;
    int j = right;
    int key = a[left];

    while (i < j) {
        while (i < j && key <= a[j])
            j--;
        a[i] = a[j];

        while (i < j && key >= a[i])
            i++;
        a[j] = a[i];
    }
    a[i] = key;
    quick_sort1(a, left, i-1);
    quick_sort1(a, i+1, right);
}



void swap(int *l, int *r)
{
    int tmp = *l;
    *l = *r;
    *r = tmp;
}
int median3(int a[], int left, int right)
{
    int mid = left + ((right-left) >> 1);
    if (a[left] > a[mid])
        swap(&a[left], &a[mid]);
    if (a[left] > a[right])
        swap(&a[left], &a[right]);
    if (a[mid] > a[right])
        swap(&a[mid], &a[right]);

    swap(&a[mid], &a[right-1]);
    return a[right-1];
}
void q_sort(int a[], int left, int right)
{
    int i, j, pivot;
    if (left + 3 <= right) {
        pivot = median3(a, left, right);
        i = left;
        j = right - 1;
        while (1) {
            while (a[++i] < pivot) {}
            while (a[--j] < pivot) {}
            if (i < j)
                swap(&a[i], &a[j]);
            else
                break;
        }
        swap(&a[i], &a[right-1]);

        q_sort(a, left, i-1);
        q_sort(a, i+1, right);
    } else {
        insert_sort(a+left, right-left+1);
    }
    
}
void quick_sort2(int arr[], int n)
{
    q_sort(arr, 0, n-1);
}



int b_search_recursion(int arr[], int low, int high, int key)
{
    if (low > high)
        return -1;
    int mid = low + ((high - low) >> 1);
    if (arr[mid] > key)
        return b_search_recursion(arr, low, mid-1, key);
    if (arr[mid] < key)
        return b_search_recursion(arr, mid+1, high, key);
    return mid;
}

int b_search(int arr[], int low, int high, int key)
{
    while (low <= high) {
        int mid = low + ((high - low) >> 1);
        if (arr[mid] > key)
            high = mid-1;
        else if (arr[mid] < key)
            low = mid + 1;
        else
            return mid;
    }
    return -1;
}

int main(int argc, char *argv[])
{
	int bubble_arr[9] = {5,2,1,7,6,9,8,3,4};
	int insert_arr[9] = {5,2,1,7,6,9,8,3,4};
	int select_arr[9] = {5,2,1,7,6,9,8,3,4};
	int merge_arr[9] = {5,2,1,7,6,9,8,3,4};
	int shell_arr[9] = {5,2,1,7,6,9,8,3,4};
	int heap_arr[9] = {5,2,1,7,6,9,8,3,4};
	int quick_arr1[9] = {5,2,1,7,6,9,8,3,4};
	int quick_arr2[9] = {5,2,1,7,6,9,8,3,4};
	int i;

	bubble_sort(bubble_arr, 9);
	insert_sort(insert_arr, 9);
	select_sort(select_arr, 9);
	merge_sort(merge_arr, 9);
	shell_sort(shell_arr, 9);
	heap_sort(heap_arr, 9);
	quick_sort1(quick_arr1, 0, 9-1);
	quick_sort2(quick_arr2, 9);

	for (i=0; i<9; i++)
		printf("%d ", bubble_arr[i]);
	printf("\n");

	for (i=0; i<9; i++)
		printf("%d ", insert_arr[i]);
	printf("\n");

	for (i=0; i<9; i++)
		printf("%d ", select_arr[i]);
	printf("\n");

	for (i=0; i<9; i++)
		printf("%d ", merge_arr[i]);
	printf("\n");

	for (i=0; i<9; i++)
		printf("%d ", shell_arr[i]);
	printf("\n");

	for (i=0; i<9; i++)
		printf("%d ", heap_arr[i]);
	printf("\n");

	for (i=0; i<9; i++)
		printf("%d ", quick_arr1[i]);
	printf("\n");

	for (i=0; i<9; i++)
		printf("%d ", quick_arr2[i]);
	printf("\n");

	return 0;
}
