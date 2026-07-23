class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_set<char> charSet;
        int result = 0;
        int l = 0;

        int n = s.size();
        for(int r = 0; r < n; ++r) {
            while(charSet.find(s[r]) != charSet.end()) {
                charSet.erase(s[l]);
                l++;
            }
            charSet.insert(s[r]);
            result = max(result, r - l + 1);
        }
        return result;
    }
};
