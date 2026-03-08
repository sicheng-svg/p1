#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <unordered_map>
#include <algorithm>
#include <iostream>

class Solution {
public:
    int _tribonacci(int n) {
        std::vector<int> dp{0, 1, 1};
        for(int i=3; i<=n; ++i){
            int tmp = dp[i-1] + dp[i-2] + dp[i-3];
            dp.emplace_back(tmp);
        }
        return dp[n];
    }
    // 滚动数组的方式来优化空间
    int tribonacci(int n) {
        if(n == 0) return 0;
        if(n == 1 || n == 2) return 1;
        
        int q = 0, w = 1, e = 1, r = 0;
        while(n - 3 >= 0) {
            r = q + w + e;

            q = w;
            w = e;
            e = r;
            --n;
        }
        return r;
    }
};


class Solution2 {
public:
    int waysToStep(int n) {
        const int tmp = 1000000007;
        if(n <= 1) return 1;
        if(n == 2) return 2;
        // int q = 1, w = 1, e = 2, r = 0;
        long long q = 1, w = 1, e = 2, r = 0;
        for(int i=3; i<=n; ++i) {
            // r = ((q + w) % tmp + e)%tmp;
            r = (q + w + e) % tmp;

            q = w;
            w = e;
            e = r;
        }
        return r;
    }
    // 对于第0层，不需要考虑，本来就在
    // 对于第1层，1，一布就可以上来
    // 对于第2层，2，从0层上来，或者从1层上来
    // 对于第3层，1+1+2，从0、1、2层都有方式上来
    // 对于第4层，7，对于第4层，无法直接从0层上来，所以得先到达1/2/3，而这些层数的方式已经知道了，再加上到达第4层的即可

    // 所以dp[i]表示的就是到达第i层的所有可能
    // 根据上述推导过程，可以得出，dp[i] = dp[i-1] + dp[i-2] + dp[i-3]
};


class Solution3 {
public:
    int _minCostClimbingStairs(std::vector<int>& cost) {
        int size = cost.size();
        std::vector<int> dp(size + 2);
        dp[0] = dp[1] = 0;
        for(int i=2; i<=size; ++i) {
            // dp[i]表示达到第i层的最小花费
            // 要么从i-1过来，要么从i-2过来
            // 所以dp[i] = min(dp[i-1]+cost[i-1], dp[i-2]+cost[i-2])
            dp[i] = std::min(dp[i-1]+cost[i-1], dp[i-2]+cost[i-2]);
        }
        return dp[size];
    }

    int minCostClimbingStairs(std::vector<int>& cost) {
        int size = cost.size();
        std::vector<int> dp(size);
        dp[size-1] = cost[size-1];
        dp[size-2] = cost[size-2];
        for(int i=size-3; i>=0; --i) {
            // dp[i]表示从i开始，到达楼顶的最小花费
            // 支付cost[i]，走一步，到i+1，到达楼顶
            // 支付cost[i],走两步，到i+2，到达楼顶
            // 所以dp[i] = std::min(dp[i+1] + cost[i+1], dp[i+2] + cost[i+2]);
            dp[i] = std::min(dp[i+1], dp[i+2]) + cost[i];
        }
        return std::min(dp[0], dp[1]);
    }
};


struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution4 {
public:
    int widthOfBinaryTree(TreeNode* root) {
        if(!root) return 0;

        std::queue<std::pair<TreeNode*, unsigned int>> q;
        q.push({root, 0}); // 根节点编号为0
        unsigned int ret = 0;

        while(!q.empty()) {
            int size = q.size();
            unsigned int left_index = q.front().second;
            unsigned int right_index = left_index; // 让最右侧节点的下标默认等于左侧节点下标

            // 出队列的同时将左右孩子入队列
            for(int i=0; i<size; ++i) {
                auto [node, index] = q.front();
                q.pop();
                right_index = index;
                if(node->left) q.push({node->left, 2*index + 1});
                if(node->right) q.push({node->right, 2*index + 2});
            }
            ret = std::max(ret, right_index - left_index + 1);
        }
        return ret;
    }
    // 既然可以将这个二叉树视作满二叉树，那么他就满足堆的下标映射特性
    // 如果根节点从0开始，那么左右孩子的下标分别为2i+1,2i+2
};

class Solution5 {
public:
    std::vector<std::vector<int>> zigzagLevelOrder(TreeNode* root) {
        if(!root) return {};
        std::vector<std::vector<int>> ret;
        std::vector<TreeNode*> q;
        q.emplace_back(root);
        bool is_left_to_right = true;

        while(!q.empty()){
            int n = q.size();
            std::vector<int> tmp(n); // 存储本层结果
            std::vector<TreeNode*> q_copy; // 临时存储下一层的节点

            for(int i=0; i<n; ++i){
                TreeNode* node = q[i];
                int index = is_left_to_right ? i : n - i - 1;
                tmp[index] = node->val;

                if(node->left) q_copy.emplace_back(node->left);
                if(node->right) q_copy.emplace_back(node->right);
            }
            q = q_copy;
            ret.emplace_back(tmp);
            is_left_to_right = !is_left_to_right;
        }
        return ret;
    }
};

class Solution6 {
public:
    std::vector<std::vector<int>> zigzagLevelOrder(TreeNode* root) {
        if(!root) return {};
        std::vector<std::vector<int>> ret;
        std::queue<TreeNode*> q;
        q.push(root);
        bool is_left_to_right = true;

        while(!q.empty()){
            int n = q.size();
            std::deque<int> levelList; // 存储本层结果

            for(int i=0; i<n; ++i){
                TreeNode* node = q.front();
                q.pop();

                if(is_left_to_right){
                    levelList.push_back(node->val);
                }else{
                    levelList.push_front(node->val);
                }

                if(node->left) q.push(node->left);
                if(node->right) q.push(node->right);
            }
            ret.emplace_back(std::vector<int>(levelList.begin(), levelList.end()));
            is_left_to_right = !is_left_to_right;
        }
        return ret;
    }
};


struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution7 {
public:
    void reorderList(ListNode* head) {
        if(head->next == nullptr || head->next->next == nullptr) return;

        // 1.借助快慢指针，找到链表的中间节点
        // 奇数节点，mid = n/2；偶数节点，mid = n/2+1；
        ListNode* mid = getMiddleNode(head);

        // 2.逆置后半部分
        // 使用头插法，进行逆置
        ListNode* l1 = head;
        ListNode* l2 = mid->next;
        mid->next = nullptr; // 分割两个链表
        l2 = reverseList(l2);
        
        // 3.合并两个链表
        head = mergeList(l1, l2);
    }
private:
    ListNode* getMiddleNode(ListNode* head) {
        ListNode* slow = head, *fast = head;
        while(fast != nullptr && fast->next != nullptr){
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }

    ListNode* reverseList(ListNode* head){
        ListNode* newHead = nullptr;
        ListNode* cur = head;
        while(cur) {
            ListNode* next = cur->next;
            cur->next = newHead;
            newHead = cur;
            cur = next;
        }
        return newHead;
    }

    ListNode* mergeList(ListNode* l1, ListNode* l2){
        ListNode* newHead = new(ListNode);
        ListNode* tail = newHead;
        while(l2) {
            tail->next = l1;
            l1 = l1->next;
            tail = tail->next;

            tail->next = l2;
            l2 = l2->next;
            tail = tail->next;
        }
        tail->next = l1;
        tail = newHead->next;
        delete newHead;
        return tail;
    }
};

class Solution8 {
public:
    int firstMissingPositive(std::vector<int>& nums) {
        // 现将所有的负数变为正数
        int n = nums.size();
        for(int& e : nums){
            if(e <= 0) e = n + 1;
        }

        // 遍历数组，将小于N+1的正数映射到下标位置，并将对应的值改为负数
        // 因为有的数可能已经被改为了负数，但是我们还得标记他，所以先计算出正数，然后进行标记
        for(int i=0; i<n; ++i){
            int num = std::abs(nums[i]);
            if(num < n + 1) {
                nums[num - 1] = -std::abs(nums[num - 1]);
            }
        }

        // 最后遍历一次数组，得出答案
        int ret = 0;
        for(int i=0; i<n; ++i){
            if( nums[i] > 0) return i + 1;
        }
        return n + 1;
    }
    // 思路就是用下标来表示1~n这正整数。
    // 让所有负数变为N+1，随后遍历数组，将小于N+1的正数映射到对应的下标位置，也就是x-1.
    // 将该位置的数变为负数。
    // 最后，只需要再遍历一次数组，如果所有都是负数，说明小于N+1的所有正数都出现了，结果就是N+1
    // 反之，第一个正数的下标+1就是结果
};

class Solution9 {
public:
    int numDecodings(std::string s) {
        int n = s.size();
        std::vector<int> dp(n + 1);

        // 初始化
        dp[0] = 1;
        dp[1] = s[0] != '0';

        for(int i=2; i<=n; ++i) {
            if(s[i - 1] != '0') dp[i] += dp[i-1];

            int val = s[i - 1] - '0';
            int prev = s[i - 2] - '0';
            if(prev*10 + val >= 10 && prev*10 + val <= 26) dp[i] += dp[i-2];
        }
        return dp[n];
    }
    int _numDecodings(std::string s) {
        int n = s.size();
        std::vector<int> dp(n);
        int s0 = s[0] - '0';
        int s1 = s[1] - '0';

        // 初始化
        dp[0] = s[0] != '0';
        if(n <= 1) return dp[0];

        if(s[1] != '0' && s[0] != '0') dp[1] += 1;
        if(s0*10 + s1 >= 10 && s0*10 + s1 <= 26) dp[1] += 1;

        for(int i=2; i<=n; ++i) {
            int val = s[i] - '0';
            int prev = s[i-1] - '0';
            if(s[i] != '0') dp[i] += dp[i-1];
            if(prev*10 + val >= 10 && prev*10 + val <= 26) dp[i] += dp[i-2];
        }
        return dp[n - 1];
    }
    // dp[i] 表示第i个位置时，一共有多少种解码方式
    // 到达i位置时，要么直接解码i位置，要么解码i和i-1这两个位置
    // s[i] 在1~9之间，则解码成功，解码方式就等于0~i-1所有的解码总和
    // s[i] 和 s[i-1]一块解码，则数字要在10~26之间，解码成功，两者作为一个字符，就等于0~i-2所有的解码总和 
    // dp[i] = dp[i - 1] + dp[i - 2] 前提是两者都可以解码
};

class Solution10 {
public:
    int uniquePaths(int m, int n) {
        std::vector<std::vector<int>> dp(m + 1,std::vector<int>(n+1));
        dp[0][1] = 1;

        for(int i = 1; i<=m; ++i) {
            for(int j=1; j<=n ; ++j) {
                dp[i][j] = dp[i-1][j] + dp[i][j-1];
            }
        }
        return dp[m][n];
    }
};

class Solution11 {
public:
    std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string>& strs) {
        std::unordered_map<std::string, std::vector<std::string>> hash;
        for(auto& str : strs) {
            std::string tmp = str;
            sort(tmp.begin(), tmp.end());
            hash[tmp].push_back(str);
        }
        std::vector<std::vector<std::string>> ret;
        for(auto& [key, value] : hash) {
            ret.push_back(value);
        }
        return ret;
    }
};

class Node {
public:
    int val;
    std::vector<Node*> children;

    Node() {}

    Node(int _val) {
        val = _val;
    }

    Node(int _val, std::vector<Node*> _children) {
        val = _val;
        children = _children;
    }
};

class Solution12 {
public:
    std::vector<std::vector<int>> levelOrder(Node* root) {
        if(root == nullptr) return {};
        std::vector<std::vector<int>> ret;
        std::queue<Node*> q;
        q.push(root);

        while(!q.empty()) {
            int curLevelSize = q.size();
            std::vector<int> curLevelList;
            curLevelList.reserve(curLevelSize);

            while(curLevelSize--) {
                Node* node = q.front();q.pop();
                curLevelList.emplace_back(node->val);

                for(auto n : node->children) {
                    q.push(n);
                }
            }
            ret.emplace_back(curLevelList);
        }
        return ret;
    }
};

// 定义双链表节点

struct DLikeNode{
    int _key, _value;
    DLikeNode* _prev;
    DLikeNode* _next;
    DLikeNode():_key(0), _value(0), _prev(nullptr), _next(nullptr){}
    DLikeNode(int key, int value):_key(key), _value(value), _prev(nullptr), _next(nullptr){}
};

class LRUCache {
public:
    LRUCache(int capacity):_capacity(capacity), _size(0) {
        // 创建虚拟头节点和尾节点
        _head = new DLikeNode();
        _tail = new DLikeNode();
        // 使之成为双链表
        _head->_next = _tail;
        _head->_prev = _tail;
        _tail->_prev = _head;
        _tail->_next = _head;
    }
    
    int get(int key) {
        int is_exists = _chche.count(key);
        if(!is_exists) return -1;

        DLikeNode* node = _chche[key];
        int value = node->_value;
        del(node);
        DLikeNode* newNode = new DLikeNode(key, value);
        insert(newNode);
        return value;
    }
    
    void put(int key, int value) {
        int is_exists = _chche.count(key);
        if(is_exists){
            // 已经存在，删除原本的，插入一个新的
            DLikeNode* node = _chche[key];
            del(node);
        }

        if(_size + 1 > _capacity) {
            // 如果满了，此时删除倒数第一个节点，将新节点插入到头部
            delLeastRecentlyUsed();
        }
        // key不存在，则新建一个节点，并头插到双链表中
        DLikeNode* newNode = new DLikeNode(key, value);
        _chche[key] = newNode; // 存入哈希表中
        
        // 将新节点头插
        insert(newNode);
    }
private:
    void insert(DLikeNode* newNode){
            newNode->_next = _head->_next;
            newNode->_prev = _head;
            _head->_next->_prev = newNode;
            _head->_next = newNode;
            _size += 1;
            _chche[newNode->_key] = newNode;
    }

    void delLeastRecentlyUsed(){
        del(_tail->_prev);
    }

    void del(DLikeNode* node){
        // 记录待删除节点的前一个和后一个
        DLikeNode* next = node->_next;
        DLikeNode* prev = node->_prev;
        prev->_next = next;
        next->_prev = prev;
        _size -= 1;
        _chche.erase(node->_key);
        delete node;
    }
private:
    std::unordered_map<int, DLikeNode*> _chche;
    DLikeNode* _head;
    DLikeNode* _tail;
    int _size;
    int _capacity;
};

class Solution13 {
public:
    
    bool CheckPermutation(std::string s1, std::string s2) {
        if(s1.length() != s2.length()) return false;
        int hash[26];
        for(char ch : s1) {
            hash[ch-'a'] += 1;
        }

        for(char ch : s2){
            hash[ch - 'a'] -= 1;
            if(hash[ch - 'a'] < 0) return false;
        }
       return true;

        // 排序之后，判断是否相等
        // sort(s1.begin(), s1.end());
        // sort(s2.begin(), s2.end());
        // return s1 == s2;
    }
};

class Solution14 {
public:
    int dx[4] = {1, 0, 0, -1};
    int dy[4] = {0, 1, -1, 0};
    int numIslands(std::vector<std::vector<char>>& grid) {
        // 遍历数组，如果遇到1开始广度优先遍历，将1开始的四周所有1添加到队列中
        // 通时更新vis数组，表示这个位置已经扫描过了。
        // 每进行一次广度优先遍历，就代表有一个陆地

        int row = grid.size();
        int col = grid[0].size();
        std::vector<std::vector<bool>> vis(row, std::vector<bool>(col));
        int ans = 0;

        for(int i=0; i<row; ++i) {
            for(int j=0; j<col; ++j) {
                if(vis[i][j] == false && grid[i][j] == '1'){
                    // 发现陆地
                    ans++;
                    vis[i][j] = true;

                    // 将初始位置添加到队列中
                    std::queue<std::pair<int, int>> q;
                    q.push({i, j});

                    // 开始广度优先遍历
                    while(!q.empty()){
                        auto [r, c] = q.front();
                        q.pop();

                        // 向四周开始发散，将1添加到队列中，并将其vis标记为true
                        for(int z=0; z<4; ++z){
                            int a = r + dx[z], b = c + dy[z];
                            // 坐标合法，判断是否是陆地
                            if(a >=0 && a <row && b >= 0 && b < col
                                && vis[a][b] == false && grid[a][b] == '1'){
                                    vis[a][b] = true;
                                    q.push({a, b});
                            }
                        }
                    }
                }
            }
        }
        return ans;
    }
};

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution15 {
public:
    std::unordered_map<int, TreeNode*> hash;
    std::unordered_map<int, bool> vis;
    void dfs(TreeNode* root) {
        if(root->left){
            hash[root->left->val] = root;
            dfs(root->left);
        }
        if(root->right){
            hash[root->right->val] = root;
            dfs(root->right);
        }
    }

    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        hash[root->val] = nullptr;
        // 首先遍历二叉树，将父子关系保存在哈希表中
        dfs(root);

        // 在哈希表中，反查p的父亲，同时在vis中进行标记，表示该节点是他的祖先
        while(p != nullptr) {
            vis[p->val] = true; // 先讲自己标记，因为自己也是自己的祖先节点
            p = hash[p->val]; // 更新为父节点
        }

        while(q != nullptr) {
            if(vis[q->val]) return q;
            q = hash[q->val]; // 获取父节点
        }
        return nullptr;
    }

    TreeNode* _lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if(root == nullptr || root == q || root == p) {
            return root;
        }

        // 分别去左右子树查找
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);

        if(left == nullptr) return right;
        if(right == nullptr) return left;
        return root;
    }
};

class Solution16 {
public:
    std::vector<int> findMinHeightTrees(int n, std::vector<std::vector<int>>& edges) {
        if(n == 1) return {0};
        // 构建邻接表和度数表
        std::vector<std::vector<int>> adj(n);
        std::vector<int> degree(n);
        for(const auto& edge : edges){
            degree[edge[0]]++;
            degree[edge[1]]++;

            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]);
        }

        // 统计度为1的节点 
        std::queue<int> q;
        for(int i=0; i<n; ++i){
            if(degree[i] == 1) q.push(i);
        }

        // 开始剥洋葱，将度为1的节点删除，同时更新邻接表，并将新的度为1的节点添加到队列中
        while(n > 2) {
            int size = q.size();
            n -= size;

            for(int i=0; i<size; ++i) {
                int node = q.front();
                q.pop();

                for(int del : adj[node]){ // 这里的del是当前度为1节点的邻居。邻居要将这个节点删除掉
                    // 邻居的度也要减少
                    // 如果节点的度变为1，则将其插入到队列中
                    if(--degree[del] == 1){
                        q.push(del);
                    }
                }
            }
        }
        // 队列中剩下的就是中心节点
        vector<int> res;
        while (!q.empty()) {
            res.push_back(q.front());
            q.pop();
        }
        return res;

    }
};

int main(){
    LRUCache cache(2);
    // cache.put(1, 1);
    // cache.put(2, 2);

    // std::cout << "get1: " << cache.get(1) << std::endl;

    // cache.put(3, 3);
    // std::cout << "get2: " << cache.get(2) << std::endl;
    // cache.put(4, 4);
    // std::cout << "get1: " << cache.get(1) << std::endl;
    // std::cout << "get3: " << cache.get(3) << std::endl;
    // std::cout << "get4: " << cache.get(4) << std::endl;

    cache.put(1, 0);
    cache.put(2, 2);
    std::cout << "get1: " << cache.get(1) << std::endl;
    cache.put(3,3 );
    std::cout << "get2: " << cache.get(2) << std::endl;
    cache.put(4, 4);
    std::cout << "get1: " << cache.get(1) << std::endl;
    std::cout << "get3: " << cache.get(3) << std::endl;
    std::cout << "get4: " << cache.get(4) << std::endl;

}