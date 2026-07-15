class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        const int n = nums.size();
        unordered_map<int, int> count;
        for(int i = 0; i < n; ++i){
            ++count[nums[i]];
        }
        priority_queue<pair<int, int>, vector<pair<int, int>>> pq;
        for(auto& [key,value] : count){
            pq.push({value, key});
        }
        vector<int> result;
        for(int i = 0; i < k; ++i){
            result.push_back(pq.top().second);
            pq.pop();
        }
        return result;
    }
};
