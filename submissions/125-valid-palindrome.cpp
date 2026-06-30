class Solution {
public:
    bool isPalindrome(string s) {
        vector<char> s2;
        for(int i = 0; i < s.size(); ++i) {
            if (isalnum(s[i])) s2.push_back(tolower(s[i]));
        }
        for(int i = 0; i < s2.size()/2; ++i) {
            if (s2[i] != s2[s2.size()-i-1]) return false;
        }
        return true;
    }
};
