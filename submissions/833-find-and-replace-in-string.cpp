class Solution {
public:
    string findReplaceString(string s, vector<int>& indices, vector<string>& sources, vector<string>& targets) {
        int n = s.size(), k = indices.size();
        vector<int> op(n, -1);
        for(int i = 0; i < k; ++i){
            if (s.compare(indices[i], sources[i].size(), sources[i]) == 0)
                op[indices[i]] = i;
        }
        string res = "";
        for(int i = 0; i < n;) {
            int idx = op[i];
            if(idx != -1) {
                res += targets[idx];
                i += sources[idx].size();
            }
            else {
                res += s[i];
                i++;
            }
        }
        return res;
    }
};
