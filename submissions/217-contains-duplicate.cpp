class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        unordered_set<int> seen;
        const int n = nums.size();
        seen.reserve(n);

        for(int x : nums) {
            if(!seen.insert(x).second) return true;
        }
        return false;
    }
};
