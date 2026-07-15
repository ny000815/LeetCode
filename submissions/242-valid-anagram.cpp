class Solution {
public:
    bool isAnagram(string s, string t) {
        if (s.size() != t.size()) return false;
        
        int seen[26] = {0};
        int n = s.size();
        for(int i = 0; i < n; ++i){
            ++seen[s[i]-'a'];
            --seen[t[i]-'a'];
        }
        for(int i = 0; i < 26; ++i){
            if(seen[i] != 0) return false;
        }
        return true;
    }
};
