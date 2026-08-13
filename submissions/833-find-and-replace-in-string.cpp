class Solution {
public:
    string findReplaceString(string s, vector<int>& indices, vector<string>& sources, vector<string>& targets) {
        int n = s.size(), k = indices.size();
        vector<int> match(n, -1);
        for(int i = 0; i < k; ++i) {
            if (s.compare(indices[i], sources[i].size(), sources[i]) == 0) match[indices[i]]=i;
        }
        string res;
        for(int i = 0; i < n;) {
            if(match[i] != -1){
                res += targets[match[i]];
                i += sources[match[i]].size();
            }
            else {
                res += s[i]; 
                i++; 
            }
        }
        return res;
    }
};
