class Solution {
public:
    bool isSubsequence(string s, string t) {
        int s_len = s.length(), t_len = t.length();
        int j = 0;

        for(int i = 0; j < s_len && i < t_len; i++){
                if(t[i] == s[j]) j++;
        }
        if(j == s_len) {
            return true;
        }else return false;
    }
};
