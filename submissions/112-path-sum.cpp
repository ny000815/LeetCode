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
    bool hasPathSum(TreeNode* root, int targetSum) {
        return recursion(0, targetSum, root);
    }
    bool recursion(int total, int target, TreeNode* current){
        if (current == nullptr) return false;
        total += current->val;
        if (total == target && current->right == nullptr && current->left == nullptr) return true;
        return recursion(total, target, current->right) || recursion(total, target, current->left);
    }
};
