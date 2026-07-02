/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    vector<vector<int>> spiralMatrix(int m, int n, ListNode* head) {
        vector<vector<int>> matrix(m, vector<int> (n, -1));
        int top = 0, left = 0;
        int right = n-1, bottom = m-1;
        ListNode* current = head;
        while (left <= right && top <= bottom){
            for(int j = left; j <= right && current; ++j) {
                matrix[top][j] = current->val;
                current = current->next;
            }
            ++top;
            for(int i = top; i <= bottom && current; ++i) {
                matrix[i][right] = current->val;
                current = current->next;
            }
            --right;
            if (top <= bottom) {
                for(int j = right; j >= left && current; --j) {
                    matrix[bottom][j] = current->val;
                    current = current->next;
                }
                --bottom;
            }
            if (left <= right) {
                for(int i = bottom; i >= top && current; --i) {
                    matrix[i][left] = current->val;
                    current = current->next;
                }
                ++left;
            }
        }
        return matrix;
    }
};
