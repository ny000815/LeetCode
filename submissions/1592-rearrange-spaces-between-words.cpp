class Solution {
public:
    string reorderSpaces(string text) {
        bool InWord = false;
        int spaceCount = 0;
        int wordCount = 0;
        int wordStart;
        vector<string> words;
        for(int i = 0; i < text.size(); ++i) {
            if(!InWord) wordStart = i;
            if('a' <= text[i] && text[i] <= 'z'){
                InWord = true;
            }
            else {
                if(InWord) {
                    wordCount++;
                    words.push_back(text.substr(wordStart, i - wordStart));
                }
                InWord = false;
                spaceCount++;
            }
        }
        if (InWord) {
            wordCount++;
            words.push_back(text.substr(wordStart));
        }
        int spacesBetween = wordCount > 1 ? spaceCount / (wordCount - 1) : 0;
        int extraSpaces   = wordCount > 1 ? spaceCount % (wordCount - 1) : spaceCount;
        string ans = "";
        for(int i = 0; i < wordCount; ++i){
            ans += words[i];
            if (i < wordCount -1) {
                for(int i = 0; i < spacesBetween; ++i) {
                    ans += " ";
                }
            }
        }
        for(int i = 0; i < extraSpaces; ++i) ans += " ";
        return ans;
    }
};
