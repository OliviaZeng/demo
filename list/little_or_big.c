#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int i = 0x1234;
	void *n = 0;
	printf("%x\n", (*(char*)&i));
	printf("%d\n", sizeof(&n));

}
