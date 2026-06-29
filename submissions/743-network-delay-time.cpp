class Solution {
public:
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        vector<vector<pair<int,int>>> route(n+1);
        for (int i = 0; i < times.size(); ++i) {
            route[times[i][0]].push_back({times[i][1],times[i][2]});
        }
        vector<int> dist(n+1, INT_MAX);
        dist[k] = 0;

        priority_queue< pair<int, int>, vector<pair<int, int>>, greater<> > pq;
        pq.push({0, k});

        while (!pq.empty()) {
            auto [d, node] = pq.top();
            pq.pop();
            if (d > dist[node]) continue;
            for(int i = 0; i < route[node].size(); ++i) {
                auto [to, weight] = route[node][i];
                if (dist[node] + weight < dist[to]) {
                    dist[to] = dist[node] + weight;
                    pq.push({dist[to], to});
                }
            }
        }
        int ans = 0;
        for (int i = 1; i <= n; ++i) ans = max(ans, dist[i]);
        return (ans == INT_MAX) ? -1 : ans;
    }
};
