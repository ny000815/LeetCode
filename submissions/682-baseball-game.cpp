class Solution {
public:
    int calPoints(vector<string>& operations) {
        vector<int> scores;
        for(string c : operations) {
            if(c == "+") {
                scores.push_back(scores[scores.size()-1]+scores[scores.size()-2]);
            }
            else if(c == "C") {
                scores.pop_back();
            }
            else if(c == "D") {
                scores.push_back(scores[scores.size()-1]*2);
            }
            else{
                scores.push_back(stoi(c));
            }
        }
        return accumulate(scores.begin(), scores.end(), 0);
    }
};
