#include <iostream>

struct ListNode {
    int _x;
    ListNode* _next;
    ListNode* _prev;
    ListNode(int x): _x(x), _next(nullptr), _prev(nullptr){}
};

class myList{
    private:
        ListNode* sentital;
    public:
        myList(){
            sentital = new ListNode(0);
            sentital->_next = sentital;
            sentital->_prev = sentital;
        }

        ~myList(){
            clear();
            delete sentital;
        }

        void push_back(int x){
            ListNode* head = sentital->_next;
            ListNode* tail = sentital->_prev;
            ListNode* newNode = new ListNode(x);

            // head tail newNode
            newNode->_prev = tail;
            newNode->_next = sentital;
            tail->_next = newNode;
            sentital->_prev = newNode;
        }

        void insert(int pos, int x){
            ListNode* target = find(pos);
            if(target == nullptr) return;
            ListNode* next = target->_next;
            ListNode *newNode = new ListNode(x);
            // target newNode target->next
            newNode->_next = target->_next;
            newNode->_prev = target;
            next->_prev = newNode;
            target->_next = newNode;
        }

        void erase(int x) {
            ListNode* target = find(x);
            if (target == nullptr) return;
            ListNode* prev = target->_prev;
            ListNode* next = target->_next;
            prev->_next = next;
            next->_prev = prev;
            delete target;
        }

        void print(){
            ListNode* cur = sentital->_next;
            while(cur != sentital){
                std::cout << cur->_x << " ";
                cur = cur->_next;
            }
            std::cout << std::endl;
        }
    private:
        ListNode* find(int x){
            ListNode* cur = sentital->_next;
            while(sentital != cur){
                if(cur->_x == x) return cur;
                cur = cur->_next;
            }
            return nullptr;
        }

        void clear(){
            ListNode* cur = sentital->_next;
            while(cur != sentital) {
                ListNode* next = cur->_next;
                delete cur;
                cur = next;
            }
            sentital->_next = sentital;
        }
};

int main(){
    myList l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    l.push_back(4);
    l.print(); // 1 2 3 4 

    l.erase(1);
    l.print(); // 2 3 4

    l.insert(3, 100);
    l.print(); // 2 3 100 4
}