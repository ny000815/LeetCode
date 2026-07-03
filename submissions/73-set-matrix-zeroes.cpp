class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        vector<int> cIdx(matrix.size(), 0);
        vector<int> rIdx(matrix[0].size(), 0);
        for(int i = 0; i < matrix.size(); ++i) {
            for(int j = 0; j < matrix[0].size(); ++j) {
                if (matrix[i][j] == 0){
                    cIdx[i] = 1;
                    rIdx[j] = 1;
                }
            }
        }
        for(int i = 0; i < matrix.size(); ++i) {
            for(int j = 0; j < matrix[0].size(); ++j){
                if (cIdx[i] || rIdx[j]) matrix[i][j] = 0;
            }
        }
    }
};
