class Solution {
public:
    int mostWordsFound(vector<string>& sentences) {
        int maximum = 0;
        for(string sentence: sentences) {
            int count = 0;
            for(char c : sentence) {
                if (c == ' ') ++count;
            }
            maximum = max(maximum, count+1);
        }
        return maximum;
    }
};
