class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        vector<int> ans;
        int n1 = s.size(), n2 = p.size();

        if(n1 < n2) return ans;
        vector<int> a(26,0), b(26,0);
        for(int i = 0; i < n2; ++i) {
            a[s[i]-'a']++;
            b[p[i]-'a']++;
        }
        if(a == b) ans.push_back(0);
        for(int i = n2; i < n1; ++i) {
            a[s[i]-'a']++;
            a[s[i-n2]-'a']--;
            if(a == b) ans.push_back(i-n2+1);
        }
        return ans;
    }
};
