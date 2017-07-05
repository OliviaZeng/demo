//485. Max Consecutive Ones 最大连续1的个数
int findMaxConsecutiveOnes(int* nums, int numsSize) {
    int i, max=0, count=0;

    for (i=0; i<numsSize; i++) {
        if (nums[i]) {
            count++;
            if (max < count)
                max = count;
        } else {
            count = 0;
        }
    }
    return max;    
}

//快一点
int findMaxConsecutiveOnes(int* nums, int numsSize) {
    int max = 0;
    int sum = 0;
    for (int i=0; i<numsSize; i++) {
        sum = (sum+nums[i])*nums[i];
        if(max<sum)
            max=sum;
    }
    return max;    
}

//448. Find All Numbers Disappeared in an Array
//给定一个整数数组,其中1<=a[i]<=n,一些元素出现两次,其他的出现一次.寻找所有[1, n]中没有出现在数组中的元素.不使用额外空间并在O(n)运行时间求解
int* findDisappearedNumbers(int* nums, int numsSize, int* returnSize) {
    int i, j=0;
    int *arr = (int *)malloc(sizeof(int)*numsSize);
    for (i=0; i<numsSize; i++)
        arr[i] = i+1;

    for (i=0; i<numsSize; i++)
        arr[nums[i]-1] = 0;


    for (i=0; i<numsSize; i++) {
        if (arr[i]) {
            if (i==j) {
                j++;
            } else {
                arr[j] = arr[i];
                arr[i] = 0;
                j++;
            }
        }
    }
    *returnSize=j;
    return arr;
}

//283. Move Zeroes 将数组中的0移到最后
//给出数组 nums  = [0, 1, 0, 3, 12] ，调用完函数后，数组元素的顺序会变为 [1, 3, 12, 0, 0]。
//1）不能复制一个新数组；2）你需要最小化对数字的操作次数
//1）将非0数字依次向前移动;2）将后面空出的部分全部补0
void moveZeroes(int* nums, int numsSize) {
    int totalZeros = 0;
    int i;
    for (i=0; i<numsSize; i++) {
        if (0 == nums[i]) {
            totalZeros++;
        } else {
            nums[i-totalZeros] = nums[i];
        }
    }
    for (i = (numsSize-totalZeros); i<numsSize; i++)
        nums[i] = 0;  
}

//1. Two Sum
//给定任意一个数组，和一个固定值，要求你在数组中找出满足a+b = target的情况，并返回这两个值的索引。当然数组里的元素并非成线性排布
//速度比两个指针那种方法快
int* twoSum(int* nums, int numsSize, int target) {
    int *returnSize = malloc(sizeof(int)*2);
    returnSize[0]=returnSize[1]=0;
    int maxPosiNum=0;
    int minNegaNum=0;
    
    for(int i=0;i<numsSize;i++){
        if(nums[i]>maxPosiNum)
            maxPosiNum=nums[i];
        else if(nums[i]<minNegaNum)
            minNegaNum=nums[i];
    }
    
    int PosiArr[maxPosiNum+1];
    int PosiArr1[maxPosiNum+1]; //if the number appears more than once, then put it in this array
    int NegaArr[-minNegaNum+1];
    int NegaArr1[-minNegaNum+1];
    memset(PosiArr,0,sizeof(int)*(maxPosiNum+1));
    memset(PosiArr1,0,sizeof(int)*(maxPosiNum+1));
    memset(NegaArr,0,sizeof(int)*(-minNegaNum+1));
    memset(NegaArr1,0,sizeof(int)*(-minNegaNum+1));
    for(int j=0;j<numsSize;j++){
        if (nums[j]>=0) {
            (PosiArr[nums[j]]>0)?(PosiArr1[nums[j]]=j+1):(PosiArr[nums[j]]=j+1);
        }
        else{
            (NegaArr[-nums[j]]>0)?(NegaArr1[-nums[j]]=j+1):(NegaArr[-nums[j]]=j+1);
        }
    }
    int lookforNum=0;
    for(int k=0;k<numsSize;k++){
        lookforNum=target-nums[k];
        if(lookforNum>=minNegaNum&&lookforNum<=maxPosiNum){
            if(lookforNum>=0&&PosiArr[lookforNum]>0&&lookforNum!=nums[k]){
                returnSize[0]=k;
                returnSize[1]=PosiArr[lookforNum]-1;
                break;
            }
            else if(lookforNum<0&&NegaArr[-lookforNum]>0&&lookforNum!=nums[k]){
                returnSize[0]=k;
                returnSize[1]=NegaArr[-lookforNum]-1;
                break;
            }
            else if(lookforNum>=0&&PosiArr1[lookforNum]>0&&lookforNum==nums[k]){
                returnSize[0]=k;
                returnSize[1]=PosiArr1[lookforNum]-1;
                break;
            }
            else if(lookforNum<0&&NegaArr1[-lookforNum]>0&&lookforNum==nums[k]){
                returnSize[0]=k;
                returnSize[1]=NegaArr1[-lookforNum]-1;
                break;
            }
        }
    }
    return returnSize;
}

int* twoSum(int* nums, int numsSize, int target) {
    int i,j;
    for (i=0;i<numsSize;i++)
        for (j=i+1;j<numsSize;j++) {
            if (nums[i]+nums[j] == target) {
                int *a = (int *)malloc(sizeof(int)*2);
                a[0] = i;
                a[1] = j;
                return a;
            }
        }
    return NULL;
}



//167. Two Sum II - Input array is sorted 两数之和之二输入数组有序
//只需要两个指针，一个指向开头，一个指向末尾，然后向中间遍历，如果指向的两个数相加正好等于target的话，直接返回两个指针的位置即可，若小于target，左指针右移一位，若大于target，右指针左移一位，以此类推直至两个指针相遇停止
//timeO(n)
int* twoSum(int* numbers, int numbersSize, int target, int* returnSize) {
    *returnSize = 0;
    if (!numbers || numbersSize < 1)
        return NULL;

    int i=0, j=numbersSize-1;
    while (i<j) {
        int sum = numbers[i] + numbers[j];
        if (sum > target) {
            j--;
        } else if (sum < target) {
            i++;
        } else {
            *returnSize = 2;
            int *arr = (int *)malloc(sizeof(int) * 2);
            arr[0] = i+1;
            arr[1] = j+1;
            return arr;
        }
    }

    return NULL;
}

//15. 3Sum
//排序之后，我们就可以对数组用两个指针分别从前后两端向中间扫描了，如果是 2Sum，我们找到两个指针之和为target就OK了，那 3Sum 类似，我们可以先固定一个数，然后找另外两个数之和为第一个数的相反数就可以了
static int cmp(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}
int** threeSum(int* nums, int numsSize, int* returnSize) {
    int **ret = NULL;
    int total = 64;
    int size = 0;
    int i,start,end;
    int sum;
    
    ret = (int **)malloc(sizeof(int *) * total);
    for(i = 0; i < total; ++i)
        ret[i] = (int *)malloc(sizeof(int) * 3);
    
    qsort(nums,numsSize,sizeof(int),cmp);
    
    for(i = 0; i < numsSize-2; ++i){
        if(i > 0 && nums[i] == nums[i-1])
            continue;
    
        start = i + 1;
        end = numsSize - 1;
        while(start < end){
            sum = nums[i] + nums[start] + nums[end];
            if(sum == 0){
                if(size > 0 && ret[size-1][0] == nums[i] && 
                    ret[size-1][1] == nums[start] && ret[size-1][2] == nums[end]){
                    ++start;
                    --end;
                    continue;
                }
                ret[size][0] = nums[i];
                ret[size][1] = nums[start];
                ret[size][2] = nums[end];
                size++;
                
                if(size == total){
                    total <<= 1;
                    int t;
                    ret = (int **)realloc(ret,sizeof(int *) * total);
                    for(t = size; t < total; ++t)
                        ret[t] = (int *)malloc(sizeof(int) * 3);
                }
                ++start;
                --end;
            } else if(sum > 0){
                --end;
            }else{
                ++start;
            }
        } // end while(start < end)
    } // end for(i = 0; i < numsSize-2; ++i)
    
    *returnSize = size;
    return ret;    
}

//18. 4Sum
//K-sum问题最好也就能做到O(n^(K-1))复杂度
//先排序，退化为3sum，以此类推。再去重
int comp(const void* p, const void* q){ 
    return (*(int*) p - *(int*) q); 
}

int** fourSum(int* nums, int numsSize, int target, int* returnSize) {
    if(nums ==NULL)	return 0;
    if(numsSize <4) return 0;
    int **Ans = (int**)malloc(200*sizeof(int*));
  	qsort(nums,numsSize,sizeof(int),comp);
  	int a,b,c,d,count=0;;
  	for(a=0; a < numsSize-3;a++){
  		// check [a,x,x,x] mininum
  		if(nums[a]+nums[a+1]+nums[a+2]+nums[a+3]>target)	
  			break;
  		// check [a,x,x,x] maxinum
  		if(nums[a]+nums[numsSize-3]+nums[numsSize-2]+nums[numsSize-1]<target)
  			continue;
  		for(b=a+1;b<numsSize-2;b++){
  			// check [a,b,x,x] mininu
  			if(nums[a]+nums[b]+nums[b+1]+nums[b+2]>target)
  				break;  	
  			// check [a,b,x,x] maxinum			
  			if(nums[a]+nums[b]+nums[numsSize-2]+nums[numsSize-1]<target)
  				continue;
  			for(c=b+1,d = (numsSize-1);c<d;){
  				if(nums[a]+nums[b]+nums[c]+nums[d]>target){
  					d--;
  				}
  				else if(nums[a]+nums[b]+nums[c]+nums[d]<target){
  					c++;
  				}else{
					Ans[count] = (int*)malloc(4*sizeof(int));
					Ans[count][0]=nums[a];
					Ans[count][1]=nums[b];
					Ans[count][2]=nums[c];
					Ans[count][3]=nums[d];
					while(c<d && Ans[count][2]==nums[c]) c++;	
					while(c<d && Ans[count][3]==nums[d]) d--;			
					count++;
					if(count % 200 == 0)
					    Ans = (int**)realloc(Ans,sizeof(int*)*(count+200));
  				}
  			}
  			while(b<numsSize-1 && nums[b+1] == nums[b]) b++;	
  		}
  		while(a<numsSize-1 && nums[a+1] == nums[a]) a++;		
  	}
  	*returnSize = count;
  	return (int**)realloc(Ans,sizeof(int*)*count);
}




//121. Best Time to Buy and Sell Stock
//用一个数组表示股票每天的价格，数组的第i个数表示股票在第i天的价格。 如果只允许进行一次交易，也就是说只允许买一支股票并卖掉，求最大的收益
//动态规划法。从前向后遍历数组，记录当前出现过的最低价格，作为买入价格，并计算以当天价格出售的收益，作为可能的最大收益，整个遍历过程中，出现过的最大收益就是所求
//数组中相邻两元素最大差值（后减前）
//时间O(n)，空间O(1)
int maxProfit(int* prices, int pricesSize) {
    int ret = 0, past = 0;
    for(int i = 1; i < pricesSize; i++)
    {
	    ret += prices[i] - prices[i-1];
	    if(ret < 0) ret = 0;
	    if(ret > past) past = ret;
    }
    return past;    
}


//122. Best Time to Buy and Sell Stock II
//用一个数组表示股票每天的价格，数组的第i个数表示股票在第i天的价格。交易次数不限，但一次只能交易一支股票，也就是说手上最多只能持有一支股票，求最大收益
//数组中非相邻两元素最大差值（后减前）
//贪心法。从前向后遍历数组，只要当天的价格高于前一天的价格，就算入收益
//时间O(n)，空间O(1)
int maxProfit(int* prices, int pricesSize) {
    int profit = 0, i;
    int tmp;
    for (i=0; i<pricesSize-1; i++) {
        tmp = prices[i+1] - prices[i];
        if (tmp > 0)
            profit += tmp;
    }
    return profit;    
}

//123. Best Time to Buy and Sell Stock III
//用一个数组表示股票每天的价格，数组的第i个数表示股票在第i天的价格。最多交易两次，手上最多只能持有一支股票，求最大收益
//动态规划法。以第i天为分界线，计算第i天之前进行一次交易的最大收益preProfit[i]，和第i天之后进行一次交易的最大收益postProfit[i]，最后遍历一遍，max{preProfit[i] + postProfit[i]} (0≤i≤n-1)就是最大收益。第i天之前和第i天之后进行一次的最大收益求法同Best Time to Buy and Sell Stock I
#define MAX(x, y) (x) > (y) ? (x) : (y)
int maxProfit(int* prices, int pricesSize) {
    int k = 2, i, j; //最多交易K次
    int *s = (int *)malloc(sizeof(int)*(k+1));
    int *b = (int *)malloc(sizeof(int)*(k+1));
    for (i=0; i<k+1; i++) {
        b[i] = INT_MIN;
        s[i] = 0;
    }
    for (i=0; i<pricesSize; i++)
        for (j=k; j>=1; j--) {
            s[j] = MAX(s[j], b[j]+prices[i]);
            b[j] = MAX(b[j], s[j-1]-prices[i]);
        }
    return s[k];
}

//188. Best Time to Buy and Sell Stock IV
//用一个数组表示股票每天的价格，数组的第i个数表示股票在第i天的价格。最多交易k次，手上最多只能持有一支股票，求最大收益
int maxProfit(int k, int* prices, int pricesSize) {
    int i, j;
    int *s = (int *)malloc(sizeof(int)*(k+1));
    int *b = (int *)malloc(sizeof(int)*(k+1));
    for (i=0; i<k+1; i++) {
        b[i] = INT_MIN;
        s[i] = 0;
    }
    for (i=0; i<pricesSize; i++)
        for (j=k; j>=1; j--) {
            s[j] = MAX(s[j], b[j]+prices[i]);
            b[j] = MAX(b[j], s[j-1]-prices[i]);
        }
    return s[k];
}

//169. Majority Element 查找数组的多数元素
//多数元素为数组中出现次数多于n/2的元素。假设数组非空且多数元素一定存在
int majorityElement(int* nums, int numsSize) {
      int cnt = 0, res;
    for (int i = 0; i < numsSize; ++i) {
        if (cnt == 0) res = nums[i];
        if (res == nums[i]) ++cnt;
        else --cnt;
    }
    return res;
}

//229. Majority Element II 
//给定一个大小为n的正整数数组，求出出现了超过n/3次的元素。要求时间复杂度为o(n)，空间复杂度为o(1)
//给出的答案有问题
int* majorityElement(int* nums, int numsSize, int* returnSize)
{
    if (NULL == nums || numsSize < 1)
        return NULL;
    if (1 == numsSize) {
        int *a = (int *)malloc (sizeof(int));
        a[0] = nums[0];
        *returnSize = 1;
        return a;
    }

    int  m1 = nums[0], m2 = nums[0], c1 = 0, c2 = 0, i;
    for (i=0; i<numsSize; i++) {
        if (nums[i] == m1)
            ++c1;
        else if (nums[i]==m2)
            ++c2;
        else if (0 == c1) {
            m1 = nums[i];
            c1++;
        } else if (0 == c2) {
            m2 = nums[i];
            c2++;
        } else {
            --c1;
            --c2;
        }
    }
    c1 = c2 = 0;
    for (i=0; i<numsSize; i++)
        if (m1 == nums[i])
            ++c1;
        else if (m2 == nums[i])
            ++c2;

    if (c1>numsSize/3 && c2>numsSize/3) {
        int *a = (int *)malloc(sizeof(int)*2);
        a[0] = m1;
        a[1] = m2;
        *returnSize = 2;
        return a;
    } else if (c1>numsSize/3) {
        int *a = (int *)malloc(sizeof(int));
        a[0] = m1;
        *returnSize = 1;
        return a;
    }

    return NULL;
}





//217. Contains Duplicate 判断数组内是否有重复元素
//hashtable
typedef struct hashlist {
     int val;
     struct hashlist *next;
}list;
bool containsDuplicate(int* nums, int numsSize) {
    if (NULL == nums || numsSize < 1)
        return false;
	int i;
	int mask = numsSize - 1;
	list *tmp[numsSize];
	memset(tmp, 0, sizeof(list*)*numsSize);
	if(numsSize == 0 || numsSize == 1) {
		return false;
	}
	for(i=0; i < numsSize; i++) {
		int idx;
		list *p;
		list *node = malloc(sizeof(list));
		node->val = *nums;
		idx = abs(*nums) % mask;	
		p = tmp[idx];
		node->next = p;
		tmp[idx] = node;
		while(p) {
			if (p->val == node->val) {
				return true;
			}
			p = p->next;
		}
		nums++;
	}
	return false;
}
//快排，比较
int comp (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}
bool containsDuplicate(int* nums, int numsSize) {
    // Sort
    qsort(nums, numsSize, sizeof(int), comp);
    
    // Loop
    for (int i = 0; i < numsSize - 1; i++) {
        if (nums[i] == nums[i+1]) return true;
    }
    return false;
}

//219. Contains Duplicate II 判断数组内是否有重复元素之二
//给出一个整数数组，判断该数组内是否有两个元素值是相同的，且他们的索引值相差不大于k，是则返回true，否则返回false
bool containsNearbyDuplicate(int* nums, int numsSize, int k) {
    if(numsSize ==0) return false;
    if(k>numsSize) k= k% numsSize;
    int i, min= INT_MAX, max = INT_MIN;
    for(i=0; i<numsSize; i++){
        min = min< nums[i]? min: nums[i];
        max = max> nums[i]? max: nums[i];
    }
    int *hash = malloc((max-min+1)* sizeof(int));
    memset(hash, -1, (max-min+1)*sizeof(int));
    for(i=0; i<numsSize; i++){
        if(hash[nums[i]-min] == -1){
            hash[nums[i]-min] = i;
        }
        else if((i-hash[nums[i]-min]) <= k){
            return true;
        }
        else{
            hash[nums[i]-min] = i;
        }
    }
    return false;    
}

//220. Contains Duplicate III


//287. Find the Duplicate Number
//给了n+1个数，所有的数都在[1, n]区域内,找重复
//利用坐标和数值之间相互转换，而由于重复数字的存在，那么一定会形成环，我们用快慢指针可以找到环并确定环的起始位置
//不能改变原数组，即不能给原数组排序，又不能用多余空间
//time O(n) space O(1)
int findDuplicate(int* nums, int numsSize) {
    if (NULL == nums || numsSize < 1)
        return 0;
    int fast=0, slow=0, i=0;

    while (1) {
        slow = nums[slow];
        fast = nums[nums[fast]];
        if (slow == fast)
            break;
    }
    while (1) {
        slow = nums[slow];
        i = nums[i];
        if (slow == i)
            return slow;
    }
    return 0;
}
//二分搜索法了，我们在区别[1, n]中搜索，首先求出中点mid，然后遍历整个数组，统计所有小于等于mid的数的个数，如果个数大于mid，则说明重复值在[mid+1, n]之间，反之，重复值应在[1, mid-1]之间，然后依次类推，直到搜索完成，此时的low就是我们要求的重复值
//time O(nlogn) space O(1)
int findDuplicate(int* nums, int numsSize) {
    if (NULL == nums || numsSize < 1)
        return 0;
    int l=1, h=numsSize-1;
    while(l < h)
    {
        int m = (l+h)/2;
        int count = 0;
        for(int i = 0; i < numsSize; i++)
            if(nums[i]>=l && nums[i]<=m) count++;
        if(count <= m-l+1) l = m+1;
        else h = m;
    }
    return l;
}



//442. Find All Duplicates in an Array
//数组中的数字可能出现一次或两次，找出所有出现两次的数字
//这类问题的核心是就是找nums[i]和nums[nums[i] - 1]的关系，我们的做法是，对于每个nums[i]，我们将其对应的nums[nums[i] - 1]取相反数，如果其已经是负数了，说明之前存在过，我们将其加入结果res中即可
typedef struct node {
    int data;
    struct node *next;
}Node;
int* findDuplicates(int* nums, int numsSize, int* returnSize) {
    if (NULL == nums || NULL == returnSize || numsSize < 1)
        return NULL;
    int i, n=0, absd=0;
    Node node;
    node.next = NULL;
    Node *p = &node;
    for (i=0; i<numsSize; i++) {
        absd = nums[i]>0 ? nums[i] : -nums[i];
        if (nums[absd-1] < 0) {
            Node *tmp = (Node *)malloc(sizeof(Node));
            if (NULL == tmp)
                return NULL;
            tmp->data = absd;
            tmp->next = NULL;
            p->next = tmp;
            p = tmp;
            n++;
        } else {
            nums[absd-1] = -nums[absd-1];
        }
	}
    if (n > 0) {
        int *arr = (int *)malloc(sizeof(int)*n);
        if (NULL == arr)
            return NULL;
        *returnSize = n;
        p = node.next;
        for (i=0; i<n; i++) {
            arr[i] = p->data;
            p = p->next;
        }
        return arr;
        
    }
    return NULL;
}

//将nums[i]置换到其对应的位置nums[nums[i]-1]上去，如果nums[i]和i+1不等，那么我们将nums[i]存入结果中即可
int* findDuplicates(int* nums, int numsSize, int* returnSize) {
    if (NULL == nums || NULL == returnSize || numsSize < 1)
        return NULL;
    int i, n;
    for (i=0; i<numsSize; i++) {
        if (nums[i] != nums[nums[i]-1]) {
            swap(&nums[i], &nums[nums[i]-1]);
            --i;
        }
    }

    int *arr = (int *)malloc(sizeof(int)*numsSize);
    if (NULL == arr)
        return NULL;
    for (i=0,n=0; i<numsSize; i++) {
        if (nums[i] != i+1)
            arr[n++] = nums[i];
    }
    *returnSize = n;
    return arr;
}


//268. Missing Number
//给出一个包含了n个不同数字的数组，从0开始一直到n，找出缺失的数字。如果数组是连续的则返回n+1
//time O(n),space O(1)
//用等差数列的求和公式求出0到n之间所有的数字之和，然后再遍历数组算出给定数字的累积和，然后做减法，差值就是丢失的那个数字
int missingNumber(int* nums, int numsSize) {
    if (NULL == nums || numsSize < 1)
        return -1;
    int i, sum1=0, sum2=0;
    for (i=1; i<=numsSize; i++)
        sum1 += i;
    for (i=0; i<numsSize; i++)
        sum2 += nums[i];
    return sum1-sum2;
}
//既然0到n之间少了一个数，我们将这个少了一个数的数组合0到n之间完整的数组异或一下，那么相同的数字都变为0了，剩下的就是少了的那个数字了
int missingNumber(int* nums, int numsSize) {
    int n = numsSize;
    int i;
    for (i=0;i<numsSize;i++) {
        n = n ^ i ^ nums[i];
    }
    return n;
}

//若已排序，二分查找比较好
int missingNumber(int* nums, int numsSize) {
    int left = 0, right = numsSize;
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] > mid) right = mid;
        else left = mid + 1;
    }
    return right;
}

//35. Search Insert Position
//已排序数组，某个值在数组中就给下标，不在则给出即将插入的下标
//二分查找。思路就是每次取中间，如果等于目标即返回，否则根据大小关系切去一半
//O(logn)，空间复杂度O(1)
int searchInsert(int* nums, int numsSize, int target) {
    int low=0, high=numsSize-1;
    int mid;
    while (low <= high) {
        mid = low + ((high-low)>>1);
        if (target > nums[mid]) {
            low = mid+1;
        } else if (target < nums[mid]) {
            high = mid-1;
        }else
            return mid;
    }
    return low;    
}

//53. Maximum Subarray 最大子串和
//对于一个包含负值的数字串array[1...n]，要找到他的一个子串array[i...j]（0<=i<=j<=n），使得在array的所有子串中，array[i...j]的和最大
//动态规划DP,最大子序列和是连续的子序列
//ThisSum[i]表示第i处，以A[i]结尾的子序列的最大和。
//则状态方程为ThisSum[i]=max(ThisSum[i-1]+nums[i],nums[i])
//time O(n)
#define MAX(x,y) (x)>(y)?(x):(y)
int maxSubArray(int* nums, int numsSize) {
    int sum=nums[0], max=nums[0];
    int i;
    for (i=1; i<numsSize; i++) {
        sum = MAX(sum+nums[i], nums[i]);
        max = MAX(max, sum);
    }
    return max;
}

//分治法Divide and Conquer Approach来解，这个分治法的思想就类似于二分搜索法，我们需要把数组一分为二，分别找出左边和右边的最大子数组之和，然后还要从中间开始向左右分别扫描，求出的最大值分别和左右两边得出的最大值相比较取最大的那一个
int maxSubArray(int* nums, int numsSize) {
    if (NULL == nums || numsSize < 1)
        return 0;
    return helper(nums, 0, numsSize-1);
}
int helper(int *nums, int left, int right) {
    if (left >= right) return nums[left];
    int mid = left + (right - left) / 2;
    int lmax = helper(nums, left, mid - 1);
    int rmax = helper(nums, mid + 1, right);
    int mmax = nums[mid], t = mmax;
    int i;
    for (i = mid - 1; i >= left; --i) {
        t += nums[i];
        mmax = MAX(mmax, t);
    }
    t = mmax;
    for (int i = mid + 1; i <= right; ++i) {
        t += nums[i];
        mmax = MAX(mmax, t);
    }
    return MAX(mmax, MAX(lmax, rmax));
}

//27. Remove Element 移除给定的值返回新长度
int removeElement(int* nums, int numsSize, int val) {
    int pos=0, i;
    for (i=0; i<numsSize; i++) {
        if (nums[i] != val)
            nums[pos++] = nums[i];
    }
    return pos;
}

//66. Plus One 数字加一
//给出一个由整型数组表示的非负数字，将这个数字加一。数组中的各元素表示该数字的各位，数组小标由小到大表示高位到低位
int* plusOne(int* digits, int digitsSize, int* returnSize) {
    int i;
    for (i=digitsSize-1; i>=0; i--) {
        if(9 == digits[i]) {
            digits[i] = 0;
        } else {
            digits[i]++;
            *returnSize = digitsSize;
            return digits;
        }
    }
    int *b = (int *)malloc(sizeof(int)*(digitsSize+1));
    for(int i = 1; i < (digitsSize+1); i++)
    {
        b[i] = digits[i-1];
    }
    b[0] = 1;
    *returnSize = digitsSize+1;
    return b;
}

//118. Pascal's Triangle 帕斯卡杨辉三角
int** generate(int numRows, int** columnSizes) {
    if (numRows < 1 || NULL == columnSizes)
        return NULL;

    int i, j;
    int **arr = NULL;
    *columnSizes = (int *)malloc(sizeof(int) * numRows);
    if (NULL == *columnSizes)
        return NULL;
    arr = (int **)malloc(sizeof(int *) * numRows);
    if (NULL == arr)
        return NULL;
    for (i=0; i<numRows; i++) {
        *(*columnSizes+i) = i+1;
        *(arr+i) = (int *)malloc(sizeof(int) * (i+1));
        for (j=0; j<=i; j++) {
            if (0==j || i==j)
                *(*(arr+i)+j) = 1;
            else
                *(*(arr+i)+j) = *(*(arr+i-1)+j-1) + *(*(arr+i-1)+j);
        }
    }
    return arr;    
}

//119. Pascal's Triangle II 求第k层元素（下标从0开始，0,1,2,3 k=3）
int* getRow(int rowIndex, int* returnSize) {
    if (rowIndex < 0)
        return NULL;
    int *res = (int *)malloc(sizeof(int) * (rowIndex+1));
    if (NULL == res)
        return NULL;
    int i, j;
    for (i=0; i<=rowIndex; i++)
        for (j=i; j>=0; j--)
            res[j] = (0==j || i==j)?1:res[j]+res[j-1];
    *returnSize = rowIndex+1;
    return res;
}

//26. Remove Duplicates from Sorted Array 
//给定一个有序数组，原地删除重复元素，使得每个元素只出现一次，并返回新的长度
//time O(n), space O(1)
int removeDuplicates(int* nums, int numsSize) {
        if(numsSize < 2) return numsSize;
        int id = 1, i;
        for(i = 1; i < numsSize; ++i) 
            if(nums[i] != nums[i-1]) nums[id++] = nums[i];
        return id;    
}

//80. Remove Duplicates from Sorted Array II
//给定一个有序数组，原地删除重复元素，使得每个元素最多出现2次，并返回新的长度
int removeDuplicates(int* nums, int numsSize) {
    if (NULL == nums || numsSize < 1)
        return 0;

    int i, n=0, count=0;
    for (i=0; i<numsSize; i++) {
        if (0 == i || nums[i] != nums[i-1])
            count = 1;
        else
            count++;
        if (count <= 2)
            nums[n++] = nums[i];
    }

    return n;    
}

//88. Merge Sorted Array 合并两个有序数组
//两个有序数组，合并成一个有序数组，假设第一个数组空间足够容纳两个数组
//从后往前合并
void merge(int* nums1, int m, int* nums2, int n) {
    int i = m-1;
    int j = n-1;
    int k = m+n-1;
    while (i>=0 && j>=0) {
        if (nums1[i] >= nums2[j])
            nums1[k--] = nums1[i--];
        else
            nums1[k--] = nums2[j--];
    }
    while (j>=0) {
        nums1[k--] = nums2[j--];
    }
}

//532. K-diff Pairs in an Array
//给定含有重复数字的无序数组，找出有多少对不重复的数对(i, j)使得i和j的差刚好为k
//使用了双指针，需要给数组排序，节省了空间的同时牺牲了时间。我们遍历排序后的数组，然后在当前数字之后找第一个和当前数之差不小于k的数字，若这个数字和当前数字之差正好为k，那么结果res自增1，然后遍历后面的数字去掉重复数字
#define MAX(x,y) (x)>(y)?(x):(y)
int comp (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}
int findPairs(int* nums, int numsSize, int k) {
    if (NULL == nums || numsSize < 1 || k < 0)
        return 0;
        
    int ans = 0, i, j;
    qsort(nums, numsSize, sizeof(int), comp);

    for (i=0, j=0; i<numsSize; i++) {
        j=MAX(j, i+1);
        while (j<numsSize && nums[j]-nums[i]<k)
            ++j;
        if (j<numsSize && nums[j]-nums[i]==k)
            ans++;
        while (i<numsSize-1 && nums[i] == nums[i+1])
            i++;
    }
    return ans;
}

//hash表，建立每个数字和其出现次数之间的映射，然后遍历哈希表中的数字，如果k为0且该数字出现的次数大于1，则结果res自增1；如果k不为0，且用当前数字加上k后得到的新数字也在数组中存在，则结果res自增1


//414. Third Maximum Number
//求数组中第三大的数，如果不存在的话那么就返回最大的数.第三大不能和第二大相同，必须是严格的小于，而并非小于等于
//用三个变量first, second, third来分别保存第一大，第二大，和第三大的数，然后我们遍历数组，如果遍历到的数字大于当前第一大的数first，那么三个变量各自错位赋值，如果当前数字大于second，小于first，那么就更新second和third，如果当前数字大于third，小于second，那就只更新third
//初始化要用长整型long的最小值，否则当数组中有INT_MIN存在时，程序就不知道该返回INT_MIN还是最大值first了
void swap(long *l, long *r)
{
    long tmp = *l;
    *l = *r;
    *r = tmp;
}
int thirdMax(int* nums, int numsSize) {
    long max1, max2, max3;
    int i;
    for (max1 = max2 = max3 = LONG_MIN, i = 0; i < numsSize; i++) {
        if (max3 >= nums[i] || max1 == nums[i] || max2 == nums[i])
            continue;
        max3 = nums[i];
        if (max2 < max3)
            swap(&max2, &max3);
        if (max1 < max2)
            swap(&max1, &max2);
    }
    return max3 == LONG_MIN ? max1 : max3;
}

//189. Rotate Array 旋转数组
//将一个数组右移k步  with len = 7 and k = 3, the array [1,2,3,4,5,6,7] is rotated to [5,6,7,1,2,3,4]
//time O(n) space O(1)
void reverse(int *nums, int n, int m)
{
    while (n < m) {
        nums[n] ^= nums[m];
        nums[m] ^= nums[n];
        nums[n] ^= nums[m];
        n++;
        m--;
    }
}
void rotate(int* nums, int numsSize, int k) {
    if (NULL == nums || numsSize < 1)
        return;
    int step = k%numsSize;
    reverse(nums, 0, numsSize-1);
    reverse(nums, 0, step-1);
    reverse(nums, step, numsSize-1);
}

//time O(n) space O(n)
void rotate(int* nums, int numsSize, int k) {
    int n = numsSize-k%numsSize;
    int i;
    int *b = (int *)malloc(sizeof(int)*numsSize);
    
    for (i=0; i<numsSize; i++) {
        n %= numsSize;
        b[i] = nums[n++];
    }

    for (i=0; i<numsSize; i++)
        nums[i] = b[i];

    free(b);
}

//495. Teemo Attacking
//给定一组递增的时间起点timeSeries，以及一个时间段duration，timeSeries中的每个起点st对应的终点ed = st + duration,求各时间段覆盖的时间总长度
//使用贪心算法，比较相邻两个时间点的时间差，如果小于duration，就加上这个差，如果大于或等于，就加上duration即可
int findPoisonedDuration(int* timeSeries, int timeSeriesSize, int duration) {
    if(NULL == timeSeries || timeSeriesSize < 1)
        return 0;
    int i, sum=0;
    for (i=1; i<timeSeriesSize; i++)
        if (timeSeries[i]-timeSeries[i-1] < duration)
            sum += timeSeries[i]-timeSeries[i-1];
        else
            sum += duration;

    return sum+duration;
    
}

//238. Product of Array Except Self
//给定一个数组，对于每一个位置上的数是其他位置上数的乘积,返回一个新数组
//time O(n) space O(1)
int* productExceptSelf(int* nums, int numsSize, int* returnSize) {
    if (NULL == nums || numsSize < 1 || NULL == returnSize)
        return NULL;

    int *a = (int *)malloc(sizeof(int)*numsSize);
    if (NULL == a)
        return NULL;
    int i, right=1;
    a[0]=1;
    for (i=1; i<numsSize; i++)
        a[i] = a[i-1] * nums[i-1];//每个位子上存之前的乘积
    for (i=numsSize-1; i>=0; i--) {
        a[i] *= right;
        right *= nums[i]; //每个位子上乘后面所有数字之积
    }
    *returnSize = numsSize;
    return a;  
}

//39. Combination Sum
//给定一个数组没有重复的数，但一个数可以用多次.从中找出一组数来，使其和等于target。数组无序，但都是正整数.要求返回结果中没有重复的解，且每个解中的数都按非递减排好序
//回溯。先对数组进行排序，然后从小到大累加，等于或超过target时回溯


//40. Combination Sum II 
//数组里有重复，一个数只能用一次.要求返回结果中没有重复的解，且每个解中的数都按非递减排好序


//216. Combination Sum III

//62. Unique Paths
//数组 A[m][n] ，从 A[0][0] 到 A[m-1][n-1] 有多少条路径
//动态规划
int uniquePaths(int m, int n) {
    int DP[100][100];
    int i,j;
    for(i = 0; i < n; ++i) DP[i][0] = 1;
    for(j = 0; j < m; ++j) DP[0][j] = 1;
    
    for(i = 1; i < n; ++i)
    {
        for(int j = 1; j < m; ++j)
        {
            DP[i][j] = DP[i - 1][j] + DP[i][j - 1];
        }
    }
    
    return DP[n - 1][m - 1];    
}

int uniquePaths(int m, int n) {
    int i, j;
    int *a = (int *)malloc(sizeof(int)*n);

    for (j = 0; j < n; j++) {  
        a[j] = 1;  
    }  
    for (i = 1; i < m; i++) {  
        for (int j = 1; j < n; j++) {  
            a[j] += a[j-1];  
        }  
    }  
    return a[n-1]; 
}

//63. Unique Paths II
//数组 A[m][n] ，从 A[0][0] 到 A[m-1][n-1] 有多少条路径，A[m][n] 中可能存在障碍，即A[i][j] = 1 时
int uniquePathsWithObstacles(int** obstacleGrid, int obstacleGridRowSize, int obstacleGridColSize) {
    int m = obstacleGridRowSize, n = obstacleGridColSize;
    
    //flip upper left cell (the start cell): 1 => 0 or 0 => 1
    obstacleGrid[0][0] ^= 1;
    
    //first row: if 1, then 0; otherwise, left cell
    for(int i = 1; i < n; i++)
        obstacleGrid[0][i] = obstacleGrid[0][i] == 1 ? 0 : obstacleGrid[0][i - 1];
    
    //first column: if 1, then 0; otherwise, top cell
    for(int i = 1; i < m; i++)
        obstacleGrid[i][0] = obstacleGrid[i][0] == 1 ? 0 : obstacleGrid[i - 1][0];
        
    //rest: if 1, then 0; otherwise, left cell + top cell
    for(int i = 1; i < m; i++)
        for(int j = 1; j < n; j++)
            obstacleGrid[i][j] = obstacleGrid[i][j] == 1 ? 0 : obstacleGrid[i - 1][j] + obstacleGrid[i][j - 1];
            
    //return lower right cell (the end cell)
    return obstacleGrid[m - 1][n - 1];
}


//54. Spiral Matrix 
int* spiralOrder(int** matrix, int m, int n) {
    if (m * n == 0) {
        return NULL;
    }

    int* spiral = (int*)malloc(sizeof(int) * m * n);
    int left = 0, right = n - 1, top = 0, bottom = m - 1, k = 0;
    for (; left < right && top < bottom; ++left, --right, ++top, --bottom) {
        for (int i = left; i < right; ++i, ++k) {
            spiral[k] = matrix[top][i];
        }
        for (int i = top; i < bottom; ++i, ++k) {
            spiral[k] = matrix[i][right];
        }
        for (int i = right; i > left; --i, ++k) {
            spiral[k] = matrix[bottom][i];
        }
        for (int i = bottom; i > top; --i, ++k) {
            spiral[k] = matrix[i][left];
        }
    }
    if (left == right) {
        for (int i = top; i <= bottom; ++i, ++k) {
            spiral[k] = matrix[i][left];
        }
    } else if (top == bottom) {
        for (int i = left; i <= right; ++i, ++k) {
            spiral[k] = matrix[top][i];
        }
    }
    return spiral;
}

//59. Spiral Matrix II 螺旋写入矩阵
int** generateMatrix(int n) {
    if (n == 0) {
        return NULL;
    }

    int** spiral = (int**)malloc(sizeof(int*) * n);
    for (int i = 0; i < n; ++i) {
        spiral[i] = (int*)malloc(sizeof(int) * n);
    }

    int low = 0, high = n - 1, k = 1;
    for (; low < high; ++low, --high) {
        for (int i = low; i < high; ++i, ++k) {
            spiral[low][i] = k;
        }
        for (int i = low; i < high; ++i, ++k) {
            spiral[i][high] = k;
        }
        for (int i = high; i > low; --i, ++k) {
            spiral[high][i] = k;
        }
        for (int i = high; i > low; --i, ++k) {
            spiral[i][low] = k;
        }
    }
    if (low == high) {
        spiral[low][low] = k;
    }
    return spiral;
}

//78. Subsets 求数组的所有子集
//给出一个数组生成该数组所有元素的组合,nums = [1,2,3],有[[3],[1],[2],[1,2,3],[1,3],[2,3],[1,2],[]]
int** subsets(int* nums, int numsSize, int** columnSizes, int* returnSize) {
    if (NULL == nums || NULL == columnSizes || NULL == returnSize|| numsSize < 1)
        return NULL;
    int n = pow(2,numsSize);
    int i, j, flag, step, count;
    int **ret = (int **)malloc(sizeof(int *)*n);
    if (NULL == ret)
        return NULL;
    *returnSize = n;

    *columnSizes = (int *)malloc(sizeof(int)*n);
    for (i=0; i<n; i++) {
        count = j = 0;
        flag = n>>1;
        while (flag) {
            if (flag & i)
                count++;
            flag = flag>>1;
        }
        *(*columnSizes+i) = count;
        *(ret+i) = (int *)malloc(sizeof(int)*count);
        if (0 == count)
            continue;

        flag = n>>1;
        step = numsSize-1;
        while (flag) {
            if (flag & i) {
                *(*(ret+i)+j) = nums[step];
                j++;
            }
            flag = flag>>1;
            step--;
        }
    }
    return ret;
}

//90. Subsets II
//给出一个数组生成该数组所有元素的组合,nums = [1,2,2],有[[2],[1],[1,2,2],[2,2],[1,2],[]]
int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

void subSets(int* n, int nS, int pos,
             int k, int subS, int* cur,
             int* colS, int* retS,
             int** ret)
{
  //the # of items in this subset is reached; push current list to the return list
  if(k == subS)
  {
    colS[*retS] = subS;
    if(subS > 0) memcpy(ret[*retS], cur, sizeof(int) * subS);
    (*retS)++;
  }

  //loop through the remaining in nums set
  for(int i = pos; i<nS; i++)
  {
    // skip the duplicate items
    if(i!=pos && n[i] == n[i-1])  continue;

    cur[k] = n[i];
    subSets(n, nS, i+1, k+1, subS, cur, colS, retS, ret);
  }
}

int** subsetsWithDup(int* n, int nS, int** colS, int* retS)
{
  if(nS == 0 || n==NULL)  return NULL;

  int worstCaseLen = pow(2,nS); // worst case no dup so 2^n
  int* cur = (int*)malloc(sizeof(int)*nS);
  int** ret = (int**)malloc(sizeof(int*)* worstCaseLen);  

  for(int i = 0; i<worstCaseLen; i++)
    ret[i] = (int*)malloc(sizeof(int)*nS);
  *retS = 0;
  *colS = (int*)malloc(sizeof(int) * worstCaseLen); 

  //sort first
  qsort(n, nS, sizeof(int), compare);
   
  // the length of subsets varies from 0, 1, ... , n-1, n
  // always start from index 0
  for(int i = 0; i <= nS; i++)
    subSets(n, nS, 0, 0, i, cur, *colS, retS, ret);
  
  return ret;
}


//48. Rotate Image
void rotate(int** matrix, int matrixRowSize, int matrixColSize) {\
/*
    if (matrixRowSize-matrixColSize)
        return;
    int i, j, n = matrixRowSize;
    for (int i=0; i<n/2; ++i)
    {
        for (int j=i; j<n-1-i; ++j)
        {
            int z = matrix[i][j];
            matrix[i][j] = matrix[n-j-1][i];
            matrix[n-j-1][i] = matrix[n-i-1][n-j-1];
            matrix[n-i-1][n-j-1] = matrix[j][n-i-1];
            matrix[j][n-i-1] = z;
        }
    }    
*/

    if (matrixRowSize - matrixColSize)
		return;

	int i, j, k;
	int t;
	int dis;
	//step1
	for (i = 0; i < matrixRowSize; ++i)
	{
		for (j = 0, k = matrixRowSize - 1; j < k; ++j, --k)
		{
			t = matrix[i][j];
			matrix[i][j] = matrix[i][k];
			matrix[i][k] = t;
		}
	}
	//step2
	for (i = 0; i < matrixRowSize-1; ++i)
	{
		for (j = 0; j < matrixRowSize - 1 - i; ++j)
		{
			dis = matrixRowSize - 1 - i - j;
			t = matrix[i][j];
			matrix[i][j] = matrix[i + dis][j + dis];
			matrix[i + dis][j + dis] = t;
		}
	}
}

//64. Minimum Path Sum
//在一个全为正整数的 m X n 的矩阵中， 取一条从左上为起点， 走到右下为重点的路径， （前进方向只能向左或者向右），求一条所经过元素和最小的一条路径
int minPathSum(int** grid, int rSize, int cSize)
{
    int *sums, pre;
    sums = (int*)malloc(sizeof(int)*cSize);
    sums[0] = grid[0][0];
    for(int i = 1; i < cSize; i++)
        sums[i] = sums[i-1] + grid[0][i];
    for(int r=1; r<rSize; r++)
    {
        for(int c=0; c<cSize; c++)
        {
            int min = sums[c];
            if(c && min > pre)
                min = pre;
            sums[c] = min+grid[r][c];
            pre = sums[c];
        }
    }
    return sums[cSize-1];
}

//75. Sort Colors 
//对一个包含0,1,2三种数字的数组重新排序，使得排好序的数组前一段都是0，中间一段都是1，最后一段都是2
void sortColors(int* nums, int numsSize) {
/*
    int second=numsSize-1, zero=0;
    for (int i=0; i<=second; i++) {
        while (nums[i]==2 && i<second) swap(&nums[i], &nums[second--]);
        while (nums[i]==0 && i>zero) swap(&nums[i], &nums[zero++]);
    }
*/
    int i, r, w, b;  
    r = w = b = 0;  
    for (i = 0; i < numsSize; i++) {  
        if (nums[i] == 0) r++;  
        else  if (nums[i] == 1) w++;  
        else b++;  
    }  
    for (i = 0; i < numsSize; i++) {  
        if (i < r) nums[i] = 0;  
        else if (i < r + w) nums[i] = 1;  
        else nums[i] = 2;  
    } 
}

//162. Find Peak Element 峰值
//给定一个相邻元素不相等的数组，找出其中的一个局部最大值，返回对应下标
//从第一个元素开始，若其大于相邻的后续元素，则第一个元素就是一个局部最大值，返回即可。若其小于相邻的后续元素，则第二个元素大于第一个元素。如此，一一遍历数组，第一次出现，第i个元素若大于其相邻后续元素，则该元素就是一个局部最大值，返回即可
int findPeakElement(int* nums, int numsSize) {
/*
//顺序查找 time O(n)
    if (NULL == nums || numsSize < 1)
        return -1;
    int i;
    for (i=0; i<numsSize-1; i++) {
        if (nums[i] < nums[i+1])
            return i;
    }
    return numsSize-1;
*/

//二分查找 time O(logn)
    int low=0, high=numsSize-1;
    while (low < high) {
        int mid1 = low + ((high-low)>>1);
        if (nums[mid1] < nums[mid1+1])
            low = mid1+1;
        else 
            high = mid1;
    }
    return low;   
}

//11. Container With Most Water
//在二维坐标系中，(i, ai) 表示 从 (i, 0) 到 (i, ai) 的一条线段，任意两条这样的线段和 x 轴组成一个木桶，找出能够盛水最多的木桶，返回其容积.用两个指针从两端开始向中间靠拢，如果左端线段短于右端，那么左端右移，反之右端左移，知道左右两端移到中间重合，记录这个过程中每一次组成木桶的容积，返回其中最大的.当左端线段L小于右端线段R时，我们把L右移，这时舍弃的是L与右端其他线段（R-1, R-2, ...）组成的木桶，这些木桶是没必要判断的，因为这些木桶的容积肯定都没有L和R组成的木桶容积大
int maxArea(int* height, int heightSize) {
    int l=0, r=heightSize-1;
    int max = 0;
    while(l < r)
    {
        int area = (r-l)*(height[l] < height[r]? height[l++] : height[r--]);
        max = max > area? max : area;
    }
    return max;    
}

//73. Set Matrix Zeroes
//- 先扫描第一行第一列，如果有0，则将各自的flag设置为true
//- 然后扫描除去第一行第一列的整个数组，如果有0，则将对应的第一行和第一列的数字赋0
//- 再次遍历除去第一行第一列的整个数组，如果对应的第一行和第一列的数字有一个为0，则将当前值赋0
//- 最后根据第一行第一列的flag来更新第一行第一列
//space O(1)
void setZeroes(int** matrix, int matrixRowSize, int matrixColSize) {
    int i, j;
    int rowflag=0, colflag=0;
    for (i=0; i<matrixRowSize; i++) {
        if (0 == matrix[i][0]) {
            rowflag = 1;
            break;
        }
    }
    for (j=0; j<matrixColSize; j++) {
        if (0 == matrix[0][j]) {
            colflag = 1;
            break;
        }
    }
    for (i=1; i<matrixRowSize; i++)
        for (j=1; j<matrixColSize; j++) {
            if (0 == matrix[i][j]) {
                matrix[i][0] = 0;
                matrix[0][j] = 0;
            }
        }

    for (i=1; i<matrixRowSize; i++)
        for (j=1; j<matrixColSize; j++) {
            if (0 == matrix[i][0] || 0 == matrix[0][j])
                matrix[i][j] = 0;
        }

    if (rowflag)
        for (i=0; i<matrixRowSize; i++)
            matrix[i][0] = 0;
    if (colflag)
        for (j=0; j<matrixColSize; j++)
            matrix[0][j] = 0;
}

//74. Search a 2D Matrix
//二分法确定target可能在第几行出现。再用二分法在该行确定target可能出现的位置。时间复杂度O(logn+logm)

bool searchMatrix(int** matrix, int matrixRowSize, int matrixColSize, int target) {
    if (NULL == matrix || matrixRowSize < 1 || matrixColSize < 1)
        return false;

    int low = 0, high = matrixRowSize - 1, mid=0, row;

    while (low <= high) {
        mid = low + ((high-low)>>1);
        if (matrix[mid][0] < target)
            low = mid + 1;
        else if (matrix[mid][0] > target)
            high = mid - 1;
        else
            return true;
    }
    
    if (matrix[mid][0] > target) {
        if (mid == 0)
            return false;
        mid--;
    }

    low = 0, high = matrixColSize-1, row = mid;
    while (low <= high) {
        int mid = low + ((high-low)>>1);
        if (matrix[row][mid] < target)
            low = mid+1;
        else if (matrix[row][mid] > target)
            high = mid-1;
        else
            return true;
    }
    return false;

}
//从右上角元素开始遍历，每次遍历中若与target相等则返回true；若小于则行向下移动；若大于则列向左移动。时间 O(m+n)
bool searchMatrix(int** matrix, int matrixRowSize, int matrixColSize, int target) {
    if (NULL == matrix || matrixRowSize < 1 || matrixColSize < 1)
        return false;

    int i = 0, j = matrixColSize - 1;

    while (i < matrixRowSize && j>= 0) {
        if (target == matrix[i][j])
            return true;
        else if (target < matrix[i][j])
            j--;
        else
            i++;
    }
    return false;
}

//120. Triangle
//给定一个三角形，求得和最小的路径。每层只能选一个整数，上一层和下一层的整数必须是相邻的
//1,动态规划。到第i层的第k个顶点的最小路径长度表示为f(i,k)，则f(i, k) = min{f(i-1,k),  f(i-1,k-1)} + d(i, k); 其中d(i, k)表示原来三角形数组里的第i行第k列的元素。则可以求得从第一行到最终到第length-1行第k个元素的最小路径长度，最后再比较第length-1行中所有元素的路径长度大小，求得最小值。
//2,本题主要关心的是空间复杂度不要超过n。
//3,注意边界条件——每一行中的第一和最后一个元素在上一行中只有一个邻居。而其他中间的元素在上一行中都有两个相邻元素
//修改了原始数组 space(1)
int minimumTotal(int** triangle, int triangleRowSize, int *triangleColSizes) {
    int i, j;
    for (i=triangleRowSize-2; i>=0; --i)
        for (j=0; j<triangleColSizes[i]; j++)
            triangle[i][j] += (triangle[i+1][j]<triangle[i+1][j+1]?triangle[i+1][j]:triangle[i+1][j+1]);
    return triangle[0][0];
}

//复制了三角形最后一行，作为用来更新的一位数组。然后逐个遍历这个DP数组，对于每个数字，和它之后的元素比较选择较小的再加上上面一行相邻位置的元素做为新的元素，然后一层一层的向上扫描，整个过程和冒泡排序的原理差不多，最后最小的元素都冒到前面，第一个元素即为所求
//不修改了原始数组 space(n)
int minimumTotal(int** triangle, int triangleRowSize, int *triangleColSizes) {


    int i, j;
    int *a = (int *)malloc(sizeof(int)*triangleColSizes[triangleRowSize-1]);
    if (NULL == a)
        return -1;
    for (i=0; i<triangleRowSize; i++)
        a[i] = triangle[triangleRowSize-1][i];
    for (i=triangleRowSize-2; i>=0; --i)
        for (j=0; j<triangleColSizes[i]; j++)
            a[j] = (a[j]<a[j+1]?a[j]:a[j+1]) + triangle[i][j];
    
    return a[0];
}

//33. Search in Rotated Sorted Array
//因为rotate的缘故，当我们切取一半的时候可能会出现误区，所以我们要做进一步的判断。具体来说，假设数组是A，每次左边缘为l，右边缘为r，还有中间位置是m。在每次迭代中，分三种情况：
//（1）如果target==A[m]，那么m就是我们要的结果，直接返回；
//（2）如果A[m]<A[r]，那么说明从m到r一定是有序的（没有受到rotate的影响），那么我们只需要判断target是不是在m到r之间，如果是则把左边缘移到m+1，否则就target在另一半，即把右边缘移到m-1。
//（3）如果A[m]>=A[r]，那么说明从l到m一定是有序的，同样只需要判断target是否在这个范围内，相应的移动边缘即可。

//根据以上方法，每次我们都可以切掉一半的数据，所以算法的时间复杂度是O(logn)，空间复杂度是O(1
int search(int* nums, int numsSize, int target) {
	int low = 0, high = numsSize - 1;
	while (low < high) {
		int m = (low + high) >> 1;
		if (target > nums[high])
			if (nums[m] < target && nums[m] > nums[high]) low = m + 1;
			else high = m;
		else
			if (nums[m] >= target && nums[m] <= nums[high]) high = m;
			else low = m + 1;
	}
	return nums[low] == target ? low : -1;    
}

//81. Search in Rotated Sorted Array II 数组有重复元素
//原来我们是依靠中间和边缘元素的大小关系，来判断哪一半是不受rotate影响，仍然有序的。而现在因为重复的出现，如果我们遇到中间和边缘相等的情况，我们就丢失了哪边有序的信息，因为哪边都有可能是有序的结果。假设原数组是{1,2,3,3,3,3,3}，那么旋转之后有可能是{3,3,3,3,3,1,2}，或者{3,1,2,3,3,3,3}，这样的我们判断左边缘和中心的时候都是3，如果我们要寻找1或者2，我们并不知道应该跳向哪一半。解决的办法只能是对边缘移动一步，直到边缘和中间不在相等或者相遇，这就导致了会有不能切去一半的可能。所以最坏情况（比如全部都是一个元素，或者只有一个元素不同于其他元素，而他就在最后一个）就会出现每次移动一步，总共是n步，算法的时间复杂度变成O(n
bool search(int* nums, int numsSize, int target) {
    int lo =0, hi = numsSize-1;
    int mid = 0;
    while(lo<hi){
          mid=(lo+hi)/2;
          if(nums[mid]==target) return true;
          if(nums[mid]>nums[hi]){
              if(nums[mid]>target && nums[lo] <= target) hi = mid;
              else lo = mid + 1;
          }else if(nums[mid] < nums[hi]){
              if(nums[mid]<target && nums[hi] >= target) lo = mid + 1;
              else hi = mid;
          }else{
              hi--;
          }
          
    }
    return nums[lo] == target ? true : false;
}


//105. Construct Binary Tree from Preorder and Inorder Traversal 
struct TreeNode* buildTree(int* preorder, int preorderSize, int* inorder, int inorderSize) {
    if (preorderSize == 0) return NULL;
    
    struct TreeNode* root = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    root->val = *preorder;
    
    int i = 0;
    while (inorder[i] != *preorder) ++i;
    root->left = buildTree(preorder + 1, i, inorder, i);
    root->right = buildTree(preorder + 1 + i, preorderSize - i - 1,
                            inorder + 1 + i, inorderSize - i - 1);
    
    return root;    
}


//106. Construct Binary Tree from Inorder and Postorder Traversal
struct TreeNode* buildTree(int* inorder, int inorderSize, int* postorder, int postorderSize) {
    if(inorderSize<=0 || postorderSize<=0)
        return NULL;
    
    struct TreeNode *root = malloc(sizeof (struct TreeNode));
    int maxIndex=postorderSize-1;
    int middleIndex=-1;
    
    //for (int i=0; i<inorderSize; i++){
    for (int i=inorderSize; i>=0; i--){
        if(*(inorder+i)==*(postorder+maxIndex)){
            middleIndex = i;
            break;
        }
    }
    
    root->val = *(postorder+maxIndex);
    root->left = NULL;
    root->right = NULL;
    
    root->left = buildTree(inorder, middleIndex, postorder, middleIndex);
    root->right = buildTree(inorder+middleIndex+1, inorderSize-1-middleIndex, postorder+middleIndex, postorderSize-middleIndex-1);
    
    return root;    
}

//34. Search for a Range 搜索特定数值的范围
//给定一个整型已排序数组，找到一个给定值在其中的起点与终点。 你的算法复杂度必须低于O(logn)。 如果目标在数组中不会被发现，返回[-1, -1]
//left:
//If A[mid] < target, then the range must begins on the right of mid (hence i = mid+1 for the next iteration)
//If A[mid] > target, it means the range must begins on the left of mid (j = mid-1)
//If A[mid] = target, then the range must begins on the left of or at mid (j= mid)
//right:

//If A[mid] > target, then the range must begins on the left of mid (j = mid-1)
//If A[mid] < target, then the range must begins on the right of mid (hence i = mid+1 for the next iteration)
//If A[mid] = target, then the range must begins on the right of or at mid (i= mid)
int* searchRange(int* nums, int numsSize, int target, int* returnSize) {
    int *a = (int *)malloc(sizeof(int)*2);
    a[0] = a[1] = -1;
    *returnSize = 2;
    int i=0, j=numsSize-1;
    // Search for the left one
    while (i < j) {
        int mid = i+((j-i)>>1);
        if (nums[mid] < target)
            i = mid+1;
        else
            j = mid;
    }
    if (nums[i] != target)
        return a;
    else
        a[0] = i;

    // Search for the right one
    j = numsSize-1;// We don't have to set i to 0 the second time
    while (i < j) {
        int mid = i+((j-i)>>1) + 1;// Make mid biased to the right
        if (nums[mid] > target)
            j = mid-1;
        else
            i = mid;// So that this won't make the search range stuck
    }
    a[1] = j;
    return a;
}

//16. 3Sum Closest 最近三数之和
//求最接近给定值的三数之和
#define ABS(x) ((x)<0?-(x):(x))

int comparefn( const void* a, const void* b)
{
     int int_a = * ( (int*) a );
     int int_b = * ( (int*) b );

     if ( int_a == int_b ) return 0;
     else if ( int_a < int_b ) return -1;
     else return 1;
}

int threeSumClosest(int* nums, int numsSize, int target) {
    // sort the array
    qsort(nums, numsSize, sizeof(int), comparefn);
    int i, j, k, diff, min_diff = INT_MAX;
    for(i = 0 ; i < numsSize ; i++)
    {
        j = i + 1;
        k = numsSize - 1;
        while(j < k)
        {
            diff = nums[i] + nums[j] + nums[k] - target;
            //temp = ABS(temp);
            if(diff == 0)
            {
                return target;
            }
            else
            {
                if(ABS(diff) < ABS(min_diff))
                {
                    min_diff = diff;
                }
                if(diff < 0)
                {// increase value
                    j++;
                }
                else//>0
                {
                    k--;
                }
            }
        }
    }
    return target + min_diff;
}


//209. Minimum Size Subarray Sum
//求子数组之和大于等于给定值的最小长度
//采用类似滑动窗口的形式，双指针start,end分别记录子数组的左右的边界位置，然后我们让end向右移，直到子数组和大于等于给定值或者end达到数组末尾，此时我们更新最短距离，并且将start像右移一位，然后再sum中减去移去的值，然后重复上面的步骤，直到end到达末尾，且start到达临界位置，即要么到达边界，要么再往右移动，和就会小于给定值
//O(n)
int minSubArrayLen(int s, int* nums, int numsSize) {
    int start=0, end=0, sum=0, min=INT_MAX;
    while (start < numsSize && end < numsSize) {
        while (sum < s && end < numsSize)
            sum += nums[end++];
        while (sum >= s && start <= end) {
            min = min<(end-start)?min:(end-start);
            sum -= nums[start++];
        }
    }
    return min==INT_MAX ? 0 : min;
    
}
//O(nlgn)
// 在不下降的序列中寻找恰好比target小的数出现位置，也即最后一个比target小的数出现的位置
// search a number that is exactly less than 'target', which means the last number less than 'target'
int binarySearchIncreaseLastSmaller(int l, int r, int target, int * nums) {  
	if (l >= r) return -1;
	while (l < r - 1) {
		int m = l + ((r - l) >> 1);
		if (nums[m] < target) l = m;
		else r = m - 1;
	}
	if (nums[r] < target) return r;
	else if (nums[l] < target) return l;
	else return -1;
}

int minSubArrayLen(int s, int * nums, int numsSize) {
	int * Sum = (int*)malloc(sizeof(int) * (numsSize + 1)), minL = numsSize + 1;
	Sum[0] = 0;
	for (int i = 1; i <= numsSize; i++) Sum[i] = Sum[i - 1] + nums[i - 1];
	for (int i = 1; i <= numsSize; i++) {
		if (Sum[i] >= s) {
			int k = Sum[i];
			int BeforePos = binarySearchIncreaseLastSmaller(0, i, Sum[i] - s + 1, Sum);
			if (BeforePos != -1 && i - BeforePos < minL) minL = i - BeforePos;
		}
	}
	return minL == numsSize + 1 ? 0 : minL;
}

//55. Jump Game
//给定一个数组，从第一个元素开始，元素的值表示能够往后跳的最大距离，问这样一个数组能否跳到最后一个元素
//贪心
//逆向思路
bool canJump(int* nums, int numsSize) {
    int i, last = numsSize - 1;
    for (i=numsSize-2; i>=0; i--)
        if (i+nums[i] >= last)
            last = i;
    return (last<=0);
}

//正向思路
bool canJump(int* nums, int numsSize) {
    int reach=0, i=0;
    for (; i<numsSize && i<=reach; i++)
        reach = reach>(i+nums[i]) ? reach : (i+nums[i]);
    return (i==numsSize);
}

//45. Jump Game II
//给出一个非负整数数组，你最初定位在数组的第一个位置。数组中的每个元素代表你在那个位置可以跳跃的最大长度。你的目标是使用最少的跳跃次数到达数组的最后一个位置
//贪心
int jump(int* nums, int numsSize) {
    int ret = 0;//当前跳数  
    int last = 0;//上一跳可达最远距离  
    int curr = 0;//当前一跳可达最远距  
    for (int i = 0; i < numsSize; ++i) {  
        //无法向前继跳直接返回  
        if(i>curr){  
            return -1;  
        }  
        //需要进行下次跳跃，则更新last和当执行的跳数ret  
        if (i > last) {  
            last = curr;  
            ++ret;  
        }  
        //记录当前可达的最远点  
        curr = curr>(nums[i]+i)?curr:(nums[i]+i);
    }  

    return ret;
}

//56. Merge Intervals
//有很多个区间，把有重叠的区间合并
//Given [1,3],[2,6],[8,10],[15,18],
//return [1,6],[8,10],[15,18]


//31. Next Permutation
//给定一个数组和一个排列，求下一个排列
void swap(int* p, int* q)
{
    int t = *p; *p = *q; *q = t;
}
void reverse(int* nums, int begin, int end)
{
    for(int i = begin; i < (begin+end+1)/2; i++)
        swap(nums+i, nums+end+begin-i);
}
void nextPermutation(int* nums, int size)
{
    int i=size-1, j=size-1;
    while(i>0 && nums[i]<=nums[i-1]) i--; //make sure the [i..size-1] is in descending order;
    if(i==0) //the whole array is descending now, reverse it to the smallest as problem requires;
    {
        reverse(nums, 0, size-1);
        return ;
    }
    while(nums[j] <= nums[i-1]) j--; //find the first bigger one backwards;
    swap(nums+j, nums+i-1); //ensure the next is bigger;
    reverse(nums, i, size-1); //since [i..size-1] is descending, after reverse it will be ascending and as a result - [i..size-1] will be the smallest - the smallest in the bigger results - the next permutation;
}

//79. Word Search
//给你一个二维字母的数组，可以上下左右走，查找是否某个单词是否存在。同一位置的字母不可以被使用多次
//类似于迷宫，递归回溯。需要一个辅助数组记录走过的位置，防止同一个位置被使用多次
bool isFound(char** board, int row, int col, char*word, int rIndex, int cIndex)
{
    if(*word == '\0') return true; //reach it end;
    if(rIndex>=row || cIndex>=col || rIndex<0 || cIndex<0 || *word!=board[rIndex][cIndex]) return false;
    char t = board[rIndex][cIndex];
    board[rIndex][cIndex] = '\0'; //avoid re-visiting;
    if(isFound(board, row, col, word+1, rIndex+1, cIndex) || 
       isFound(board, row, col, word+1, rIndex-1, cIndex) || 
       isFound(board, row, col, word+1, rIndex, cIndex+1) || 
       isFound(board, row, col, word+1, rIndex, cIndex-1)) 
             return true;
    board[rIndex][cIndex] = t; //restore to the original;
    return false; //if all failed, return false;
}

bool exist(char** board, int row, int col, char* word) 
{
    //Start from each position;
    for(int i = 0; i < row; i++)
        for(int j = 0; j < col; j++)
            if(isFound(board, row, col, word, i, j))
                return true;
    return false;
}

//152. Maximum Product Subarray
//求一个数组，连续子数组的最大乘积
//跟Maximum Subarray模型上和思路上都比较类似，还是用一维动态规划中的“局部最优和全局最优法”。这里的区别是维护一个局部最优不足以求得后面的全局最优，这是由于乘法的性质不像加法那样，累加结果只要是正的一定是递增，乘法中有可能现在看起来小的一个负数，后面跟另一个负数相乘就会得到最大的乘积。不过事实上也没有麻烦很多，我们只需要在维护一个局部最大的同时，在维护一个局部最小，这样如果下一个元素遇到负数时，就有可能与这个最小相乘得到当前最大的乘积和，这也是利用乘法的性质得到的


//153. Find Minimum in Rotated Sorted Array 在旋转排序数组中查找最小值
//首先要判断这个有序数组是否旋转了，通过比较第一个和最后一个数的大小，如果第一个数小，则没有旋转，直接返回这个数。如果第一个数大，就要进一步搜索。我们定义left和right两个指针分别指向开头和结尾，还要找到中间那个数，然后和left指的数比较，如果中间的数大，则继续二分查找右半段数组，反之查找左半段。终止条件是当左右两个指针相邻，返回小的那个
int findMin(int* nums, int numsSize) {
    int low=0, high=numsSize-1;
    while (low < high) {
        if (nums[low] < nums[high])
            return nums[low];

        int mid = low + ((high-low)>>1);
        if (nums[mid] >= nums[low])
            low = mid+1;
        else
            high = mid;
    }
    return nums[low];    
}

//154. Find Minimum in Rotated Sorted Array 在旋转排序数组中查找最小值(有重复元素)
int findMin(int* nums, int numsSize) {
    if (NULL == nums || numsSize < 1)
        return 0;
    int low=0, high = numsSize-1, mid = 0;
    while (low < high) {
        mid = low + ((high-low)>>1);
        if (nums[mid] > nums[high])
            low = mid+1;
        else if (nums[mid] < nums[high])
            high = mid;
        else {
            if (nums[low]==nums[mid]) {
                low++;
                high--;
            } else {
                high = mid;
            }
        }
    }
    return nums[high];
}

//128. Longest Consecutive Sequence 
//给定一个整形数组，求出最长的连续序列。例如数组[100,4,200,1,3,2]，最长的连续序列长度为[1,2,3,4]，长度为4。要求时间复杂度为O(n)


//42. Trapping Rain Water 积水问题
//给以一个数组，每个元素表示长方体的高，这些长方体宽度为1，求这个数组的长方体组成的图形能蓄多少水
//当前位置能蓄水要求两边的高度都高于当前位置，所以我们扫描到当前位置，只需判断当前位置是否对蓄的水有贡献
//双指针 时间 O(N) 空间 O(1)
int trap(int* height, int n) {
    int *L = height, *R = L+n-1, level = 0, water = 0;
    while (L < R) {
        int lower = *L < *R ? *L++ : *R--;
        if (lower > level) level = lower;
        water += level - lower;
    }
    return water;
}
/*
int trap(int* height, int n) {
    int level = 0, water = 0;
    while (n--) {
        int lower = *height < height[n] ? *height++ : height[n];
        if (lower > level) level = lower;
        water += level - lower;
    }
    return water;
}
*/

//381. Insert Delete GetRandom O(1) - Duplicates allowed 


//85. Maximal Rectangle
//在一个M * N的矩阵中，所有的元素只有0和1， 找出只包含1的最大矩形
//动态规划

//57. Insert Interval 
//给一些已经按开始时间排好序的区间，现在往里面插入一个区间，如果有重叠就合并区间，返回插入后的区间序列
// Example 1:
//Given intervals [1,3],[6,9], insert and merge [2,5] in as [1,5],[6,9].

//Example 2:
//Given [1,2],[3,5],[6,7],[8,10],[12,16], insert and merge [4,9] in as [1,2],[3,10],[12,16].

//This is because the new interval [4,9] overlaps with [3,5],[6,7],[8,10]
/**
 * Definition for an interval.
 * struct Interval {
 *     int start;
 *     int end;
 * };
 */
struct Interval* insert(struct Interval* intervals, int size, struct Interval newInterval, int* returnSize)
{
    size++;
    struct Interval *arr = (struct Interval*)malloc(sizeof(struct Interval)*size);
    int j = 0;
    int i = 0;
    int count=0;
    while(i < size-1) //inserting newInterval into arr with all in ascending order;
    {
        if(intervals[i].start < newInterval.start || count) // newInterval already inserted then we need to collect the remained intervals or it's bigger then we need to collect the intervals first;
            arr[i+count] = intervals[i++];
        else
            arr[i] = newInterval, count++; //inserting newInterval and counter it as a flag;
    }
    if(count == 0) arr[i] = newInterval; //make sure newInterval is not missed at the end of the intervals;
    *returnSize = 0;
    int max = 0;
    i = 0;
    while(i < size)
    {
        struct Interval t;
        t.start = arr[i].start;
        int j = i+1;
        max = arr[i].end;
        while(j<size && arr[j].start<=max) //merging intervals in arr now;
        {
            if(arr[j].end > max) max = arr[j].end;
            j++;
        }
        i = j; //update the next start index;
        t.end = max;
        *returnSize += 1;
        arr[*returnSize-1] = t; //since we are merging, arr then can be reused;
    }
    return arr;
}

//41. First Missing Positive 在数组中找到第一个丢失的正整数
//给定一个数组，找出第一个缺失的正数，要求时间复杂度为O(n)，空间复杂度为O(1)
//Given [1,2,0] return 3,
//and [3,4,-1,1] return 2
void swap(int* nums, int j, int k) {
    int tmp = nums[k];
    nums[k] = nums[j];
    nums[j] = tmp;
}

int firstMissingPositive(int* nums, int numsSize) {
    int i = 0;
    int p = numsSize;

    while (i < p) {
        if (nums[i] == i+1)  ++i; // already in the right place,
        else if (nums[i] <= 0 || nums[i] > p || nums[i] == nums[nums[i]-1]) {
            swap(nums, i, --p); // out of range or duplicate, put it at the end
        } else {
            swap(nums, i, nums[i]-1); // within range; put it in the right place.
        }
    }

    return p + 1;
}

//4. Median of Two Sorted Arrays
//找两个已排序数组的中位数，其实就是将两个有序数组有序合并后找第K小的数。而找第K小的数，可以将K平分到两个数组中，然后利用一个重要的结论：如果A[k/2-1]<B[k/2-1]，那么A[0]~A[k/2-1]一定在第k小的数的序列当中
//定义一个函数来找到第K个元素，由于两个数组长度之和的奇偶不确定，因此需要分情况来讨论，对于奇数的情况，直接找到最中间的数即可，偶数的话需要求最中间两个数的平均值。下面重点来看如何实现找到第K个元素，首先我们需要让数组1的长度小于或等于数组2的长度，那么我们只需判断如果数组1的长度大于数组2的长度的话，交换两个数组即可，然后我们要判断小的数组是否为空，为空的话，直接在另一个数组找第K个即可。还有一种情况是当K = 1时，表示我们要找第一个元素，只要比较两个数组的第一个元素，返回较小的那个即可
#define min(x, y) (x < y ? x : y)
int odd(int n) { return n & 0x1; }
void swap(int *x, int *y) {
    int tmp = *x; *x = *y; *y = tmp;
}
/* meidan of an array */
double medianof(int A[], int n) {
    return odd(n) ? (double) A[n / 2] : (double)(A[ n / 2] + A[n / 2 - 1]) / 2.0;
}
int find(int A[], int m, int B[], int n) {
    int l = 0, u = m;
    int i, j, half = (m + n + 1) / 2;
    if (!A || m == 0)
        return medianof(B, n);
    if (!B || n == 0)
        return medianof(A, m);
    while (l < u) {
        i = (l + u) / 2;
        j = half - i - 2;
        if (j < 0 || j >= n) {
            if (j == -1 && A[i] <= B[0])
                return i; /* found */
            if (j >= n )
                l = i + 1; /* too small */
            else
                u = i; /* too big */
        } else {
            if (B[j]<= A[i] && (j == n - 1 || A[i] <= B[j+1]))
                return i; /* found */
            else if (A[i] < B[j])
                l = i + 1; /* too small */
            else
                u = i; /* too big */
        }
    }
    return -1;
}
double findMedianSortedArrays(int A[], int m, int B[], int n) {
    int i, j, k, *C;
    if (!A || m == 0)
        return medianof(B, n);
    if (!B || n == 0)
        return medianof(A, m);
    if ((i = find(A, m, B, n)) == -1) {
        i = find(B, n, A, m);
        C = A; A = B; B = C;
        swap(&m, &n);
    }
    if (odd(m + n))
        return (double)A[i];
    j = (m + n) / 2 - i - 2;
    if (i == m - 1)
        k = B[j+1];
    else if (j == n - 1)
        k = A[i+1];
    else
        k = min(A[i+1], B[j+1]);
    return (double)(A[i] + k) / 2.0;
}

//126. Word Ladder II

