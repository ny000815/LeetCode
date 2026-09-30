class Solution {
    public int maxSubArray(int[] nums) {
        int n = nums.length;
        int ans = nums[0];
        int current = 0;
        for(int i = 0; i < n; ++i) {
            current = (current < 0)? nums[i] : nums[i]+current;
            ans = Math.max(ans, current);
        }
        return ans;
    }
}
