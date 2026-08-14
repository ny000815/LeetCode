class Solution {
public:
    int numPairsDivisibleBy60(vector<int>& time) {
        int res = 0;
        vector<int> nums(60,0);
        for(int t : time) {
            res += nums[(60-t%60)%60];
            nums[t%60]++;
        }
        return res;
    }
};
