class Solution {
    int pow(long long x, long long y){
        if(x * x == y) return 0;
        if(x * x < y) return -1;
        else return 1;
    }
public:
    int mySqrt(int x) {
        int l = 0, r = x;
        while(l <= r) {
            int mid = l + (r-l)/2;
            int res = pow(mid, x);
            if(res == 0) return mid;
            else if(res == -1) l = mid+1;
            else r = mid-1;
        }
        return r;
    }
};
