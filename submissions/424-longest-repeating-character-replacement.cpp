class Solution {
public:
    int characterReplacement(string s, int k) {
        int n = s.size();
        int result = 0;
        int i = 0;
        int mostF = 0;
        int count[26] = {0};
        for(int j = 0; j < n; ++j) {
            count[s[j]- 'A']++;
            mostF = max(mostF, count[s[j]- 'A']);
            while((j - i + 1) - mostF > k) {
                count[s[i]- 'A']--;
                i++;
            }
            result = max(result, j - i + 1);
        }
        return result;
    }
};
