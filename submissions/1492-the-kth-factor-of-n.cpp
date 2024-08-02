class Solution {
public:
    int kthFactor(int n, int k) {
        vector<int> factors;
        for (int i = 1; i <= sqrt(n); i++){
            if (n%i == 0){
                factors.push_back(i);
                if (i != (n/i)) factors.push_back(n/i);
            }
        }
        sort(factors.begin(), factors.end());
        if(factors.size() < k) return -1;
        return factors[k - 1];
    }
};
