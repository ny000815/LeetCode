class Solution {
public:
    bool isValid(string s) {
    static const unordered_map<char, char> close_to_open = {
        {')', '('},
        {'}', '{'},
        {']', '['}
    };

        stack<char> stk;
        for(char c : s) {
            if (close_to_open.count(c)){
                if (stk.empty() || close_to_open.at(c) != stk.top()) return false;
                stk.pop();
            }
            else {
                stk.push(c);
            }
        }
        if (stk.empty()) return true;
        else return false;
    }
};
