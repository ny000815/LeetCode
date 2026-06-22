class MyHashMap {

    private class Node{
        int key;
        int value;
        Node next;
        Node (int key, int value) {
            this.key = key;
            this.value = value;
        }
    }

    private int SIZE = 10000;
    private Node[] buckets;

    private int hash(int key) {
        return key % SIZE;
    } 

    private Node findPrev(Node bucketHead, int key) {
        Node curr = bucketHead;
        while (curr.next != null && curr.next.key != key) {
            curr = curr.next;
        }
        return curr;
    }

    public MyHashMap() {
        buckets = new Node[SIZE];
    }
    
    public void put(int key, int value) {
        int index = hash(key);
        if (buckets[index] == null) buckets[index] = new Node(-1, -1);
        
        Node prev = findPrev(buckets[index], key);
        if (prev.next != null ) {
            prev.next.value = value;
        } else {
            prev.next = new Node(key, value);
        }
        
    }
    
    public int get(int key) {
        int index = hash(key);
        if (buckets[index] == null) return -1;
        Node prev = findPrev(buckets[index], key);
        if (prev.next == null) return -1;
        return prev.next.value;
    }
    
    public void remove(int key) {
        int index = hash(key);
        if (buckets[index] == null ) {
            return;
        }
        Node prev = findPrev(buckets[index], key);
        if (prev.next!=null){
            prev.next = prev.next.next;
        }
    }
}

/**
 * Your MyHashMap object will be instantiated and called as such:
 * MyHashMap obj = new MyHashMap();
 * obj.put(key,value);
 * int param_2 = obj.get(key);
 * obj.remove(key);
 */
