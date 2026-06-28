class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0, right = nums.size()-1;
        while (right - left > 1){
            int mid = (right + left) / 2;
            if (nums[mid] == target) return mid;
            if (nums[left] <= nums[mid]) {
                if (nums[left] <= target && target <= nums[mid]) right = mid;
                else left = mid;
            } else {
                if (nums[mid] <= target && target <= nums[right]) left = mid;
                else right = mid;
            }
        }
        if(nums[right] == target) return right;
        if(nums[left] == target) return left;
        return -1;
    }
};
