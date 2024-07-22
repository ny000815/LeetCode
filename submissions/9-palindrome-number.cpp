class Solution {
public:
    bool isPalindrome(int x) {
        if (x < 0 || (x % 10 == 0 && x != 0)){ 
            return false;
        }
        int revertedNum = 0;
        while (revertedNum < x){
            revertedNum = revertedNum * 10 + x %10;
            x /= 10;
        }
        if (x == revertedNum || x == revertedNum / 10){
            return true;
        }else{
            return false;
        }
    }
};
