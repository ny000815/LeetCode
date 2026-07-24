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
    bool isSubtree(TreeNode* root, TreeNode* subRoot) {
        if (!subRoot) return true;
        if (!root) return false;
        if (isSameTree(root, subRoot)) return true;
        return isSubtree(root->right, subRoot) || isSubtree(root->left, subRoot);
    }
    bool isSameTree(TreeNode* root, TreeNode* subRoot) {
        if(!root && !subRoot) return true;
        if (root && subRoot && root->val == subRoot->val) {
            return isSameTree(root->right, subRoot->right) && isSameTree(root->left, subRoot->left);
        }else return false;
    }
};
