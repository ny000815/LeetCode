class Solution {
public:
    string discountPrices(string sentence, int discount) {
        vector<string> words;
        int i = 0;
        for(int j = 0; j < sentence.size(); ++j) {
            if(j != 0 && sentence[j] == ' ') {
                words.push_back(sentence.substr(i, j-i));
                i = j + 1;
            }
        }
        words.push_back(sentence.substr(i));
        string res;
        for(string word: words) {
            if(word.size() >= 2 && word[0] == '$') {
                bool isPrice = true;
                for (int i = 1; i < word.size(); ++i) {
                    if (!isdigit(word[i])) {
                        isPrice = false;
                        break;
                    }
                }
                if(isPrice){
                    double num = stod(word.substr(1));
                    word = '$';
                    num = num - num * discount * 0.01;
                    char buf[64];
                    snprintf(buf, sizeof(buf), "$%.2f", num);
                    word = buf;
                }
            }
            res += word;
            res += ' ';
        }
        res = res.substr(0, res.size()-1);
        return res;
    }
};
