class Solution {
public:
    string reformat(string s) {
        string al, num;
        for(int i = 0; i < s.size(); ++i) {
            if(isalpha(s[i])) al += s[i];
            else num += s[i];
        }
        if(al.size() > num.size()+1 || num.size() > al.size()+1) return "";
        string ans;
        ans.reserve(s.size());
        if (num.size() > al.size()) swap(al, num);
        for(int i = 0; i < min(num.size(), al.size()); ++i) {
            ans += al[i];
            ans += num[i];
        }
        if (al.size() > num.size()) ans += al.back();
        return ans;
    }
};
