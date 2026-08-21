class Solution {
public:
    int maximumWealth(vector<vector<int>>& accounts) {
        int maximum = 0;
        for(vector<int> account : accounts) {
            int total = 0;
            for(int money : account) {
                total += money;
            }
            maximum = max(maximum, total);
        }
        return maximum;
    }
};
