class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        string result = strs[0];
        for(string str : strs) {
            for(int i = 0; i < result.size(); ++i) {
                if (result[i] != str[i]) {
                    result = result.substr(0, i);
                    break;
                }
            }
        }
        return result;
    }
};
