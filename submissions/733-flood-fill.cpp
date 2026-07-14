class Solution {
public:
    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int color) {
        vector<int> DX = {0, 0, 1, -1};
        vector<int> DY = {1, -1, 0, 0};
        
        int startColor = image[sr][sc];
        if(startColor == color) return image;

        queue<pair<int, int>> q;
        q.push({sr, sc});
        image[sr][sc] = color;

        while (!q.empty()){
            auto [r,c] = q.front(); q.pop();
            for(int i = 0; i < 4; ++i) {
                int nr = r + DX[i], nc = c + DY[i];
                if (nr >= image.size() || nr < 0 || nc >= image[0].size() || nc < 0) continue;
                if (image[nr][nc] != startColor) continue;
                image[nr][nc] = color;
                q.push({nr,nc});
            }
        }
        return image;
    }
};
