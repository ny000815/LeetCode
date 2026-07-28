class Solution {
public:
    bool isHappy(int n) {
        unordered_set<int> seen;
        seen.insert(1);
        while(!seen.count(n)) {
            seen.insert(n);
            string number = to_string(n);
            int num = 0;
            for(char c : number) {
                num += (c - '0') * (c - '0');
            }
            n = num;
        }
        return (n == 1);

    }
};
