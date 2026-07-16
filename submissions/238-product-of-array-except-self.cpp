class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = nums.size();
        vector<int> result(n, 1);
        
        for(int i = n - 2; i >= 0; --i) {
            result[i] = result[i+1] * nums[i+1];
        }
        int left_prod = 1;
        for(int i = 0; i < n; ++i) {
            result[i] *= left_prod;
            left_prod *= nums[i];
        }
        return result;
    }
};
