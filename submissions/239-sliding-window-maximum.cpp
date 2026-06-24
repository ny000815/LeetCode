class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        int n = nums.size();
        vector<int> result;
        deque<int> q;
        for (int i = 0; i < n; i++) {
            if (!q.empty() && q.front() < i - k + 1) q.pop_front();
            while (!q.empty() && nums[q.back()] < nums[i]) q.pop_back();
            q.push_back(i);
            if (i >= k - 1) result.push_back(nums[q.front()]);
        }
        return result;
    }
};
