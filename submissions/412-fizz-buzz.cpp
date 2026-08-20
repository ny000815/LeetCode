class Solution {
public:
    vector<string> fizzBuzz(int n) {
        vector<string> ans(n);
        for(int i = 0; i<n; ++i){
            bool d3 = (i+1) % 3 == 0;
            bool d5 = (i+1) % 5 == 0;
            if(d3 && d5) ans[i] = "FizzBuzz";
            else if (d3) ans[i] = "Fizz";
            else if (d5) ans[i] = "Buzz";
            else ans[i]= to_string((i+1));
        }
        return ans;
    }
};
