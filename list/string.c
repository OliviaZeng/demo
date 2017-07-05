//3. Longest Substring Without Repeating Characters 
//给定一个字符串，找到其中的一个最长的字串，使得这个子串不包含重复的字符
//Given "abcabcbb", the answer is "abc", which the length is 3.
//Given "bbbbb", the answer is "b", with the length of 1
//hashtable
int lengthOfLongestSubstring(char* s)
{
	int len=0;
    char *end=s,*temp;
	char* addressTable[128]={NULL};
	while (*end) {
		temp = addressTable[*end];
		addressTable[*end] = end;
		if(temp>=s){
            len=end-s>len?end-s:len;
            s = temp+1;
		}
        end++;
	}
	len=end-s>len?end-s:len;
	return len;
}

//双指针，i表示满足条件的字符串的起始位置，j表示结束位置。j每次往前移动一次，遇到一个新的字符元素，判断当前的集合中是否已经存在这个字母，如果存在，移动i指针，直到排除之前出现过的这个元素。类似贪心的思想，两个指针维护了以j结束的最长的满足条件的字符串，因此能得到最优解。这中算法同样能够解决类似的题目例如每种元素最多出现n次的字符串，只需要用sign数组记录出现的次数并判断
int lengthOfLongestSubstring(char* s)
{
    int n = strlen(s);
    int i=0, j=0;
    int maxLen = 0;
    int exist[256] = {0};
    while (j < n) {
        if (exist[s[j]]) {
            maxLen = maxLen>(j-i)? maxLen : (j-i);
            while (s[i] != s[j]) {
                exist[s[i]] = 0;
                i++;
            }
            i++;
            j++;
        } else {
            exist[s[j]] = 1;
            j++;
        }
    }
    maxLen = maxLen>(n-i)? maxLen : (n-i);
    return maxLen;
}

//5. Longest Palindromic Substring 最长回文字串
char* longestPalindrome(char* s) {
    if (NULL == s) return NULL;
    int len = strlen(s);
    if (len == 1) return s;
    int min_start = 0, max_len = 1, i;
    for (i = 0; i < len;) {
        if (len - i <= max_len / 2) break;
        int j = i, k = i;
        while (k < len-1 && s[k+1] == s[k]) ++k; // Skip duplicate characters.
        i = k+1;
        while (k < len-1 && j > 0 && s[k + 1] == s[j - 1]) { ++k; --j; } // Expand.
        int new_len = k - j + 1;
        if (new_len > max_len) { min_start = j; max_len = new_len; }
    }

    char *a = (char *)malloc(max_len+1);
    for (i=0; i<max_len; i++)
        a[i] = s[min_start+i];
    a[i] = '\0';
    return a;
}

//6. ZigZag Conversion
char* convert(char* s, int numRows) {
    if ((NULL == s) | (numRows < 1))
        return NULL;

    const size_t len = strlen(s);
    char* output = (char*) malloc(sizeof(char) * ( len + 1));
    char* head = output;

    output[len] = '\0';
    unsigned int index;
    if ( 1 == numRows )
        return strcpy(output, s);

    for (int row = 0; row < numRows; ++row) {
        //processing row by row using (2nRows-2) rule
        for (index = row; index < len; index += 2*numRows-2) {
            // if it is the first row or the last row, then this is all
            *output++ = s[index];
            // otherwise, there are middle values, using (2nRows-2-2*row) rule
            // notice that nRows-1 is the last row
            if ( (row>0)&(row<numRows-1) & ((index+2*numRows - 2 - 2*row) < len))
                *output++ = s[index+2*numRows - 2 - 2*row];
        }
    }
    return head;
}

//8. String to Integer (atoi)
//discards all leading whitespaces
//sign of the number
//overflow
//invalid input
int myAtoi(char* str) {
    int sign = 1, base = 0, i = 0;
    while (str[i] == ' ') { i++; }
    if (str[i] == '-' || str[i] == '+') {
        sign = 1 - 2 * (str[i++] == '-'); 
    }
    while (str[i] >= '0' && str[i] <= '9') {
        if (base >  INT_MAX / 10 || (base == INT_MAX / 10 && str[i] - '0' > 7)) {
            if (sign == 1) return INT_MAX;
            else return INT_MIN;
        }
        base  = 10 * base + (str[i++] - '0');
    }
    return base * sign;    
}

//10. Regular Expression Matching 正则表达式匹配
//‘.’匹配任意单个字符，‘*’匹配0个或多个前一字符。如果匹配整个串返回true
//isMatch("aa","a") → false
//isMatch("aa","aa") → true
//isMatch("aaa","aa") → false
//isMatch("aa", "a*") → true
//isMatch("aa", ".*") → true
//isMatch("ab", ".*") → true
//isMatch("aab", "c*a*b") → true
//- 若p为空，若s也为空，返回true，反之返回false
//- 若p的长度为1，若s长度也为1，且相同或是p为'.'则返回true，反之返回false
//- 若p的第二个字符不为*，若此时s为空返回false，否则判断首字符是否匹配，且从各自的第二个字符开始调用递归函数匹配
//- 若p的第二个字符为*，若s不为空且字符匹配，调用递归函数匹配s和去掉前两个字符的p，若匹配返回true，否则s去掉首字母
//- 返回调用递归函数匹配s和去掉前两个字符的p的结果
bool isMatch(const char *s, const char *p) 
{   
    if (s == NULL || p == NULL) return false;
    if (*p == '\0') return *s == '\0';
    // ".*" matches "", so we can't check (*s == '\0') here.
    if (*(p + 1) == '*') {
        // Here *p != '\0', so this condition equals with
        // (*s != '\0' && (*p == '.' || *s == *p)).
        while ((*s != '\0' && *p == '.') || *s == *p) {
            if (isMatch(s, p + 2)) return true;
            ++s;
        }
        return isMatch(s, p + 2);
    } else if ((*s != '\0' && *p == '.') || *s == *p) {
        return isMatch(s + 1, p + 1);
    }
    return false;
}

//44. Wildcard Matching 通配符"?"和"*"匹配
//isMatch("aa","a") → false
//isMatch("aa","aa") → true
//isMatch("aaa","aa") → false
//isMatch("aa", "*") → true
//isMatch("aa", "a*") → true
//isMatch("ab", "?*") → true
//isMatch("aab", "c*a*b") → false
bool isMatch(char* s, char* p) {
    char *p0 = NULL, *s0 = NULL;
    while (*s) {
        if (*p == *s || *p == '?') {
            p++; 
            s++;
        }else if (*p == '*') { // save
            p0 = ++p;
            s0 = s;
        }else if (p0 != NULL) { // load
            p = p0;
            s = ++s0;
        }else {
            return false;
        }
    }
    while (*p == '*') p++;
    return !*s && !*p;
}


//12. Integer to Roman
char* intToRoman(int num) {
    if (num > 3999 || num < 1)
        return NULL;
    char *a[4][10] = { {"","I","II","III","IV","V","VI","VII","VIII","IX"},
                       {"","X","XX","XXX","XL","L","LX","LXX","LXXX","XC"},
                       {"","C","CC","CCC","CD","D","DC","DCC","DCCC","CM"},
                       {"","M","MM","MMM"},
                     };
    int len[4][10] = { {0,1,2,3,2,1,2,3,4,2},
                       {0,1,2,3,2,1,2,3,4,2},
                       {0,1,2,3,2,1,2,3,4,2},
                       {0,1,2,3,0,0,0,0,0,0},
                     };
    int chLen[4] = {num%10, num/10%10, num/100%10, num/1000%10};
    int len
    int i, offset=0;
    int strLen = len[3][chLen[3]]+len[2][chLen[2]]+len[1][chLen[1]]+len[0][chLen[0]];
    char *b = (char *)malloc(sizeof(char)*(strLen+1));
    for (i=3; i>=0; i--) {
        strncpy(b+offset, a[i][chLen[i]], len[i][chLen[i]]);
        offset += len[i][chLen[i]];
    }
    b[strLen] = '\0';
    return b;
}


#define _M 1000
#define _D 500
#define _C 100
#define _L 50
#define _X 10
#define _V 5
#define _I 1
int romanToInt(char *s) {
    int result  = 0;
    int i;
    int last = _M;
    for(i = 0;i < strlen(s);i++)
    {
        switch (s[i])
        {
            case 'M':
                result = (last >= _M ? result + _M:result + _M - (last << 1));
                last = _M;        
            break;
            case 'D':
                result = (last >= _D ? result + _D:result + _D - (last << 1));
                last = _D;
            break;
            case 'C':
                result = (last >= _C ? result + _C:result + _C - (last << 1));
                last = _C;
            break;
            case 'L':
                result = (last >= _L ? result + _L:result + _L - (last << 1));
                last = _L;
            break;
            case 'X':
                result = (last >= _X ? result + _X:result + _X - (last << 1));
                last = _X;
            break;
            case 'V':
                result = (last >= _V ? result + _V:result + _V - (last << 1));
                last = _V;
            break;
            case 'I':
                result = (last >= _I ? result + _I:result + _I - (last << 1));
                last = _I;
            break;
        }
    }
    return result;
}

//14. Longest Common Prefix
//找出所有字符串的最长公共前缀
char* longestCommonPrefix(char** strs, int strsSize) {
    if (strsSize <= 0 || strs == NULL) return strs;
    if (strsSize == 1) return strs[0];
    for (int i = 1; i < strsSize; ++i) {
        char* s0 = strs[0];
        char* si = strs[i];
        int pos = 0;
        while (true) {
            if (s0[pos] != si[pos] || s0[pos] == NULL || si[pos] == NULL) {
                s0[pos] = '\0';
                if (pos <= 0) return s0;
                break;
            }
            pos++;
        }
    }
    return strs[0];
}


//17. Letter Combinations of a Phone Number
static const char* letters[10] = {"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

void combine(char* digits, int from, int to, char* result, char** combinations, int* index) {
	if (from <= to) {
		const char *letter = letters[digits[from] - '0'];
		for (int i = 0; i < strlen(letter); ++i) {
			result[from] = letter[i];
			combine(digits, from+1, to, result, combinations, index);
		}
	} else {
		result[from] = '\0';
		combinations[*index] = malloc(sizeof(strlen(result)));
		strcpy(combinations[*index], result);
		(*index)++;
	}
}

char** letterCombinations(char* digits, int* returnSize) {
    if (digits == NULL) {
    	return NULL;
    }
    int digitsLength = strlen(digits);
    if (digitsLength == 0) {
    	return NULL;
    }
    int returnStrLen = 0;
    *returnSize = 1;
    for (int i = 0; i < digitsLength; ++i) {
    	int index = (int)(digits[i] - '0');
    	if (index >= 2 && index <= 9) {
    		*returnSize *= strlen(letters[index]);
    		returnStrLen++;
    	}
    }
    char** combinations = malloc(sizeof(char*) * (*returnSize));
    char* resultStr = malloc(sizeof(returnStrLen+1));
    int* index = malloc(sizeof(int));
    *index = 0;
    combine(digits, 0, digitsLength-1, resultStr, combinations, index);
    return combinations;
}

//20. Valid Parentheses 括号匹配
//完成括号是否正确匹配的功能,使用栈来做
bool isValid(char* s) {
    int len = strlen(s);
    if( len%2 != 0 )
        return false;
    
    int limit = len / 2;
    char *stack = malloc(limit);
    int idx = 0;
    char cur, peek;
    for (int i = 0; i < len; ++i) {
        cur = s[i];
        if (cur == '(' || cur == '[' || cur =='{') {
            if(idx == limit)
                return false;
            else
                stack[idx++] = cur;
        } else {
            if(idx == 0)
                return false;
            peek = stack[idx -1];
            if ( (peek == '(' && cur == ')')
                || (peek == '[' && cur == ']')
                || (peek == '{' && cur == '}') ) {
                idx--;
            }
        }
    }
    return idx == 0;
}

//22. Generate Parentheses
//给定一个非负整数n，生成n对括号的所有合法排列
//回归
int judge(int *s, int len, int cap) {
   int left = 0, right = 0;
   for (int i = 1; i <= len; i++) {
       if (s[i] == 0) {
           left++;
       } else {
           right++;
       }

       if (right > left) {
           return 0;
       }
   }

   if (left > cap / 2) {
       return 0;
   }

   return 1;
 }
char** generateParenthesis(int n, int* returnSize) {
    int s[1024] = {-1};   // solution footprint, the value is the position in ss
    char ss[2] = {'(', ')'}; // solution tree
    int k = 1;
    int num = 0, total = 2 * n;

    // actually the maximum size if pow(2, total), here we just use 4096
    char **ret = (char **) malloc(sizeof(char*) * 4096);

    // Mac can't set default value, so we do memset here
    memset(s, -1, 1024);

    while (k) {
        int i;

        // to next possible choice
        s[k] += 1;

        // try to find available one at level 'k'
        //for (int i = f(n, k); i <= g(n, k); i++) {...}
        while (s[k] <= 1 && !judge(s, k, total)) {
            s[k] += 1;
        }

        if (s[k] <= 1) {
            if (k == total) {
                // find a solution
                char *r = (char *) malloc(sizeof(char) * total);
                for (int j = 1; j <= k; j++) {
                    r[j-1] = ss[s[j]];
                }

                ret[num++] = r;
            } else {
                // goto next level
                k++;
                s[k] = -1;
            }
        } else {
            // backtrace
            k--;
        }
    }

    *returnSize = num;
    return ret;
}

//c++ 递归
class Solution {
public:
    vector<string> generateParenthesis(int n) {
        vector<string> res;
        addingpar(res, "", n, 0);
        return res;
    }
    void addingpar(vector<string> &v, string str, int n, int m){
        if(n==0 && m==0) {
            v.push_back(str);
            return;
        }
        if(m > 0){ addingpar(v, str+")", n, m-1); }
        if(n > 0){ addingpar(v, str+"(", n-1, m+1); }
    }
};

//28. Implement strStr() 
//实现strStr()。返回匹配时的第一个索引， 如果没有匹配的就返回-1
int strStr(char *haystack, char *needle) {
	char *p, *q;
	int i, len = strlen(haystack) - strlen(needle);

	for (i = 0; i <= len; i++) {
		p = haystack + i;
		q = needle;
		while (*q && *p == *q) {
			++p;
			++q;
		}
		if (*q == 0)
			return i;
	}
	return -1;
}

//30. Substring with Concatenation of All Words
//给定一个字符串S和一个字符串数组L，L中的字符串长度都相等，找出S中所有的子串恰好包含L中所有字符各一次，返回子串的起始位置
//S: "barfoothefoobarman"
//L: ["foo", "bar"]
//return the indices: [0,9].
//频率较低，暂时不收录

//32. Longest Valid Parentheses 最长有效小括弧
//给定一个包含‘（’和‘）’的字符串，找出最长的有效括号匹配子串的长度
//time O(n) space O(n) stack
int longestValidParentheses(char* s)
{
    int len = strlen(s);
    int *stack = (int*)malloc(sizeof(int)*len);
    int top = -1;
    stack[++top] = -1;
    int max = 0;
    for(int i = 0; i < len; i++)
    {
        int t = stack[top];
        if(t!=-1 && s[i]==')' && s[t]=='(')
        {
            if(i-stack[--top] > max)
                max = i-stack[top];
        }
        else
            stack[++top] = i;
    }
    return max;
}

//time O(n) space O(n)- DP solution;
int longestValidParentheses2(char* s)
{
    int len = strlen(s);
    if(len < 2) return 0;
    int max = 0;
    int *maxs = (int*)malloc(sizeof(int)*len); //record the max viable length ending with the current;
    memset(maxs, 0, sizeof(int)*len);
    for(int i = 1; i < len; i++)
    {
        if(s[i] == ')')
        {
            if(s[i-1] == '(')
                    maxs[i] = 2+ (i>1? maxs[i-2] : 0);
            else if(i-maxs[i-1] > 0 && s[i-maxs[i-1]-1] == '(')
                    maxs[i] = maxs[i-1] + (i-maxs[i-1]>1? maxs[i-maxs[i-1]-2]:0) + 2; 
            if(maxs[i] > max)
                max = maxs[i];
        }
    }
    return max;
}

//38. Count and Say
//这是一道根据规则推导题目，要求给定序列数n，求出该序列对应的字符串
char* countAndSay(int n) {
    if( n == 1 ) return "1";
	char *cur = malloc(2), *tmp;
	cur[0] = '1';
	cur[1] = 0;
	
	int len, idx, j, count;
	for(int i = 2; i <= n; ++i)
	{
		len = strlen(cur);
		tmp = malloc(len * 3);
		memset(tmp, 0, len * 3);
		count = 1;
		for(idx = 1, j = 0; idx < len; ++idx)
		{
			if(cur[idx] == cur[idx-1])
        	{
            	++count;
        	}
			else
        	{
            	tmp[j++] = '0' + count;
            	tmp[j++] = cur[idx-1];
            	count = 1;
        	}
		}//end of for
		tmp[j++] = '0' + count;
    	tmp[j++] = cur[len-1];
		free(cur);
		cur = tmp;
	}	
	return cur;
}

//43. Multiply Strings
//计算两个字符串表示的非负大整数的乘积，结果仍然用字符串表示.12*12=144
char* multiply(char* num1, char* num2){
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

//49. Group Anagrams
//给定一个字符串数组，将用相同字母（包括个数）组成的字符串放到一起
//For example, given: ["eat","tea","tan","ate","nat","bat"], Return:
//[["ate","eat","tea"],["nat","tan"],["bat"]]
void calculate_hash(char *a, int l, int r) {
    char privot = a[l];
    int i, j;
    char temp;
    j = l;
    if (l < r) { 
        for (i = l; i <= r; i++) {
            if (a[i] < privot) {
                temp = a[j];
                a[j] = a[i];
                a[i] = a[j + 1];
                a[j + 1] = temp;
                j++;
            }
        }
        calculate_hash(a, l, j - 1);
        calculate_hash(a, j + 1, r);
    }
}
void quicksort(char** strs, char** hash, int l, int r) {
    char* privot;
        privot = hash[l];
    char* temp;
    int i, j;
    j = l;
    if (l < r) {
        for (i = l; i <= r; i++) {
            if (0 > strcmp(hash[i], privot)) {
                temp = hash[j];
                hash[j] = hash[i];
                hash[i] = hash[j + 1];
                hash[j + 1] = temp;
                temp = strs[j];
                strs[j] = strs[i];
                strs[i] = strs[j + 1];
                strs[j + 1] = temp;
                j++;
            }
        }
        quicksort(strs, hash, l, j - 1);
        quicksort(strs, hash, j + 1, r);
    }
}
char* hash(char* str) {
    char* result;
    int length;
    int i;
    length = strlen(str);
    result = (char*)malloc((length + 1) * sizeof(char));
    for (i = 0; i < length; i++) {
        result[i] = str[i];
    }
    result[length] = '\0';
    calculate_hash(result, 0, length - 1);
    return result;
}
char*** groupAnagrams(char** strs, int strsSize, int** columnSizes, int* returnSize) {
    int i;
    char** hash_value;
    char*** result;
    if (0 == strsSize) {
        return NULL;
    }
    *returnSize = 0;
    result = (char***)malloc(sizeof(char**));
    result[0] = (char**)malloc(sizeof(char*)); 
    *columnSizes = (int*)malloc(sizeof(int));
    hash_value = (char**)malloc(strsSize * sizeof(char*));
    for (i = 0; i < strsSize; i++) {
        hash_value[i] = hash(strs[i]);
    }
    quicksort(strs, hash_value, 0, strsSize - 1);
    result[0][0] = strs[0];
    (*columnSizes)[0] = 1;
    for (i = 1; i < strsSize; i++) {
        if (0 != strcmp(hash_value[i], hash_value[i - 1])) {
            *returnSize += 1;
            result = realloc(result, (*returnSize + 1) * sizeof(char**));
            result[*returnSize] = (char**)malloc(sizeof(char*));
            *columnSizes = realloc(*columnSizes, (*returnSize + 1) * sizeof(int));
            (*columnSizes)[*returnSize] = 0;
        }
        (*columnSizes)[*returnSize] += 1;
        result[*returnSize] = realloc(result[*returnSize], ((*columnSizes)[*returnSize] + 1) * sizeof(char*));
        result[*returnSize][(*columnSizes)[*returnSize] - 1] = strs[i]; 
    }
    *returnSize += 1;
    return result;
}

//58. Length of Last Word（最后一个单词的长度）
//Given s = "Hello World", return 5.
int lengthOfLastWord(char* s) {
  int lastLen = 0;
  char* p = s + strlen(s) -1;
  while(p>=s && isspace(*p)) p--;
  while(p>=s && !isspace(*(p--))) lastLen++;
  return lastLen;
}

//65. Valid Number 判断输入的字符串是否是合法的数
bool isNumber(char* s) {
    char c;
    //in : have integer <==>fr : have fraction
    bool in = false, fr = false;
    //ein : left of e <==> eout : right of e
    bool ein = false, eout = false;
    //1. space
    while(isspace(*s))
    {
        s++;
    }
    //2. after space
    if((!isdigit(*s)) && (*s != 'e') && (*s != '.') && (*s != '+') && (*s != '-'))
    {
        return false;
    }
    //3. + -
    if((*s == '+') || (*s == '-'))
    {
        s++;
    }
    //4. integer part
    if(isdigit(*s))
    {
        in = true;
        ein = true;
    }
    while(isdigit(*s))
    {
        s++;
    }
    //5. '.' and 'e'
    if((*s) == '.')
    {
        s++;
        if(isdigit(*s))
        {
            fr = true;
        }
        if((!fr) && (!in))
        {
            return false;
        }
        while(isdigit(*s))
        {
            s++;
        }
        ein = true;
    }
    if(*s == 'e')
    {
        s++;
        if((*s == '-') || (*s == '+'))
        {
            s++;
        }
        if(isdigit(*s))
        {
            eout = true;
        }
        if((!ein) || (!eout))
        {
            return false;
        }
        while(isdigit(*s))
        {
            s++;
        }
    }
  
    //6. ending space
    while(isspace(*s))
    {
        s++;
    }
    return (*s == '\0') ? true : false;
    
}

//67. Add Binary
//计算两个二进制的字符串的和，返回值依然是个二进制字符串


char* addBinary(char* a, char* b) {
    int la = strlen(a);
    int lb = strlen(b);
    int lr = la > lb ? la : lb;
    int carry = 0;
    char *res = (char*)calloc(lr + 2, sizeof(char));
    res[lr + 1] = '\0';
    la--; lb--;
    while (la >= 0 || lb >= 0) {
        int ba = la >= 0 ? a[la--] - '0' : 0;
        int bb = lb >= 0 ? b[lb--] - '0' : 0;
        int br = ba ^ bb ^ carry;
        carry = (ba & bb) | (carry & (ba ^ bb));
        res[lr--] = br + '0';
    }
    if (!carry) return res + 1;
    res[0] = '1';
    return res;
}

//71. Simplify Path
//path = "/home/", => "/home"
//
//path = "/a/./b/../../c/", => "/c"
//
//Corner Cases:
//
//    Did you consider the case where path = "/../"?
//    In this case, you should return "/".
//    Another corner case is the path might contain multiple slashes '/' together, such as "/home//foo/".
//    In this case, you should ignore redundant slashes and return "/home/foo".
//
//这道题的要求是简化一个Unix风格下的文件的绝对路径。
//
//字符串处理，由于".."是返回上级目录（如果是根目录则不处理），因此可以考虑用栈记录路径名，以便于处理。需要注意几个细节：
//
//    重复连续出现的'/'，只按1个处理，即跳过重复连续出现的'/'；
//    如果路径名是"."，则不处理；
//    如果路径名是".."，则需要弹栈，如果栈为空，则不做处理；
//    如果路径名为其他字符串，入栈。
//
//最后，再逐个取出栈中元素（即已保存的路径名），用'/'分隔并连接起来，不过要注意顺序呦。
//
//时间复杂度：O(n) 空间复杂度：O(n)

char* simplifyPath(char* path) 
{
    int top = -1;
    int i;
    int j;

    for(i = 0; path[i] != '\0'; ++i)
    {
        path[++top] = path[i]; 
        if(top >= 1 && path[top - 1] == '/' && path[top] == '.' && (path[i + 1] == '/' || path[i + 1] == '\0'))
        {
            top -= 2;
        }
        else if(top >= 2 && path[top - 2] == '/' && path[top - 1] == '.' && path[top] == '.' && (path[i + 1] == '/' || path[i + 1] == '\0'))
        {
            for(j = top - 3; j >= 0; --j)
            {
                if(path[j] == '/') break;
            }
            if(j < 0)
            {
                top = -1;
            }
            else
            {
                top = j - 1;
            }
        }
        else if(path[top] == '/' && path[i + 1] == '/') --top;
    }
    if(top > 0) 
    {
        if(path[top] == '/') path[top] = '\0';
        else path[top + 1] = '\0';
    }
    else if(top == 0) path[top + 1] = '\0';
    else 
    {
        path[0] = '/'; 
        path[1] = '\0';
    }
    return path;
}

//72. Edit Distance
//动态规划 hard
int arr[1024][1024] = {0};
int min_(int a,int b,int c){
	if(a<=b&&a<=c) return a;
	if(b<=a&&b<=c) return b;
	if(c<=a&&c<=b) return c;
}
int minDistance(char *word1, char *word2) {
	int l1 = 0,l2 = 0;
	for(l1 = 0;word1[l1];arr[l1][0] = ++l1);
	for(l2 = 0;word2[l2];arr[0][l2] = ++l2);
	for(int i = 1;i<=l1;i ++)
		for(int j = 1;j<=l2;j ++)
			arr[i][j] = min_(arr[i-1][j-1]+(word1[i-1]!=word2[j-1]),arr[i][j-1]+1,arr[i-1][j]+1);
	return arr[l1][l2];
}

//125. Valid Palindrome
//输入一个字符串，还原成合理的ip地址，要求输出所有的可能性
#define INRANGE(c, m, n) ((c) >= (m) && c <= (n))

char** restoreIpAddresses(char* s, int* returnSize);
char* getAddr(char* s, int l, int a, int b, int c);
int isValidPart(char* s, int l);

/**
 * Loop through all combinations of 3 dot positions and check if each part is
 * valid IP address part
 */
char** restoreIpAddresses(char* s, int* returnSize)
{
    *returnSize = 0;

    int l = strlen(s);
    if (l < 4 || l > 12) return NULL;

    char** addrs = malloc(sizeof(char*) * ((l-1) * (l-2) * (l-3) / 6));
    char* addr;

    int a, b, c;

    for (a = 1; a < 4; a++) {
        if (a > l - 3 || a < l - 9) continue;
        for (b = a + 1; b < a + 4; b++) {
            if (b > l - 2 || b < l - 6) continue;
            for (c = b + 1; c < b + 4; c++) {
                if (c > l - 1 || c < l - 3) continue;
                if (addr = getAddr(s, l, a, b, c))
                    addrs[(*returnSize)++] = addr;
            }
        }
    }

    return addrs;
}

char* getAddr(char* s, int l, int a, int b, int c)
{
    if (isValidPart(s, a) &&
        isValidPart(s+a, b-a) &&
        isValidPart(s+b, c-b) &&
        isValidPart(s+c, l-c))
    {
        char* addr = malloc(sizeof(char) * (l + 4));
        memcpy(addr, s, a);
        memcpy(addr+a+1, s+a, b-a);
        memcpy(addr+b+2, s+b, c-b);
        memcpy(addr+c+3, s+c, l-c);
        addr[a] = addr[b+1] = addr[c+2] = '.';
        addr[l+3] = '\0';
        return addr;
    } else
        return NULL;
}

int isValidPart(char* s, int l)
{
    if (l == 1)
        return INRANGE(s[0], '0', '9');
    else if (l == 2)
        return INRANGE(s[0], '1', '9') && INRANGE(s[1], '0', '9');
    else if (l == 3)
        return (s[0] == '1' && INRANGE(s[1], '0', '9') && INRANGE(s[2], '0', '9')) ||
               (s[0] == '2' && (
                   INRANGE(s[1], '0', '4') && INRANGE(s[2], '0', '9') ||
                   s[1] == '5' && INRANGE(s[2], '0', '5')
               ));
    else return 0;
}


//76. Minimum Window Substring
//87. Scramble String
//91. Decode Ways
//93. Restore IP Addresses
//97. Interleaving String
//115. Distinct Subsequences
//126. Word Ladder II
//151. Reverse Words in a String
