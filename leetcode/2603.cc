#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <climits>
#include <limits>

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
        std::vector<int> res;
        while (!q.empty()) {
            res.push_back(q.front());
            q.pop();
        }
        return res;

    }
};

class Solution17 {
public:
    int climbStairs(int n) {
        std::vector<int> dp(n+1);
        dp[0] = 1;
        dp[1] = 1;
        for(int i=2; i<=n; ++i) {
            dp[i] = dp[i-1] + dp[i-2];
        }
        return dp[n];
    }
    // dp[i] 表示到达第i层一共多少种方法
    // dp[i] = dp[i-1] + dp[i-2] 要么从前一个位置走一步过来，要么从前两个位置走两步过来
};

class Solution18 {
public:
    std::vector<std::vector<int>> generate(int numRows) {
        std::vector<std::vector<int>> vv(numRows);
        for(int i=0; i<numRows; ++i){
            vv[i].resize(i+1, 1); // 设置容量并初始化为1
        }
        for(int i=2; i<numRows; ++i){
            for(int j=1; j<vv[i].size() - 1; ++j) {
                vv[i][j] = vv[i-1][j-1] + vv[i-1][j];
            }
        }
        return vv;
    }
};

class Solution19 {
public:
    int maxProfit(std::vector<int>& prices) {
        int size = prices.size();
        std::vector<int> dp(size + 1);
        int minprice = prices[0];

        dp[0] = 0;
        for(int i=1; i<size; ++i) {
            minprice = std::min(minprice, prices[i]);
            dp[i] = std::max(dp[i-1], prices[i] - minprice);
        }
        return dp[size-1];
    }
    // dp[i]表示前i天的最大利润
    // dp[i] = max(dp[i-1], dp[i] - minprice)
    // 所以，dp[i]要么还是昨天的利润，要么就是prices[i]-minprice
};

class Solution20 {
public:
    int fib(int n) {
        if(n < 1) return 0;
        int a = 0, b = 1, c = 1;
        for(int i=3; i<=n; ++i){
            a = b;
            b = c;
            c = a + b;
        }
        return c;
    }
    // dp[i]表示第i个斐波那契数
    // dp[i] = dp[i-1] + dp[i-2];
};

class Solution21 {
public:
    int maxProfit(std::vector<int>& prices) {
        int n = prices.size();
        int dp[n][2];
        dp[0][0] = 0, dp[0][1] = -prices[0];
        for(int i=1; i<n; ++i){
            dp[i][0] = std::max(dp[i-1][0], dp[i-1][1] + prices[i]);
            dp[i][1] = std::max(dp[i-1][1], dp[i-1][0] - prices[i]);
        }
        return dp[n-1][0];
    }
    // 因为可以多次进行买卖股票，但是同一天内，手上最多只能有一只股票
    // dp[i][0]表示第i天，手上没有股票时的最大利润；dp[i][1]表示第i天，手上还有一支股票的最大利润
    // dp[i][0] = max(dp[i-1][0], dp[i-1][1] + prices[i])
    // dp[i][1] = max(dp[i-1][1], dp[i-1][0] - prices[i])
};


class Solution22 {
public:
    int eatenApples(std::vector<int>& apples, std::vector<int>& days) {
        int n = apples.size();
        int ans = 0;

        std::priority_queue<std::pair<int,int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
        for(int i=0; i<n || !pq.empty(); ++i){
            // 1、当天有苹果，将该苹果和过期时间插入
            if(i < n && apples[i] > 0){
                pq.push({i + days[i], apples[i]});
            }
            // 2.将过期的食物踢出去
            while(!pq.empty() && pq.top().first <= i){
                pq.pop();
            }

            // 3.吃掉最快过期的
            if(!pq.empty()) {
                auto [expired, count] = pq.top();
                pq.pop();

                if(count - 1 > 0) {
                    pq.push({expired, count - 1});
                }
                ans++;
            }
        }
        return ans;
    }
};

class Solution23 {
public:
    int storeWater(std::vector<int>& bucket, std::vector<int>& vat) {
        int n = bucket.size();
        // 找到最大的限额，执行这么多次一定满足所有限额要求
        int maxVat = *std::max_element(vat.begin(), vat.end()); 
        if(maxVat == 0) return 0;
        
        //int res = INT_MAX;
        int res = std::numeric_limits<int>::max();
        for(int k=1; k<=maxVat && k<res; ++k){
            // 用来记录在当前次数下，需要进行多少次升级操作
            int t = 0;
            // 先对bucket进行升级
            for(int i=0; i<bucket.size(); ++i){
                t += std::max(0, (vat[i]+k-1)/k - bucket[i]);
            }
            res = std::min(res, t+k);
        }
        return res;
    }
    // bucket[i]表示第i个水缸所配备水桶的大小
    // vat[i]表示第i个水缸的最低水平线
    // 需要让所有的水缸都满足 >= vat[i]
    // 解题思路：假设一共进行了K次蓄水操作。那么bucket[i] 必须满足 vat[i]/k
    // 而bucket[i]的升级次数 = max(0, vat[i]/k - bucket[i])
    // 所以我们依次枚举所有可能的k，在其基础上，获取所有桶的升级次数，随后+上k就是本次的最小操作数
    // 但是计算升级操作时，可能不是整数。此时要采取向上取整，因为bucket[i]表示到水量，可以多到，不能少到
};

class Solution24 {
public:
    int nthSuperUglyNumber(int n, std::vector<int>& primes) {    
        std::vector<long> dp(n);
        dp[0] = 1;
        std::vector<int> pointers(primes.size()); // 下标表示primes，内容指向dp
        for(int i=1; i<n; ++i){
            // 找到每个生产线所能产出的最小的丑数
            long tmp = LONG_MAX;
            for(int j=0; j<primes.size(); ++j){
                tmp = std::min(tmp, dp[pointers[j]] * primes[j]);
            }
            // 保存丑数
            dp[i] = tmp;

            // 去重，更新指针
            for(int j=0; j<primes.size(); ++j){
                if(dp[pointers[j]] * primes[j] == tmp) pointers[j]++; // 指向下一个丑数
            }
        }
        return dp[n-1];
    }

    // dp[i]表示第i个丑数。为了按照升序，找到第n个丑数，我们必须让找到的丑数一个比一个大。
    // 新的丑数 = 旧丑数 * primes中的某个质数。
    // 为了每一次找到的丑数最小。我们使用一个pointes数组，该数组与primes数组对应位置进行结合，用来得出丑数
    // 每一列是一个生产线。表示当前的丑数 * primes
    // 如果找到了一个最小的丑数。那么该位置的pointer就得++，指向下一个丑数。
    // 在此过程中，要进行去重。如果某几列得出了同一个最小丑数，此时这些位置都要指向下一个丑数。
    // 否则，没有移动的指针，接下来还会计算出上一次算出的丑数。
};

class Solution25 {
public:
    int numSquares(int n) {
        std::vector<long> dp(n+1, std::numeric_limits<int>::max());
        dp[0] = 0;
        for(int i=1;i<=n; ++i){
            for(int j=0; j*j<=i; ++j){
                dp[i] = std::min(dp[i], dp[i-j*j]+1);
            }
        }
        return dp[n];
    }
};

class Solution26 {
public:
    bool wordBreak(std::string s, std::vector<std::string>& wordDict) {
        std::unordered_set<std::string> hash(wordDict.begin(), wordDict.end());
        int n = s.size();
        std::vector<bool> dp(n+1, false);
        dp[0] = true;
        
        for(int i=1; i<=n; ++i){
            for(int j=0; j<i; ++j){
                if(dp[j] && hash.find(s.substr(j, i-j)) != hash.end()){
                    dp[i] = true;
                    break;
                }
            }
        }
        return dp[n];
    }
    // dp[i]表示s的前i个字符是否可以被dict中的表示
    // 将dp[i]从某个地方切开，比如j。如果dp[j]可以被表示，并且j~i-1的能在字典中找到。说明dp[i]就可以被表示
};

class Solution27 {
public:
    int lengthOfLIS(std::vector<int>& nums) {
        int n = nums.size();
        std::vector<int> dp(n, 1);
        for(int i=0; i<n; ++i){
            for(int j=0; j<i; ++j){
                if(j<i && nums[j] < nums[i]){
                    dp[i] = std::max(dp[i], dp[j]+1);
                }
            }
        }
        return *std::max_element(dp.begin(), dp.end());
    }
    // dp[i]第i个数，并且符合递增
    // dp[i] = nums[i] > dp[i-1]?nums[i]:dp[i]
    // 结束后可能会有多个重复。去重后返回size
};

class Solution28 {
public:
    int _maxProduct(std::vector<int>& nums) {
        int n = nums.size();
        std::vector<int> dpMax(n);
        std::vector<int> dpMin(n);
        dpMax[0] = nums[0];
        dpMin[0] = nums[0];

        for(int i=1; i<n; ++i){
            dpMax[i] = std::max({nums[i], dpMax[i-1]*nums[i], dpMin[i-1]*nums[i]});
            dpMin[i] = std::min({nums[i], dpMax[i-1]*nums[i], dpMin[i-1]*nums[i]});
        }
        return *std::max_element(dpMax.begin(), dpMax.end());
    }

    int maxProduct(std::vector<int>& nums) {
        // 因为dp数组只依赖前一个的值，所以可以进行空间优化，使用两个变量来存储dp表
        int n = nums.size();
        int dpMax = nums[0];
        int dpMin = nums[0];
        int res = nums[0];

        for(int i=1; i<n; ++i){
            // 保存上一次的最大和最小
            int tmpMax = dpMax;
            int tmpMin = dpMin;
            // 更新这一次的
            dpMax = std::max({nums[i], tmpMax*nums[i], tmpMin*nums[i]});
            dpMin = std::min({nums[i], tmpMax*nums[i], tmpMin*nums[i]});
            res = std::max(res, dpMax);
        }
        return res;
    }
};

class Solution29 {
public:
    bool canPartition(std::vector<int>& nums) {
        int sum = 0;
        for(int val:nums) sum += val;
        if(sum%2 != 0) return false;

        int target = sum / 2;
        std::vector<int> dp(target+1, false);
        dp[0] = true;
        for(int num:nums){
            for(int j=target; j>=num; --j){
                if(dp[j-num]){
                    dp[j] = true;
                }
            }
        }
        return dp[target];
    }
    // 要想满足两个子集的和相等，则数组的和必须是偶数，如果不是偶数则直接返回false
    // 所以我们的目标就是找到一个子集，和为target即可
    // dp[i]表示是否有子集满足和为i
    // 我们才去倒叙遍历数组，dp[i] = dp[i] || dp[i-num]，如果我们选了num，则构成i就只需要i-num为真即可。
};

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    // 哈希表存储一个链表节点，在遍历另一个链表，第一个出现的就是相交节点。
    // 如果没有找到，则返回nullptr
    ListNode *_getIntersectionNode(ListNode *headA, ListNode *headB) {
        std::unordered_set<ListNode*> hash;
        ListNode* cur = headA;
        while(cur){
            hash.emplace(cur);
            cur = cur->next;
        }

        cur = headB;
        while(cur){
            if(hash.count(cur)) return cur;
            cur = cur->next;
        }
        return nullptr;
    }

    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if(!headA || !headB) return nullptr;
        ListNode *pA = headA, *pB = headB;
        while(pA != pB){
            pA = pA ? pA->next : headB;
            pB = pB ? pB->next : headA;
        }
        return pA;
    }
};

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
    // 虚拟头节点
    ListNode* _reverseList(ListNode* head) {
        if(!head) return nullptr;
        ListNode *dummy = new ListNode;
        ListNode* next = head->next;
        while(head){
            head->next = dummy->next;
            dummy->next = head;
            head = next;
            if(next) next = next->next;
        }
        head = dummy->next;
        delete dummy;
        return head;
    }

    // 直接使用指针交换
    ListNode* reverseList(ListNode* head) {
        ListNode *prev = nullptr, *cur = head;
        while(cur){
            ListNode* next  = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next;
        }
        return prev;
    }
};

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
    bool isPalindrome(ListNode* head) {
        // 1.先使用快慢指针，找到中间节点
        // 如果是奇数，则slow就是中间节点；如果是偶数，则是中间靠右的节点
        ListNode *fast = head, *slow = head;
        while(fast && fast->next) {
            fast = fast->next->next;
            slow = slow->next;
        }

        // 2.然后反转后半部分
        ListNode *prev = nullptr, *cur = slow->next;
        while(slow){
            ListNode* next = slow->next;
            slow->next = prev;
            prev = slow;
            slow = next;
        }
        // 3.同时从开始和中间部分，如果后半部分能走到结尾则说明是回文链表
        cur = head;
        while(prev) {
            if(cur->val != prev->val) return false;
            cur = cur->next;
            prev = prev->next;
        }
        return true;
    }
};

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    bool hasCycle(ListNode *head) {
        ListNode *fast = head, *slow = head;
        while(fast && fast->next){
            fast = fast->next->next;
            slow = slow->next;
            if(fast == slow) return true;
        }   
        return false;
    }
};

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode *_detectCycle(ListNode *head) {
        // 使用哈希表存储每一个访问过的节点。第一个重复访问的节点就是入环节点
        std::unordered_set<ListNode*> visited;
        while(head) {
            if(visited.count(head)) return head;
            visited.emplace(head);
            head = head->next;
        }
        return nullptr;
    }

    ListNode *detectCycle(ListNode *head) {
        ListNode *fast = head, *slow = head;
        while(fast && fast->next) {
            fast = fast->next->next;
            slow = slow->next;

            // 说明是环形链表，相遇后，head和slow同时走，相遇位置就是入环点
            if(fast == slow){
                while(slow != head) {
                    slow = slow->next;
                    head = head->next;
                }
                return slow;
            }
        }
        return nullptr;
    }
};

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
    // 递归
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        if(!list1 || !list2) return list1 ? list1 : list2;

        if(list1->val < list2->val){
            list1->next = mergeTwoLists(list1->next, list2);
            return list1;
        }else{
            list2->next = mergeTwoLists(list1, list2->next);
            return list2;
        }
    }

    // 迭代
    ListNode* _mergeTwoLists(ListNode* list1, ListNode* list2) {
        if(!list1 || !list2) return list1 ? list1 : list2;

        ListNode *head = nullptr, *tail = nullptr;
        while(list1 && list2){
            if(list1->val <= list2->val){
                if(head == nullptr) {
                    head = list1;
                    tail = head;
                }else{
                    tail->next = list1;
                    tail = tail->next;  
                }
                list1 = list1->next;
            }else{
                if(head == nullptr) {
                    head = list2;
                    tail = head;
                }else{
                    tail->next = list2;
                    tail = tail->next;  
                }
                list2 = list2->next;
            }
        }

        // 判断那个链表不为空，将剩余部分直接连接在tail后面
        if(list1){
            tail->next = list1;
        }else{
            tail->next = list2;
        }
        return head;
    }
};

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
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode *dummy = new ListNode;
        ListNode *tail = dummy;
        int b = 0;
        while(l1 && l2){
            int sum = l1->val + l2->val + b;
            int a = sum % 10;
            b = sum / 10;       // 进位
            ListNode *newNode = new ListNode(a);
            tail->next = newNode;
            tail = newNode;
            l1 = l1->next;
            l2 = l2->next;
        }
        ListNode *last = l1;
        if(!l1) last = l2;

        while(last){
            int sum = last->val + b;
            ListNode *newNode = new ListNode(sum % 10);
            b = sum / 10;
            tail->next = newNode;
            tail = newNode;
            last = last->next;
        }
        if(b){
            ListNode *newNode = new ListNode(b % 10);
            tail->next = newNode;
            tail = newNode;
        }

        last = dummy->next;
        delete dummy;
        return last;
    }
};

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
    // 使用快慢指针来解决，引入dummy，让fast先走n+1步。
    // 然后slow 和fast同步走，当fast为空时，slow的位置就是待删除的前一个位置
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0, head);
        ListNode *fast = &dummy, *slow = &dummy;
        for(int i=0; i<=n; ++i){
            fast = fast->next;
        }
        while(fast){
            slow = slow->next;
            fast = fast->next;
        }
        ListNode *del = slow->next;
        slow->next = del->next;
        delete del;
        return dummy.next;
    }

    ListNode* _removeNthFromEnd(ListNode* head, int n) {
        if(head->next == nullptr && n == 1) return nullptr;

        // 1.求出一共有多少个节点
        ListNode *cur = head;
        int size = 0;
        while(cur){
            ListNode *next = cur->next;
            size++;
            cur = next;
        }
        // 2.将倒数节点转换为正数
        int target = size - n;
        cur = head;
        ListNode *prev = nullptr;
        while(target--){
            prev = cur;
            cur = cur->next;
        }
        // 此时cur的位置就是待删除的节点
        if(!prev){
            ListNode *next = cur->next;
            delete cur;
            return next;
        }
        prev ->next = cur->next;
        delete cur;
        return head;
    }
};

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
    // 使用递归进行两两交换
    // 我们看待链表，只看当前的两个，以及后面的链
    // 我们相信swapPairs一定可以给我们将后面的链给两两反转。在本次，我们只需要将head和second反转即可
    ListNode* swapPairs(ListNode* head) {
        // 递归终止条件
        if(!head || !head->next) return head;

        ListNode *second = head->next;
        head->next = swapPairs(second->next);
        second->next = head;
        return second;
    }

    ListNode* _swapPairs(ListNode* head) {
        // 借助虚拟头节点
        if(!head || !head->next) return head;

        ListNode *dummy = new ListNode;
        dummy->next = head;

        ListNode *prev = dummy;
        ListNode *cur = head;
        ListNode *next = cur->next;
        ListNode *nnext = next->next;
        while(cur && next){
            prev->next = next;
            next->next = cur;
            cur->next = nnext;

            // 更新指针
            prev = cur;
            cur = nnext;
            if(cur) next = cur->next;
            if(next) nnext = next->next;
        }

        head = dummy->next;
        delete dummy;
        return head;
    }
};


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
    ListNode* reverseKGroup(ListNode* head, int k) {
       ListNode dummy(0, head);
       ListNode *groupPrev = &dummy;
       for(;;){
            // 1.检查够不够k个
            ListNode *check = groupPrev;
            for(int i=0; i<k; ++i){
                check = check->next;
                if(!check) return dummy.next;
            }

            // 2.依次反转k个
            ListNode *prev = nullptr, *cur = groupPrev->next;
            for(int i=0; i<k; ++i){
                ListNode *next = cur->next;
                cur->next = prev;
                prev = cur;
                cur = next;
            }

            // 3. 此时groupPrev是本次的头，要将他变为尾
            ListNode *groupTail = groupPrev->next; // 原本的头，反转后变为尾
            groupTail->next = cur; // 尾连到下一次的头
            groupPrev->next = prev;// 前一组接到反转后的头

            // 4.更新新的一组
            groupPrev = groupTail;
       }
    }
    ListNode* _reverseKGroup(ListNode* head, int k) {
        // 1.先检查够不都k个，不够直接返回
        ListNode *check = head;
        for(int i=0; i<k; ++i){
            while(!check) return head;
            check = check->next;
        }

        // 2.我们无条件相信该函数可以将后面剩余的部分反转好。
        // 我们只需要反转当前的即可。
        ListNode *prev = nullptr, *cur = head;
        for(int i=0; i<k; ++i){
            ListNode *next = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next;
        }

        // 3.此时prev是反转后的新头，head是尾，让head->next指向后面部分
        head->next = reverseKGroup(cur, k);
        return prev;
    }
};

/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* next;
    Node* random;
    
    Node(int _val) {
        val = _val;
        next = NULL;
        random = NULL;
    }
};
*/

class Solution {
public:
    Node* copyRandomList(Node* head) {
        std::unordered_map<Node*, Node*> map;
        for(Node *cur=head; cur; cur=cur->next){
            map[cur] = new Node(cur->val);
        }
        for(Node* cur = head; cur; cur=cur->next){
            map[cur]->next = map[cur->next];
            map[cur]->random = map[cur->random];
        }
        return map[head];
    }

    Node* _copyRandomList(Node* head) {
        if(!head) return head;

        std::unordered_map<Node*, Node*> hash;
        Node dummy(0);
        Node *cur = head, *tail = &dummy;
        while(cur){
            Node *newNode = new Node(cur->val);
            newNode->next = nullptr;
            tail->next = newNode;
            tail = newNode;

            hash[cur] = newNode; // 旧节点 -> 新节点
            cur = cur->next;
        }

        cur = head;
        Node *copy = dummy.next;
        while(cur){
            if(cur->random == nullptr) copy->random = nullptr;
            else{
                copy->random = hash[cur->random]; // cur->random指向的节点，映射的就是我们需要的新random
            }
            cur = cur->next;
            copy = copy->next;
        }
        return dummy.next;
    }
};

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
    ListNode* sortList(ListNode* head) {

    }

    ListNode* _sortList(ListNode* head) {
        std::vector<int> nums;
        ListNode *cur = head;
        while(cur){
            ListNode *next = cur->next;
            nums.emplace_back(cur->val);
            cur = next;
        }

        sort(nums.begin(), nums.end());

        cur = head;
        for(int val:nums){
            ListNode *next = cur->next;
            cur->val = val;
            cur = next;
        }
        return head;
    }
};

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


    // 借助priority_queue，默认是大堆，我们升序需要小堆
    // 先将每一个链表的头节点放入队列中。每一次会取到最小的头结点，然这个节点往后走，插入到队列中
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        auto comp = [](ListNode *l1, ListNode *l2){
            return l1->val > l2->val;
        };
        std::priority_queue<ListNode*, std::vector<ListNode*>, decltype(comp)> q;
        for(auto node:lists){
            if(node) q.push(node);
        }

        ListNode dummy, *tail = &dummy;
        while(!q.empty()){
            auto node = q.top(); q.pop();
            tail->next = node;
            tail = tail->next;
            if(node->next) q.push(node->next);
        }
        return dummy.next;
    }

    ListNode* mergeLists(ListNode *l1, ListNode *l2){
        if(!l1 || !l2) return l1 == nullptr ? l2 : l1;

        ListNode dummy;
        ListNode *tail = &dummy;

        while(l1 && l2){
            if(l1->val <= l2->val){
                tail->next = l1;
                l1 = l1->next;
            }else{
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }

        tail->next = l1 ? l1 :l2;
        return dummy.next;
    }

    // 合并k个，我们可以两个两个合并。用前两个合并的结果，和第三个继续合并，重复这个过程
    ListNode* _mergeKLists(vector<ListNode*>& lists) {
        int n = lists.size();
        if(n < 1) return nullptr;
        if(n == 1) return lists[0];

        ListNode *result = mergeLists(lists[0], lists[1]);
        for(int i=2; i<n; ++i){
            result = mergeLists(result, lists[i]);
        }
        return result;
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
    LRUCache(int capacity):_capacity(capacity) {
        // 创建虚拟头节点和尾节点
        _head = new DLikeNode;
        _tail = new DLikeNode;
        // 使之成为双链表
        _head->_next = _tail;
        _tail->_prev = _head;
    }
    ~LRUCache(){
        while(_head){
            DLikeNode *next = _head->_next;
            delete _head;
            _head = next;
        }
        _head = _tail = nullptr;
    }
    
    int get(int key) {
        int is_exists = _chche.count(key);
        if(!is_exists) return -1;

        // 存在，则保存结果，并接节点插入到头部
        DLikeNode* node = _chche[key];
        int value = node->_value;
        moveToHead(node);
        return value;
    }
    
    void put(int key, int value) {
        int is_exists = _chche.count(key);
        if(is_exists){
            // 已经存在，删除原本的，插入一个新的
            DLikeNode* node = _chche[key];
            node->_value = value;
            moveToHead(node);
            return;
        }

        // key不存在，则新建一个节点，并头插到双链表中
        if(_chche.size() + 1 > _capacity) {
            // 如果满了的话，将最后一个节点，移动到头部，更新k和v
            DLikeNode* node = _tail->_prev;
            _chche.erase(node->_key); // 将旧的映射关系删除
            node->_key = key; node->_value = value;
            moveToHead(node);
            return;
        }
        DLikeNode* newNode = new DLikeNode(key, value);
        // 将新节点头插
        insert(newNode);
    }
private:
    void insert(DLikeNode* node){
        node->_next = _head->_next;
        node->_prev = _head;
        _head->_next->_prev = node;
        _head->_next = node;
        _chche[node->_key] = node;
    }
    void remove(DLikeNode *node){
        DLikeNode *prev = node->_prev;
        DLikeNode *next = node->_next;
        prev->_next = next;
        next->_prev = prev;
    }

    void moveToHead(DLikeNode* node){
        // 将指定节点，迁移至头部
        remove(node);
        insert(node);
    }

    
private:
    std::unordered_map<int, DLikeNode*> _chche;
    DLikeNode* _head;
    DLikeNode* _tail;
    int _capacity;
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */

 class Solution {
public:
    // 建大堆，向下调整建堆，从倒数第一个非叶子节点开始向下调整
    void adjustDown(std::vector<int>& nums, int size, int parent){
        int child = parent*2 + 1;
        int val = nums[parent];
        while(child < size){
            if(child + 1 < size && nums[child] < nums[child + 1]){
                child += 1;
            }

            if(val >= nums[child]) break;
            // 孩子大于父亲，则将孩子上移
            nums[parent] = nums[child];
            parent = child;
            child = parent*2 + 1;
        }
        nums[parent] = val;
    }

    int findKthLargest(vector<int>& nums, int k) {
        // 1.从倒数第一个非叶子节点开始向下调整建堆
        int n = nums.size();
        for(int i=(n-1-1)/2; i>=0; --i){
            adjustDown(nums, n, i);
        }

        // 2.此时所有的数都已大堆的方式存储在数组中。
        // 从堆顶删除k次，最后一次的数就是结果
        int ans = nums[0];
        for(int i=0; i<k; ++i){
            ans = nums[0];

            std::swap(nums[0], nums.back());
            nums.pop_back();       
            adjustDown(nums, nums.size(), 0); 
        }
        return ans;
    }

    int _findKthLargest(vector<int>& nums, int k) {
        std::priority_queue<int> heap(nums.begin(), nums.end());
        int ans = 0;
        while(k--){
            ans = heap.top();heap.pop();
        }
        return ans;
    }
};

class Solution {
public:
    // 建大堆，向下调整建堆，从倒数第一个非叶子节点开始向下调整
    void adjustDown(std::vector<int>& nums, int size, int parent){
        int child = parent*2 + 1;
        int val = nums[parent];
        while(child < size){
            if(child + 1 < size && nums[child] < nums[child + 1]){
                child += 1;
            }

            if(val >= nums[child]) break;
            // 孩子大于父亲，则将孩子上移
            nums[parent] = nums[child];
            parent = child;
            child = parent*2 + 1;
        }
        nums[parent] = val;
    }

    int findKthLargest(vector<int>& nums, int k) {
        // 1.从倒数第一个非叶子节点开始向下调整建堆
        int n = nums.size();
        for(int i=(n-1-1)/2; i>=0; --i){
            adjustDown(nums, n, i);
        }

        // 2.此时所有的数都已大堆的方式存储在数组中。
        // 从堆顶删除k次，最后一次的数就是结果
        int ans = nums[0];
        for(int i=0; i<k; ++i){
            ans = nums[0];

            std::swap(nums[0], nums.back());
            nums.pop_back();       
            adjustDown(nums, nums.size(), 0); 
        }
        return ans;
    }

    int _findKthLargest(vector<int>& nums, int k) {
        std::priority_queue<int> heap(nums.begin(), nums.end());
        int ans = 0;
        while(k--){
            ans = heap.top();heap.pop();
        }
        return ans;
    }
};

class Solution {
public:
    // 3.快排
    vector<int> topKFrequent(vector<int>& nums, int k) {
        // 1.统计出现次数
        std::unordered_map<int, int> map;
        for(int val:nums){
            map[val]++;
        }

        // 2.将剩余的数字提取出来
        std::vector<int> uniq;
        for(auto& [key,value]:map){
            uniq.push_back(key);
        }

        // 3.快排
        int left = 0, right = uniq.size()-1;
        while(left <= right){
            int pivot = map[uniq[right]];
            int storeIdx = left; // storeIdx左边都是大于pivot的
            for(int i=left; i<right; ++i){
                if(map[uniq[i]] > pivot){
                    std::swap(uniq[i], uniq[storeIdx]);
                    storeIdx++;
                }
            }
            std::swap(uniq[storeIdx], uniq[right]);

            if(storeIdx + 1 == k) break;
            else if(storeIdx + 1 < k) left = storeIdx + 1;
            else right = storeIdx - 1;
        }
        return std::vector<int>(uniq.begin(), uniq.begin()+k);
    }

    // 2.借助桶排序
    vector<int> __topKFrequent(vector<int>& nums, int k) {
        // 1.统计出现次数
        std::unordered_map<int, int> map;
        for(int val:nums){
            map[val]++;
        }

        // 2.借助桶排序
        int n = nums.size();
        std::vector<std::vector<int>> bucket(n+1);
        for(auto& [key, value]:map){
            bucket[value].push_back(key);
        }

        // 3.提取结果
        std::vector<int> ans;
        for(int i=n; i>=0 && ans.size() < k; --i){
            for(int val : bucket[i]){
                ans.push_back(val);
            }
        }
        return ans;
    }

    // 1.借助priority_queue
    vector<int> _topKFrequent(vector<int>& nums, int k) {
        // 1.统计出现次数
        std::unordered_map<int, int> map;
        for(int val:nums){
            map[val]++;
        }
        for(auto[key,value]:map){
            std::cout << key <<":"<<value << std::endl;
        }

        std::priority_queue<std::pair<int, int>> heap;
        for(auto [key, value]:map){
            heap.push({value,key});
        }


        std::vector<int> ans(k);
        for(int i=0; i<k; ++i){
            ans[i] = heap.top().second; heap.pop();
        }
        return ans;
    }
};

class MedianFinder {
public:
    MedianFinder() {
        
    }
    
    void addNum(int num) {
        maxHeap.emplace(num); // 数据默认插入左边
        minHeap.emplace(maxHeap.top());
        maxHeap.pop();

        if(minHeap.size() > maxHeap.size()){
            maxHeap.emplace(minHeap.top());
            minHeap.pop();
        }

    }
    
    double findMedian() {
        if(maxHeap.size() == minHeap.size()){
            return (maxHeap.top() + minHeap.top())/2.0;
        }
        return maxHeap.top();
    }
private:
    std::priority_queue<int> maxHeap; // 大堆，存储小于中位数的值
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap; // 小堆，存储大于中位数的值
};

/**
 * Your MedianFinder object will be instantiated and called as such:
 * MedianFinder* obj = new MedianFinder();
 * obj->addNum(num);
 * double param_2 = obj->findMedian();
 */

 class Solution {
public:
    bool isValid(string s) {
        std::stack<char> st;
        for(char c:s){
            if(c == '(' || c == '{' || c == '['){
                st.push(c);
            }
            else{
                if(st.empty()) return false;

                char top = st.top();
                if(c == ')' && top == '(' 
                || c == '}' && top == '{' 
                || c == ']' && top == '[') st.pop();
                else return false;
            }
        }
        return st.empty();
    }
};

class MinStack {
public:
    MinStack() {
    }
    
    void push(int val) {
        if(minSt.empty() || val <= minSt.top()){
            st.push(val);
            minSt.push(val);
        }else{
            st.push(val);
        }
    }
    
    void pop() {
        if(st.top() == minSt.top()) {
            st.pop();
            minSt.pop();
        }else{
            st.pop();
        }
    }
    
    int top() {
        return st.top();
    }
    
    int getMin() {
        return minSt.top();
    }
private:
    std::stack<int> minSt;
    std::stack<int> st;
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(val);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */

 class Solution {
public:
    string decodeString(string s) {
        std::stack<int> nst;
        std::stack<string> sst;
        sst.push("");
        
        int n = s.size();   
        for(int i=0; i<n;){
            if(isdigit(s[i])){
                // 1.如果是数字，则将数字提取出来，放入数字栈
                int tmp = 0;
                int j = 0;
                for(j=i; j<n && isdigit(s[j]); ++j){
                    tmp = tmp*10 + (s[j]-'0');
                }
                i = j; // 更新i
                nst.push(tmp);
            }else if(s[i] == '['){
                // 2.如果是左括号，则将后续的字母压入栈中
                string tmp = "";
                i++;
                int j = 0;
                for(j=i; j<n && s[j] >= 'a' && s[j] <= 'z'; ++j){
                    tmp += s[j];
                }
                i = j;
                sst.push(tmp);
            }else if(s[i] == ']') {
                // 3.如果是右括号，则拿出数字栈顶x，随后将字符串栈顶的s重复x次后，重新压入栈中
                int count = nst.top(); nst.pop();
                string tmp = "";
                while(count--) tmp += sst.top();
                sst.pop();
                sst.top() += tmp;
                i++;
            }else{
                // 4.如果直接是字母，则将后续的字母直接追加到sst的栈顶'
                int j = 0;
                for(j = i; j < n && s[j] >= 'a' && s[j] <= 'z'; ++j){
                    sst.top() += s[j];
                }
                i = j;
            }
        }
        return sst.top();
    }
};

class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        int n = temperatures.size();
        std::vector<int> ans(n, 0);
        std::stack<int> st;

        for(int i=n-1; i>=0; --i){
            // 找到第一个比当前大的
            while(!st.empty() && temperatures[st.top()] <= temperatures[i]){
                st.pop();
            }

            if(!st.empty()){
                ans[i] = st.top() - i;
            }
            st.push(i);
        }
        return ans;
    }
};

class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        int n = heights.size();
        std::stack<int> stk;
        int ans = 0;
        for(int i=0; i<=n; ++i){
            int curHeight = (i == n) ? 0 : heights[i]; 
            while(!stk.empty() && heights[stk.top()] >= curHeight){
                // 左侧>右侧高度,可以计算当前栈中的面积
                int h = heights[stk.top()];stk.pop();
                int left = stk.empty() ? -1 : stk.top();
                int width = i - left - 1;
                ans = std::max(ans, h * width);
            }
            stk.push(i);
        }
        return ans;
    }
};

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int left = 0, right = 0, n = s.size();
        int ans = 0;
        std::unordered_map<char, int> dict;
        while(right < n) {
            // 1.进窗口
            dict[s[right]]++;

            // 2.判断
            while(dict[s[right]] > 1){
                // 3.出窗口
                dict[s[left]]--;
                left++;
            }

            // 3.满足条件更新结果
            ans = std::max(ans, right - left + 1);
            right++; // 走到下一个位置
        }
        return ans;
    }
};

class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        std::unordered_map<char, int> need;
        std::unordered_map<char, int> window;
        for(char ch : p) {
            need[ch]++;
        } 

        int left = 0;
        int vaild = 0;
        std::vector<int> ans;
        for(int right=0; right<s.size(); ++right){
            // 进窗口
            char c = s[right];
            if(need.count(c)){
                window[c]++;
                if(window[c] == need[c]) vaild++;
            }

            // 窗口大小与p.size()一致时，更新答案，并出窗口
            if(right - left + 1 == p.size()){
                // 更新结果
                if(vaild == need.size()){
                    ans.emplace_back(left);
                }

                // 出窗口
                char d = s[left];
                if(need.count(d)){
                    if(window[d] == need[d]){
                        --vaild;
                    }
                    window[d]--;
                }
                left++;
            }
        }
        return ans;
    }
};

class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        // 使用前缀和
        // 当遍历到i位置时，它的前缀和为sum
        // 我们想要找到和为k的left,right的区间，只需要找到前缀和为sum - k的位置即可
        std::unordered_map<int, int> prefix_count;
        prefix_count[0] = 1;
        int ans = 0, sum = 0;
        for(int e : nums){
            sum += e;
            if(prefix_count.count(sum - k)) {
                ans += prefix_count[sum-k];
            }
            prefix_count[sum]++;
        } 
        return ans;
    }
};

class Solution {
public:
    int tribonacci(int n) {
        if(n == 0) return 0;
        if(n <= 2) return 1;
        int a=0, b=1, c=1, d=0;
        for(int i=3; i<=n; ++i){
            d = a + b + c;
            a = b;
            b = c; 
            c = d;
        }
        return d;
    }
};

class Solution {
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

class Solution:
    def numDecodings(self, s: str) -> int:
        n = len(s)
        dp = [0]*(n+1)
        dp[0] = 1
        dp[1] = 1 if s[0] != '0' else 0

        for i in range(2, n + 1):
            val = int(s[i-1])
            prev = int(s[i-2])
            dp[i] += dp[i-1] if val != 0 else 0

            dp[i] += dp[i-2] if 10<=prev*10 + val<=26 else 0
        return dp[n]


        class Solution:
    def uniquePaths(self, m: int, n: int) -> int:
        # dp[i][j]表示走到ij位置时，一共有多少种走法
        # dp[i][j] = dp[i-1][j] + dp[i][j-1]
        dp = [[0]*(n+1) for _ in range(m+1)]
        dp[1][0] = 1
        for i in range(1, m+1):
            for j in range(1, n+1):
                dp[i][j] = dp[i-1][j] + dp[i][j-1]
        return dp[m][n]


class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        int m = obstacleGrid.size();
        int n = obstacleGrid[0].size();
        std::vector<std::vector<int>> dp(m+1, std::vector<int>(n+1, 0));
        dp[0][1] = 1;
        for(int i=1; i<=m; ++i){
            for(int j=1; j<=n; ++j){
                dp[i][j] = obstacleGrid[i-1][j-1] != 1 ? dp[i-1][j] + dp[i][j-1] : 0;
            }
        }
        return dp[m][n];
    }
};

class Solution:
    def jewelleryValue(self, frame: List[List[int]]) -> int:
        m, n = len(frame), len(frame[0])
        dp = [[0]*(n+1) for _ in range(m+1)]
        for i in range(1, m+1):
            for j in range(1, n+1):
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]) + frame[i-1][j-1]
        return dp[m][n]

class Solution {
public:
    int minFallingPathSum(vector<vector<int>>& matrix) {
        int n = matrix.size();
        std::vector<std::vector<int>> dp(n, std::vector<int>(n));
        dp[0] = matrix[0];
        for(int i=1; i<n; ++i){
            for(int j=0; j<n; ++j){
                int l = j-1 >= 0 ? dp[i-1][j-1] : std::numeric_limits<int>::max();
                int m = dp[i-1][j];
                int r = j+1 < n ? dp[i-1][j+1] : std::numeric_limits<int>::max();
                dp[i][j] = std::min({l, m ,r}) + matrix[i][j];
            }
        }
        return *std::min_element(dp[n-1].begin(), dp[n-1].end());
    }
};

class Solution:
    def minPathSum(self, grid: List[List[int]]) -> int:
        m, n = len(grid), len(grid[0])
        dp = [[float('inf')]*(n+1) for _ in range(m+1)]
        dp[0][1] = 0
        for i in range(1, m+1):
            for j in range(1, n+1):
                dp[i][j] = min(dp[i-1][j], dp[i][j-1]) + grid[i-1][j-1]
        return dp[m][n]
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