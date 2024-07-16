class Solution {
public:
    int largestAltitude(vector<int>& gain) {
        int altitude = 0;
        int result = 0;
        for (int i = 0; i < gain.size(); i++){
            altitude += gain[i];
            if (result < altitude){
                result = altitude;
            }
        }
        return result;
    }
};
