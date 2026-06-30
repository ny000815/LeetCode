class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        stack<int> stk;
        vector<int> ans(temperatures.size(), 0);
        for(int i = 0; i < temperatures.size(); ++i) {
            while(!stk.empty() && temperatures[i] > temperatures[stk.top()]) {
                int prev = stk.top();
                stk.pop();
                ans[prev] = i - prev;
            }
            stk.push(i);
        }
        return ans;
    }
};
