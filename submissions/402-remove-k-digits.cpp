class Solution {
public:
    string removeKdigits(string num, int k) {
        string ans = "";
        for(int c : num) {
            while (k > 0 && !ans.empty() && ans.back() > c) {
                ans.pop_back();
                --k;
            }
            ans.push_back(c);
        }
        while (k > 0) {
            ans.pop_back();
            --k;
        }
        int i = 0;
        while(i < ans.size() && ans[i] == '0') ++i;
        ans = ans.substr(i);
        return ans.empty()? "0" : ans;
    }
};
