#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//分配空间头尾交换
char *reverse(char *s)
{
	if (NULL == s)
		return NULL;

    int len = strlen(s);
	char *tmp = (char *)malloc(sizeof(char)*len+1);
    if (NULL == tmp)
        return NULL;
	int i;
	for (i=0; i<len; i++)
		tmp[len-i-1] = s[i];
	tmp[len] = '\0';
	return tmp;
}

//使用字符串的结束符'\0'所在的位置作为交换空间
char *reverse_endchar(char *s)
{
    char *start = s;
    char *left = s;
    char ch;
    while (*s++)
        ;
    s -= 2;
    printf("%s\n", s);
    while (left < s) {
        ch = *left;
        *left++ = *s;
        *s-- = ch;
    }
    return start;
}

char *reverse_xor(char *s)
{
    char* r = s ;

    //令p指向字符串最后一个字符
    char* p = s;
    while (*(p + 1) != '\0')
        ++p ;

    // 使用异或操作进行交换
    while (p > s)
    {
        *p = *p ^ *s ;
        *s = *p ^ *s ;
        *p = *p-- ^ *s++ ;
    }

    return r ;

}

int main()
{
    char s[5] = {'a','b', 'c', 'd'};
	//char *t = reverse(s);
	//char *t = reverse_xor(s);
	char *t = reverse_endchar(s);
	if (NULL != t)
		printf("%s\n", t);

    return 0;
}
