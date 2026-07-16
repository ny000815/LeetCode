class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        unordered_set<int> seen(nums.begin(), nums.end());
        seen.reserve(nums.size());

        int max_cnt = 0;
        for(int num : seen) {
            auto it = seen.find(num-1);
            if (it == seen.end()) {
                int cnt = 1;
                while (seen.find(num + cnt)!= seen.end()) {
                    ++cnt;
                }
            max_cnt = max(max_cnt, cnt);
            }
        }
        return max_cnt;
    }
};
