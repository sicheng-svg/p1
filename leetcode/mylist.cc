#include <iostream>

/* 链表节点 */
struct ListNode {
    int val;
    ListNode* _next;
    ListNode* _prev;
    ListNode(int x): val(x), _next(nullptr), _prev(nullptr) {}
};

class MyList {
    private:
        ListNode* sentital; // 哨兵节点
    public:
        MyList(): sentital(){
            sentital = new ListNode(0);
            sentital->_next = sentital;
            sentital->_prev = sentital;
        }
        ~MyList() {
            clear();
            delete sentital;
        }

        void push_back(int x) {
            // head tail newnode
            ListNode* head = sentital->_next;
            ListNode* tail = sentital->_prev;
            ListNode* newNode = new ListNode(x);

            newNode->_next = sentital;
            newNode->_prev = tail;
            tail->_next = newNode;
            sentital->_prev = newNode;
        }

        void insert(int pos, int x) {
            // target newnode target->next
            ListNode* target = find(pos);
            if(target == nullptr) return; // 没有找到目标节点
            ListNode* next = target->_next;

            ListNode* newNode = new ListNode(x);
            newNode->_next = next;
            newNode->_prev = target;
            target->_next = newNode;
            next->_prev = newNode;
        }

        void erase(int x) {
            ListNode* target = find(x);
            if(target == nullptr) return;
            ListNode* prev = target->_prev;
            ListNode* next = target->_next;
            prev->_next = next;
            next->_prev = prev;
            delete target;
        }

        void print(){
            ListNode* cur = sentital->_next;
            while(cur != sentital){
                std::cout << cur->val << " ";
                cur = cur->_next;
            }
            std::cout << std::endl;
        }

        ListNode* find(int x) {
            ListNode* cur = sentital->_next;
            while (cur != sentital) {
                if (cur->val == x) return cur;
                cur = cur->_next;
            }
            return nullptr;
        }

        void clear(){
            ListNode* cur = sentital->_next;
            while(cur != sentital){
                ListNode* next = cur->_next;
                delete cur;
                cur = next;
            }
                sentital->_next = sentital;
                sentital->_prev = sentital;
        }
};

int main() {
    MyList myList;
    myList.push_back(1);
    myList.push_back(2);
    myList.push_back(3);
    myList.print(); // 输出: 1 2 3

    myList.insert(2, 4); // 在值为2的节点后插入4
    myList.print(); // 输出: 1 2 4 3

    myList.erase(2); // 删除值为2的节点
    myList.print(); // 输出: 1 4 3 

    return 0;
}