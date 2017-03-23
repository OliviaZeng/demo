#include <stdio.h>
#include <stdlib.h>


void bubble(int a[], int n)
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

void insert(int a[], unsigned int n)
{
	int i, j;
	int tmp;
	for (i=1; i<n; i++) {
		tmp = *(a+i);
		for (j=i; j>0&&*(a+j-1)>tmp; j--)
			*(a+j) = *(a+j-1);
		*(a+j) = tmp;
	}
}

void select_sort(int *a, unsigned int n)
{
	int i, j, min, t;
	for (i=0; i<n-1; i++) {
		min = i;
		for (j=i+1; j<n; j++)
			if (a[min]>a[j])
				min = j;
		if (min != i) {
			t = a[min];
			a[min] = a[i];
			a[i] = t;
		}
	}
}

int main(int argc, char *argv[])
{
	int bubble_arr[9] = {5,2,1,7,6,9,8,3,4};
	int insert_arr[9] = {5,2,1,7,6,9,8,3,4};
	int select_arr[9] = {5,2,1,7,6,9,8,3,4};
	int i;

	bubble(bubble_arr, 9);
	insert(insert_arr, 9);
	select_sort(select_arr, 9);

	
	for (i=0; i<9; i++)
		printf("%d ", bubble_arr[i]);

	printf("\n");

	for (i=0; i<9; i++)
		printf("%d ", insert_arr[i]);

	printf("\n");

	for (i=0; i<9; i++)
		printf("%d ", select_arr[i]);



	for (i=0; i<0;i++)
		printf("i=%d\n", i);
	return 0;
}
