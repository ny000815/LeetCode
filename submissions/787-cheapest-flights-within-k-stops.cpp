class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        vector<int> dist(n, INT_MAX);
        dist[src] = 0;

        for (int i  = 0; i <= k; ++i) {
            vector<int> tmp = dist;
            for (auto& f : flights) {
                int from = f[0], to = f[1], price = f[2];
                if (dist[from] == INT_MAX) continue;
                if (dist[from] + price < tmp[to]) tmp[to] = dist[from] + price;
            }
            dist = tmp;
        }
        return dist[dst] == INT_MAX ? -1 : dist[dst];
    }
};
