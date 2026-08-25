class Solution {
public:
    string mostCommonWord(string paragraph, vector<string>& banned) {
        vector<char> pun = {' ', '!', '?', '\'', ',', ';', '.'};
        unordered_set punc(pun.begin(), pun.end());

        unordered_map<string, int> count;
        int start = 0;
        for (int i = 0; i < paragraph.size(); ++i) {
            char letter = paragraph[i];
            if(isalpha(letter)) paragraph[i] = tolower(letter);
            if(punc.contains(letter)) {
                if(i > start) ++count[paragraph.substr(start, i-start)];
                start = i + 1;
            }
        }
        unordered_set<string> ban(banned.begin(), banned.end());

        if(!punc.contains(paragraph[paragraph.size()-1])) ++count[paragraph.substr(start, paragraph.size()-start)];

        int maximum = 0;
        string ans;
        for (const auto&[key,value] : count) {
            if (!ban.contains(key) && value > maximum) {
                ans = key;
                maximum = value;
            }
        }
        return ans;
    }
};
