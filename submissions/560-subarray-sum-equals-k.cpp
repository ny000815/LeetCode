class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int n = nums.size(); 
        int sum = 0, ans = 0;
        unordered_map<int, int> count;
        count[0]++;
        for(int i = 0; i < n; ++i) {
            sum += nums[i];
            if(count[sum-k]) ans += count[sum-k];
            count[sum]++;
        }
        return ans;
    }
};
