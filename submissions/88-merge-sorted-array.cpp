class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int idx = nums1.size()-1;
        int i = m-1, j = n-1;
        while(idx >= 0) {
            if(j < 0 || i >= 0 && nums1[i] > nums2[j]) nums1[idx--] = nums1[i--];
            else nums1[idx--] = nums2[j--];
        }
    }
};
