class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        queue<pair<int,int>> q;
        int fresh = 0;
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[0].size(); ++j) {
                if (grid[i][j] == 2) q.push({i,j});
                else if (grid[i][j] == 1) ++fresh;
            }
        }
        if (fresh == 0) return 0;
        int time = 0;
        vector<int> DX = {0, 0, 1, -1};
        vector<int> DY = {1, -1, 0, 0};

        while (!q.empty()) {
            int size = q.size();
            bool spread = false;
            for (int i = 0; i < size; ++i) {
                auto [x, y] = q.front();
                q.pop();
                for(int j = 0; j < 4; ++j) {
                    int nx = x + DX[j], ny = y + DY[j];
                    if (nx < 0 || ny < 0 || nx >= grid.size() || ny >= grid[0].size()) continue;
                    if (grid[nx][ny] != 1) continue;
                    grid[nx][ny] = 2;
                    --fresh;
                    q.push({nx, ny});
                    spread = true;
                }
            }
            if (spread) ++time;
        }
        return fresh == 0? time: -1;
        
    }
};
