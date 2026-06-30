class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int j = 0;
        unordered_map<char, int> seen;
        int maxLen = 0;
        for (int i =0; i < s.size(); ++i) {
            if(seen.find(s[i]) != seen.end()) {
                if (seen[s[i]] >= j) j = seen[s[i]]+1;
            }
            maxLen = max(maxLen, i-j + 1);
            seen[s[i]] = i;
        }
        return maxLen;
    }
};
