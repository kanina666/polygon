class Solution {
public:
    int maxCoins(vector<int>& nums) {
        int n = nums.size(); //+4

        nums.insert(nums.begin(), 1);
        nums.push_back(1);
        //(n+2)*4

        vector<vector<int>> dp(n + 2, vector<int>(n + 2, 0));//(n+2)**2*4

        for (int left = n; left >= 1; --left) { // O(n)
            for (int right = 1; right <= n; ++right) { // O(n)
                if (left > right) continue;

                int maxCoins = 0;

                for (int burst = left; burst <= right; ++burst) { // O(n)
                    int coins = nums[left - 1] * nums[burst] * nums[right + 1];
                    int rem = dp[left][burst - 1] + dp[burst + 1][right];
                    maxCoins = max(maxCoins, coins + rem);
                }

                dp[left][right] = maxCoins;
            }
        }

        return dp[1][n];
    }
};
// memory used:(n+2)**2*4+4+(n+2)*4 bytes
// сложность: O(n*n*n)= O(n**3)