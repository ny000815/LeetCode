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
    public ListNode insertGreatestCommonDivisors(ListNode head) {
        ListNode result = head;
        while (head.next != null) {
            int a = head.val;
            int b = head.next.val;
            ListNode newNode = new ListNode(GCD(a,b));
            ListNode nextNode = head.next;
            head.next = newNode;
            newNode.next = nextNode;
            head = head.next.next;
        }
        return result;
    }
    public int GCD(int a, int b) {
        while (a % b != 0) {
            int c = a % b;
            a = b;
            b = c;
        }
        return b;
    }
}
