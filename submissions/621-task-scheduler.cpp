class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        vector<int> freq(26,0);
        for(int t : tasks) {
            freq[t-'A']++;
        }
        sort(freq.begin(), freq.end());
        int chunk = freq[25]-1;
        int idol = chunk * n;
        for(int i = 24; i >= 0; i--) {
            idol -= min(chunk, freq[i]);
        }
        return idol < 0? tasks.size():tasks.size()+idol;
    }
};
