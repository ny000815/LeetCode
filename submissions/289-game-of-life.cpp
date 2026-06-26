class Solution {
public:
    void gameOfLife(vector<vector<int>>& board) {
        int m = board.size(), n = board[0].size();
        vector<vector<int>> copy = board;
        vector<int> DX = {0, 0, 1, 1, 1, -1, -1, -1};
        vector<int> DY = {1, -1, 0, 1, -1, 0, 1, -1};
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                int count = 0;
                for (int k = 0; k < 8; k++) {
                    int ni = DX[k]+i, nj = DY[k]+j;
                    if (ni >= 0 && nj >= 0 && ni < m && nj < n && copy[ni][nj]) ++count;
                }

                if (count == 2 || count == 3) {
                    if (count == 2 && !board[i][j]) continue;
                    board[i][j] = 1;
                } else {
                    board[i][j] = 0;
                }

            }
        }
    }
};
