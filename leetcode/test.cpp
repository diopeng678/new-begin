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