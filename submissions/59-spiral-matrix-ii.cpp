class Solution {
public:
    vector<vector<int>> generateMatrix(int n) {
        vector<vector<int>> matrix(n, vector<int> (n, 0));
        int top = 0;
        int right = n-1;
        int bottom = n-1;
        int left = 0;
        int idx = 1;
        while (idx <= n * n){
            for(int j = left; j <= right; ++j) matrix[top][j] = idx++;
            ++top;
            for(int i = top; i <= bottom; ++i) matrix[i][right] = idx++;
            --right;
            if (top <= bottom) {
                for(int j = right; j >= left; --j) matrix[bottom][j] = idx++;
                --bottom;
            }
            if (left <= right) {
                for(int i = bottom; i >= top; --i) matrix[i][left] = idx++;
                ++left;
            }
        }
        return matrix;
    }
};
