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