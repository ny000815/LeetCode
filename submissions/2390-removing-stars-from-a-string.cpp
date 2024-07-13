class Solution {
public:
    string removeStars(string s) {
        stack<char> tempresult;

        for (int i = 0; i < s.length(); i++){
            tempresult.push(s[i]);
            if (s[i] == '*' ){
                tempresult.pop();
                tempresult.pop();
            }
        }

        string result = "";
        while (tempresult.size() > 0){
            result += tempresult.top();
            tempresult.pop();
        }
        reverse(result.begin(), result.end());
        
        return result;
    }
};
