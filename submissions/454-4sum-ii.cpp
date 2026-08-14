class Solution {
public:
    int fourSumCount(vector<int>& nums1, vector<int>& nums2, vector<int>& nums3, vector<int>& nums4) {
        int res=0;
        unordered_map<int, int> seen;
        for(int n : nums1) {
            for(int m : nums2) {
                seen[n+m]++;
            }
        }
        for(int n: nums3) {
            for(int m: nums4) {
                if(seen[0-m-n]) res+= seen[0-m-n];
            }
        }
        return res;
    }
};
