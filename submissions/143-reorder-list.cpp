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
    void reorderList(ListNode* head) {
        ListNode* fast = head;
        ListNode* slow = head;
        while(fast->next && fast->next->next){
            fast = fast->next->next;
            slow = slow->next;
        }

        ListNode* cur = slow->next;
        ListNode* prev = slow->next = nullptr;
        while(cur){
            ListNode* tmp = cur->next;
            cur->next = prev;
            prev = cur;
            cur = tmp;
        }

        ListNode* odd = head;
        ListNode* even = prev;
        while(even) {
            ListNode* tmp1 = odd->next;
            ListNode* tmp2 = even->next;
            odd->next = even;
            even->next = tmp1;
            odd = tmp1;
            even = tmp2;
        }
    }
};
