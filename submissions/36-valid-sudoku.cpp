class Solution {
public:
    bool isValidSudoku(vector<vector<char>>& board) {
        for(int i = 0; i < 9; ++i){
            unordered_set<int> seen;
            for(int j = 0; j < 9; ++j) {
                if (board[i][j] == '.') continue;
                if(board[i][j] && seen.count(board[i][j])) return false;
                if(board[i][j]) seen.insert(board[i][j]); 
            }
        }
        for(int i = 0; i < 9; ++i){
            unordered_set<int> seen;
            for(int j = 0; j < 9; ++j) {
                if (board[j][i] == '.') continue;
                if(board[j][i] && seen.count(board[j][i])) return false;
                if(board[j][i]) seen.insert(board[j][i]); 
            }
        }
        for(int i = 0; i < 3; ++i) {
            for(int j = 0; j < 3; ++j) {
                unordered_set<int> seen;
                for(int k = 0; k < 3; ++k) {
                    for(int l = 0; l < 3; ++l) {
                        int a = k + 3 * i;
                        int b = l + 3 * j;
                        if (board[a][b] == '.') continue;
                        if(board[a][b] && seen.count(board[a][b])) return false;
                        if(board[a][b]) seen.insert(board[a][b]); 
                    }
                }
            }
        }
        return true;
    }
};
