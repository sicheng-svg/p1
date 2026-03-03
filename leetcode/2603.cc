#include <vector>
#include <queue>
#include <deque>

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