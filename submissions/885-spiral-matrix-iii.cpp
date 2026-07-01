class Solution {
public:
    vector<vector<int>> spiralMatrixIII(int rows, int cols, int rStart, int cStart) {
        int colD[] = {1,0,-1,0};
        int rowD[] = {0,1,0,-1};

        vector<vector<int>> visited;
        int currentRow = rStart;
        int currentCol = cStart;

        visited.push_back({currentRow, currentCol});

        int directionIndex = 0;
        int legLength = 1;

        while ((int)visited.size() < rows*cols) {
            for(int leg = 0; leg < 2; ++leg) {
                for(int step = 0; step < legLength; ++step) {
                    currentRow += rowD[directionIndex];
                    currentCol += colD[directionIndex];

                    if(currentRow<0 || currentRow >= rows || currentCol<0 || currentCol >= cols) continue;
                    visited.push_back({currentRow, currentCol});
                }
                directionIndex = (directionIndex + 1) % 4;
            }
            ++legLength;
        }
        return visited;
    }
};
