class Solution {
public:
    vector<int> asteroidCollision(vector<int>& asteroids) {
        vector<int> result;
        
        for (int asteroid : asteroids) {
            while (!result.empty() && result.back() > 0 && asteroid < 0 && result.back() < abs(asteroid)) {
                result.pop_back();
            }
            
            if (result.empty() || result.back() < 0 || asteroid > 0) {
                result.push_back(asteroid);
            } else if (result.back() == abs(asteroid)) {
                result.pop_back();
            }
        }
        
        return result;
    }
};
