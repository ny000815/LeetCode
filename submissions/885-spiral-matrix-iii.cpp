class Solution {
public:
    vector<vector<int>> spiralMatrixIII(int rows, int cols, int rStart, int cStart) {
        vector<vector<int>> result;
        int dx[] = {1, 0, -1, 0};
        int dy[] = {0, 1, 0, -1};
        int directionIdx = 0;

        int legLength = 1;

        int r = rStart, c = cStart;
        result.push_back({rStart, cStart});
        
        while (result.size() < rows * cols) {
            for(int i = 0; i < 2; ++i) {
                for(int leg = 0; leg < legLength; ++leg) {
                    r += dy[directionIdx];
                    c += dx[directionIdx];
                    if (r < 0 || r >= rows || c < 0 || c >= cols) continue;
                    result.push_back({r,c});
                }
                directionIdx = (directionIdx+1) % 4;
            }
            ++legLength;
        }
        return result;

    }
};
