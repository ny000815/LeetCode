class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        unordered_map<int, int> location;
        for(int i = 0; i < nums.size(); ++i) {
            if(location.count(nums[i])) {
                if (i - location[nums[i]] <= k) return true;
            }
            location[nums[i]] = i;
        }
        return false;
    }
};
