class Solution {
public:
    string mergeAlternately(string word1, string word2) {
        string res;
        while(word1.size() && word2.size()) {
            res += word1[0];
            res += word2[0];
            word1 = word1.substr(1);
            word2 = word2.substr(1);
        }
        if(word1.size()) res += word1;
        if(word2.size()) res += word2;
        return res;
    }
};
