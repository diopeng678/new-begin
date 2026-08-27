//724 题 寻找数组的中心索引
class Solution {
public:
    int pivotIndex(vector<int>& nums) {
        int n = nums.size();
        vector<int> f(n), g(n);
        for (int i = 1; i < n; i++)
        {
            f[i] = f[i - 1] + nums[i - 1];
        }
        for (int i = n - 2; i >= 0; i--)
        {
            g[i] = g[i + 1] + nums[i + 1];
        }

        for (int j = 0;j < n;j++)
            if (f[j] == g[j])
                return j;

        return -1;
    }
};
//238题 除自身以外数组的乘法
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = nums.size();
        vector<int> f(n), g(n);
        f[0] = 1;
        g[n - 1] = 1;
        for (int i = 1;i < n;i++)
        {
            f[i] = f[i - 1] * nums[i - 1];
        }
        for (int i = n - 2;i >= 0;i--)
        {
            g[i] = g[i + 1] * nums[i + 1];
        }
        vector<int> ret(n);
        for (int i = 0; i < n; i++)
        {
            ret[i] = g[i] * f[i];
        }
        return ret;
    }
};

//560 题 和为 K 的子数组 前缀和 + 哈希表

class Solution
{
public:
    // 函数：nums是输入数组，k是目标和，返回和为k的连续子数组数量
    int subarraySum(vector<int>& nums, int k)
    {
        // 1. 定义哈希表：key=前缀和，value=该前缀和出现过的次数
        unordered_map<int, int> hash;
        // 2. 关键初始化：空前缀和0，默认出现1次
        hash[0] = 1;

        // 3. sum：滚动记录当前位置的前缀和；ret：最终答案，统计符合条件的子数组总数
        int sum = 0, ret = 0;

        // 4. 遍历数组中每一个元素x
        for (auto x : nums)
        {
            // 5. 更新前缀和：累加当前元素
            sum += x;

            // 6. 核心判断：查看之前是否存在前缀和 = sum - k
            if (hash.count(sum - k))
                ret += hash[sum - k];

            // 7. 把当前前缀和存入哈希表，次数+1，留给后面循环使用
            hash[sum]++;
        }
        // 8. 遍历结束，返回统计到的总数
        return ret;
    }
};
//974 题 和可被 K 整除的子数组 前缀和 + 哈希表
class Solution {
public:
    int subarraysDivByK(vector<int>& nums, int k) {
        unordered_map<int, int> hash;
        hash[0 % k] = 1;

        int sum = 0;
        int ret = 0;
        for (auto x : nums)
        {
            sum += x;     //算出前缀和
            int r = (sum % k + k) % k;    //修正后的余数
            if (hash.count(r)) ret += hash[r];
            hash[r]++;
        }
        return ret;
    }
};

//1576 题 替换所有的问号  模拟的方法

class Solution {
public:
    string modifyString(string s) {
        int n = s.size();
        for (int i = 0;i < n;i++)
        {
            if (s[i] == '?')
            {
                for (char ch = 'a';ch < 'z';ch++)
                {
                    if ((i == 0 || ch != s[i - 1]) && (ch != s[i + 1] || i == n + 1))
                    {
                        s[i] = ch;
                        break;
                    }
                }
            }
        }
        return s;
    }

};