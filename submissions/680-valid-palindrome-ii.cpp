class Solution {
public:
    bool validPalindrome(string s) {
        int i = 0;
        int j = s.size()-1;
        while(i < j) {
            if (s[i] != s[j]) {
                bool isValid1 = true;
                bool isValid2 = true;
                int i2 = i, j2 = j;
                i++;
                while(i < j) {
                    if(s[i] != s[j]) isValid1 = false;
                    i++; j--;
                }
                j2--;
                while(i2 < j2) {
                    if(s[i2] != s[j2]) isValid2 = false;
                    i2++; j2--;
                }
                return isValid1 || isValid2;
            }
            i++;j--;
        }
        return true;
    }
};
