class LRUCache {
public:
    struct ListNode {
        int val;
        int key;
        ListNode* next;
        ListNode* prev;
        ListNode() : val(0), key(0), next(nullptr), prev(nullptr) {}
        ListNode(int key, int val) : val(val), key(key), next(nullptr), prev(nullptr) {}
    };
    int capacity;
    ListNode* head;
    ListNode* tail;
    unordered_map<int, ListNode*> cacheMap;

    LRUCache(int capacity) {
        this->capacity = capacity;
        head = new ListNode();
        tail = new ListNode();
        head->next = tail;
        tail->prev = head;
    }
    
    int get(int key) {
        auto it = cacheMap.find(key);
        if(it == cacheMap.end()) return -1;

        ListNode* node = it->second;
        moveToHead(node);
        return node->val;
    }
    
    void put(int key, int value) {
        auto it = cacheMap.find(key);
        if(it != cacheMap.end()) {
            ListNode* node = it->second;
            moveToHead(node);
            node->val = value;
            return;
        }
        ListNode* node = new ListNode(key,value);
        addToHead(node);
        cacheMap[key] = node;

        if (cacheMap.size() > capacity){
            ListNode* lru = tail->prev;
            removeNode(lru);
            cacheMap.erase(lru->key);
            delete lru;
        }
    }
    
    void removeNode(ListNode* target) {
        target->next->prev = target->prev;
        target->prev->next = target->next;
    }
    
    void addToHead(ListNode* Node) {
        head->next->prev = Node;
        Node->next = head->next;
        head->next = Node;
        Node->prev = head;
    }

    void moveToHead(ListNode* Node) {
        removeNode(Node);
        addToHead(Node);
    }

};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */
