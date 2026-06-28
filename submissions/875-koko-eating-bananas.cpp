class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int left = 0;
        int right = *max_element(piles.begin(), piles.end());
        while (right - left > 1) {
            int mid = (right + left)/2;
            long long count = 0;
            for(int i = 0; i < piles.size(); ++i) {
                count += (piles[i]+mid-1) / mid;
            }
            if (h >= count) right = mid;
            else left = mid;
        }
        return right;
    }
};
