class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector<vector<string>> result;
        unordered_map<string, vector<string>> seen;
        const int n = strs.size();
        seen.reserve(n);
        
        for(int i = 0; i < n; ++i) {
            int count[26] = {0};
            int const len = strs[i].size();
            for(int j = 0; j < len; ++j) {
                ++count[strs[i][j]-'a'];
            }
            string key = "";
            for(int j = 0; j < 26; ++j){
                key += to_string(count[j])+'#';
            }
            seen[key].push_back(strs[i]);
        }
        for(auto &[key, value] : seen) result.push_back(move(value));
        return result;
    }
};
