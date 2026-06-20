/**
 * Definition for singly-linked list.
 * public class ListNode {
 *     int val;
 *     ListNode next;
 *     ListNode() {}
 *     ListNode(int val) { this.val = val; }
 *     ListNode(int val, ListNode next) { this.val = val; this.next = next; }
 * }
 */
class Solution {
    public ListNode oddEvenList(ListNode head) {
        if (head == null || head.next == null) return head;
        ListNode oddN = head;
        ListNode evenN = head.next;
        ListNode evenHead = evenN;

        while (evenN != null && evenN.next != null) {
            oddN.next = evenN.next;
            oddN = oddN.next;
            evenN.next = oddN.next;
            evenN = evenN.next;
        }
        oddN.next = evenHead;
        return head;
    }
}
