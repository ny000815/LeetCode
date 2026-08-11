class Solution {
public:
    int subtractProductAndSum(int n) {
        int num = n;
        int prod = 1;
        int sum = 0;
        while (num > 0) {
            prod *= num%10;
            num /= 10;
        }
        num = n;
        while (num > 0) {
            sum += num%10;
            num /= 10;
        }
        return prod-sum;
    }
};
