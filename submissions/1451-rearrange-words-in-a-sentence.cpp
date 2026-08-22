class Solution {
public:
    string arrangeWords(string text) {
        text[0] = tolower(text[0]);
        vector<string> words;
        int start = 0;
        for(int i = 0; i < text.size(); ++i) {
            if (text[i] == ' ') {
                words.push_back(text.substr(start, i - start));
                start = i+1;
            }
        }
        words.push_back(text.substr(start, text.size() - start));
        stable_sort(words.begin(), words.end(), [](const auto& w1, const auto& w2) {
           return w1.size() < w2.size(); 
        });
        string ans;
        for(int i = 0; i < words.size(); ++i) {
            if (i != 0) ans += " ";
            ans += words[i];
        }
        ans[0] = toupper(ans[0]);
        return ans;
    }
};
