class Solution {
public:
    int findMinArrowShots(vector<vector<int>>& points) {
        sort(points.begin(), points.end());
        int arrowX = points[0][1];
        int ans = 1;
        for(vector<int> p : points) {
            if(p[0] > arrowX){
                ans++;
                arrowX = p[1];
            }
            arrowX = min(arrowX, p[1]);
        }
        return ans;
    }
};
