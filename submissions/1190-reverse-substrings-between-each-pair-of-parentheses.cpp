class Solution {
public:
    string reverseParentheses(std::string s) {
        stack<std::string> stk;
        string current;
        
        for (char c : s) {
            if (c == '(') {
                stk.push(current);
                current = "";
            } else if (c == ')') {
                reverse(current.begin(), current.end());
                if (!stk.empty()) {
                    current = stk.top() + current;
                    stk.pop();
                }
            } else {
                current += c;
            }
        }
        
        return current;
    }
};
