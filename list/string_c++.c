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

///////////////////////////////////////////////
//String类的构造函数、析构函数、赋值函数

/*
class String {
    public:
        String(const char *str=NULL); //普通构造函数
        String(const String &other);  //拷贝构造函数
        ~String(void);               //析构函数
        String & operate = (const String &other); //赋值函数
    private:
        char *m_data; //用于保存字符串
};

String::String(const char *str)
{
    if (NULL == str) {
        m_data = new char[1];
        *m_data = '\0';
    } else {
        int len = strlen(str);
        m_data = new char[len+1];
        strcpy(m_data, str);
    }
}

String::~String(void)
{
    delete [] m_data;
}

String::String(const String &other)
{
    int len = strlen(other.m_data);
    m_data = new char[len+1];
    strcpy(m_data, other.m_data);
}

String & String::operate = (const String &other)
{
    if (this == &other)
        return *this;
    delete [] m_data;
    int len = strlen(other.m_data);
    m_data = new char[len+1];
    strcpy(m_datt, other.m_data);
    return *this;
}
*/
