class Solution {
public:
    int equalPairs(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        map<vector<int>, int> seen;
        for(int i = 0; i < m; ++i) {
            seen[grid[i]]++;
        }
        for(int i = 0; i < m; ++i) {
            for(int j = i+1; j < n; ++j) {
                swap(grid[i][j], grid[j][i]);
            }
        }
        
        int ans = 0;
        for(int i = 0; i < m; ++i) {
            if(seen[grid[i]]) ans += seen[grid[i]];
        }
        return ans;
    }
};
