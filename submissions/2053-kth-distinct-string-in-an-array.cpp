class Solution {
public:
    string kthDistinct(vector<string>& arr, int k) {
        vector<string> distinctArr;
        vector<int> count(arr.size(), 0);

        for (int i = 0; i < arr.size(); i++){
            for (int j = 0; j < arr.size(); j++){
                if (arr[i] == arr[j]) count[i]++;
            }
        }
        for (int i = 0; i < arr.size(); i++){
            if(count[i] == 1) {
                distinctArr.push_back(arr[i]);
            }
        }
        if (k <= distinctArr.size()) return distinctArr[k - 1];
        return "";
    }
};
