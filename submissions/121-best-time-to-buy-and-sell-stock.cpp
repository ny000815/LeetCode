class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int low = INT_MAX;
        int result = 0;
        for (int i = 0; i < prices.size(); ++i) {
            if (prices[i] < low){
                low = prices[i];
            } else if (result < prices[i] - low) {
                result = prices[i] - low;
            }
        }
        return result;
    }
};
