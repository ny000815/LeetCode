class Solution {
public:
    string sortSentence(string s) {
        vector<string> sentence;
        int start = 0;
        for(int i = 0; i < s.size(); ++i) {
            if (s[i] == ' '){
                sentence.push_back(s.substr(start, i-start));
                start = i+1;
            }
        }
        if(start != s.size()) sentence.push_back(s.substr(start, s.size()-start));

        vector<string> words(sentence.size()+1);
        for(string word : sentence) {
            int pos = word.back()-'0';
            word.pop_back();
            words[pos] = word;
        }

        string ans = "";
        for(int i = 1; i < words.size(); ++i) {
            if(i != 1) ans += " ";
            ans += words[i];
        }
        return ans;
    }
};
