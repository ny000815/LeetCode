class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector<vector<string>> result;
        unordered_map<string, vector<string>> anagrams;
        for(int i = 0; i < strs.size(); ++i) {
            array<int, 26> count = {};
            for(int j = 0; j < strs[i].size(); ++j) {
                char c = strs[i][j];
                count[c - 'a']++;
            }
            string key;
            for (int k = 0; k < 26; ++k) {
                key += to_string(count[k]);
                key += '#';
            }
            anagrams[key].push_back(strs[i]);
        }
        for (auto& [key, value] : anagrams) {
            result.push_back(value);
        }
        return result;
    }
};
