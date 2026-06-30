class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int j = 0;
        int sum = 0;
        int ans = INT_MAX;
        for (int i = 0; i < nums.size(); ++i) {
            sum += nums[i];
            if (sum >= target){
                while (sum - nums[j] >= target) sum-=nums[j++];
                ans = min(ans, i - j + 1);
            }
        }
        return ans == INT_MAX ? 0 : ans;
    }
};
