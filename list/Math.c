//7. Reverse Integer 附有溢出整数翻转
//Example1: x = 123, return 321
//Example2: x = -123, return -321
int reverse(int x) {
    long long val = 0;
	do {
		val = val * 10 + x % 10;
		x /= 10;
	} while (x);
	
	return (val > INT_MAX || val < INT_MIN) ? 0 : val;
}

//9. Palindrome Number
//判断数字是否是回文串，只能用常数空间，不能转换成字符串，不能反转数字
//先计算数字的位数，分别通过除法和取余，提取出要对比的对应数字
bool isPalindrome(int x) {
    int revertX = 0;
    int orgX = x;
    while(x > 0) {
        revertX = 10 * revertX + x % 10;
        x = x / 10;
    }
    
    return revertX == orgX;
}

//29. Divide Two Integers 两数相除
//不能用乘法，除法和取余操作

//we assure the factor ret's binary fomula is
//ret = a0 + a1*2 + a2*2^2 + ...... + a29*2^29 + a30*2^30 + a31*2^31; ai = 0 or 1, i = 0......31
//the dividend B and divisor A is non-negative, then
//A(a0 + a1*2 + a2*2^2 + ...... + a29*2^29 + a30*2^30 + a31*2^31) = B; Eq1
//(1) when Eq1 divided by 2^31, we can get A*a31 = B>>31; then a31 = (B>>31)/A;
//if (B>>31) > A, then a31 = 1; else a31 = 0;
//(2) when Eq1 divided by 2^30, we can get A*a30 + A*a30*2 = B>>30; then a30 = ((B>>30) - a30*A*2)/A; and (B>>30) - a31*A*2 can be rewritten by (B-a31*A<<31)>>30, so we make B' = B-a31*A<<31, the formula simplified to a30 = (B'>>30)/A
//if (B'>>31) > A, then a30 = 1; else a30 = 0;
//(3) in the same reason, we can get a29 = ((B-a31*A<<31-a30*A<<30)>>29)/A, we make B'' = B' - a30*A<<30, the formula simplified to a29 = (B''>>29)/A;
//do the same bit operation 32 times, we can get a31 ..... a0, so we get the ret finally
int divide(int dividend, int divisor) {
    //special cases
    if(divisor == 0 || (dividend == INT_MIN && divisor == -1))
        return INT_MAX;
    
    // transform to unsigned int
    bool sign = (dividend > 0)^(divisor > 0);
    unsigned int A = (divisor < 0) ? -divisor : divisor;
    unsigned int B = (dividend < 0) ? -dividend : dividend;
    int ret = 0;
    
    // shift 32 times
    for(int i = 31; i >= 0; i--)
    {
        if((B>>i) >= A)
        {
            ret = (ret<<1)|0x01;
            B -= (A<<i);   // update B
        }
        else
            ret = ret<<1;
    }
    
    if(sign)
        ret = -ret;
    
    return ret;
}


//任何一个整数可以表示成以2的幂为底的一组基的线性组合，即num=a_0*2^0+a_1*2^1+a_2*2^2+...+a_n*2^n。基于以上这个公式以及左移一位相当于乘以2，我们先让除数左移直到大于被除数之前得到一个最大的基。然后接下来我们每次尝试减去这个基，如果可以则结果增加加2^k,然后基继续右移迭代，直到基为0为止。因为这个方法的迭代次数是按2的幂直到超过结果，所以时间复杂度为O(logn)
int divide(int dividend, int divisor) {
    if (!divisor || (dividend == INT_MIN && divisor == -1))
        return INT_MAX;
    int sign = ((dividend < 0) ^ (divisor < 0)) ? -1 : 1;
    long long dvd = labs(dividend);
    long long dvs = labs(divisor);
    int res = 0;
    while (dvd >= dvs) { 
        long long temp = dvs, multiple = 1;
        while (dvd >= (temp << 1)) {
            temp <<= 1;
            multiple <<= 1;
        }
        dvd -= temp;
        res += multiple;
    }
    return sign == 1 ? res : -res; 
}

//43. Multiply Strings
//计算两个字符串表示的非负大整数的乘积，结果仍然用字符串表示
char* multiply(char* num1, char* num2) {
    if(*num1=='0' || *num2=='0') return "0";
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int len = len1+len2;
    int *arr = (int*)malloc(sizeof(int)*len); //the number of digits of the result - len is the top;
    memset(arr, 0, sizeof(int)*len); //this is critical;
    for(int i=len1-1; i > -1; i--)
        for(int j=len2-1; j > -1; j--)
            arr[i+j+1] += (num1[i]-'0')*(num2[j]-'0'); //collect result of each position;
    for(int i=len-1; i > 0; i--) {
        arr[i-1] += arr[i]/10;
        arr[i] %= 10;
    }
    char *s = (char*)malloc(sizeof(char)*(len+1)); //converting the digits result to string;
    int index = 0;
    int i = 0;
    if(arr[i]==0) i++; //in case the zero position has no carry, if it does, ignore it;
    while(i < len)
        s[index++] = arr[i++]+'0';
    s[index] = '\0';
    return s;    
}

//50. Pow(x, n)
double myPow(double x, int n) {
    if(n == 0)
        return 1.0;
    if(n == 1)
        return x;
    long long int m = n>0 ? n : -n;
    double ans = x;
    long long int cur = 1.0;
    while(cur * 2 < m) {
        cur *= 2;
        ans *= ans;
    }
    ans *= myPow(x, (int)(m-cur));
    return n>0 ? ans : 1.0/ans;
}

//69. Sqrt(x)

//60. Permutation Sequence
//1~n组成的序列从小到大排列，按顺序找出第k个序列输出"123" "132" "213" "231" "312" "321"
void swap(int* p, int* q)
{
    int t = *p; *p = *q; *q = t;
}
void reverse(int* nums, int begin, int end)
{
    for(int i = begin; i < (begin+end+1)/2; i++)
        swap(nums+i, nums+end+begin-i);
}

//AC - 0ms;
char* getPermutation(int n, int k)
{
    int *nums = (int*)malloc(sizeof(int)*n);
    for(int i = 0; i < n; i++)
        nums[i] = i+1;
    k--;
    while(k)
    {
        int i=n-1, j=n-1;
        while(i>0 && nums[i]<=nums[i-1]) i--; //make sure the [i..n-1] is in descending order;
        while(nums[j] <= nums[i-1]) j--; //find the first bigger one backwards;
        swap(nums+j, nums+i-1); //ensure the next is bigger;
        reverse(nums, i, n-1); //since [i..n-1] is descending, after reverse it will be ascending and as a result - [i..n-1] will be the smallest - the smallest in the bigger results - the next permutation;
        k--;
    }
    char *s = (char*)malloc(sizeof(char)*(n+1));
    for(int i = 0; i < n; i++)
        s[i] = nums[i]+'0';
    s[n] = '\0';
    return s;
}


