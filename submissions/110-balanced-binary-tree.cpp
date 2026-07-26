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
    bool isBalanced(TreeNode* root) {
        if(!root) return true;
        bool isValid = true;
        dfs(root, isValid);
        return isValid;
    }
private:
    int dfs(TreeNode* node, bool& isValid) {
        if(!node) return 0;
        int l = dfs(node->left, isValid);
        int r = dfs(node->right, isValid);
        if(abs(l - r) >= 2) isValid = false;
        return 1 + max(l, r);
    }
};
