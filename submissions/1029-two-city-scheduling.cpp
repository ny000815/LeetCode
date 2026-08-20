class Solution {
public:
    int twoCitySchedCost(vector<vector<int>>& costs) {
        int n = costs.size();
        for(int i = 0; i < n; ++i) {
            costs[i].push_back(abs(costs[i][0]-costs[i][1]));
        }
        sort(costs.begin(), costs.end(), [](const auto& a, const auto& b){
            return a[2] > b[2];
        });
        int ans = 0;
        int aCount = 0, bCount = 0;
        int i = 0;
        while (aCount != n/2 && bCount != n/2){
            if(costs[i][0] < costs[i][1]) {
                ans += costs[i][0];
                ++aCount;
            }
            else {
                ans += costs[i][1];
                ++bCount;
            }
            ++i;
        }
        while(aCount < n/2){
            ans += costs[i][0];
            ++aCount;
            ++i;
        }
        while(bCount < n/2){
            ans += costs[i][1];
            ++bCount;
            ++i;
        }
        return ans;
    }
};
