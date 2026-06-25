/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    bool isSymmetric(TreeNode* root) {
        return isSame(root->right, root->left);
    }
    bool isSame(TreeNode* N1, TreeNode* N2){
        if (N1 == nullptr && N2 == nullptr) return true;
        if (N1 == nullptr || N2 == nullptr) return false;
        if (N1->val != N2->val) return false;
        return (isSame(N1->right, N2->left) && isSame(N1->left, N2->right));
    }
};
