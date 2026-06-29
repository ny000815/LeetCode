class Solution {
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> adj(numCourses);
        vector<int> indegree(numCourses, 0);
        for (auto& e : prerequisites) {
            int from = e[0], to = e[1];
            adj[from].push_back(to);
            ++indegree[to];
        }
        queue<int> q;
        for (int i = 0; i < numCourses; ++i) {
            if(indegree[i] == 0) q.push(i);
        }
        vector<int> done;
        while(!q.empty()) {
            int node = q.front();
            q.pop();
            done.push_back(node);
            
            for(int next : adj[node]) {
                --indegree[next];
                if (indegree[next] == 0) q.push(next);
            }
        }
        return done.size() == numCourses ? true : false;
    }
};
