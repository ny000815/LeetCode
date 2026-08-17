class Solution {
public:
    int jump(vector<int>& nums) {
        int currentEnd = 0, nextEnd = 0, jumps = 0;
        int n = nums.size();
        for(int i = 0; i < n-1; ++i) {
            nextEnd = max(nextEnd, i + nums[i]);
            if(i == currentEnd) {
                jumps++;
                currentEnd = nextEnd;
            }
        }
        return jumps;
    }
};
