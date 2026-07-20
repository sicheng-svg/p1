#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <climits>
#include <algorithm>
using namespace std;

class Solution1 {
public:
    int maximumSafenessFactor(vector<vector<int>>& grid) {
        // 1. 多源bfs从小偷出发，计算每个位置的安全系数
        int n = grid.size();
        std::vector<std::vector<int>> dist(n, std::vector<int>(n, -1));
        std::queue<std::pair<int, int>> q;
        // 将小偷入队列，同时开始bfs
        for(int i=0; i<n; ++i)
            for(int j=0; j<n; ++j)
                if(grid[i][j] == 1){ q.push({i, j}); dist[i][j] = 0; /*标记小偷*/ }
        int dr[] = {-1,1,0,0}, dc[] = {0,0,-1,1};
        while(!q.empty()){
            auto [r, c] = q.front(); q.pop();
            for(int i=0; i<4; ++i){
                int nr = r + dr[i], nc = c + dc[i];
                if(nr>=0 && nr<n && nc>=0 && nc<n && dist[nr][nc] == -1){ // -1表示当前位置还没有被访问
                    dist[nr][nc] = dist[r][c] + 1;
                    q.push({nr, nc});
                }
            }
        }
        // 2. 使用二分查找，依次寻找答案
        auto check = [&](int limit) -> bool {
            if(dist[0][0] < limit) return false; // 起始位置的安全系数都小于limit，说明没有路径符合
            vector<vector<bool>> vis(n, vector<bool>(n, false));
            queue<pair<int,int>> bq;
            bq.push({0,0}); vis[0][0] = true;
            while (!bq.empty()) {
                auto [r, c] = bq.front(); bq.pop();
                if (r == n-1 && c == n-1) return true;
                for (int d = 0; d < 4; ++d) {
                    int nr = r+dr[d], nc = c+dc[d];
                    if (nr>=0&&nr<n&&nc>=0&&nc<n&&!vis[nr][nc]&&dist[nr][nc]>=limit) {
                        vis[nr][nc] = true;
                        bq.push({nr, nc});
                    }
                }
            }
            return false;
        };

        int l = 0, r = 2*(n-1), ans = 0;
        while(l <= r){
            int mid = (r-l)/2 + l;
            if(check(mid)) { ans = mid; l = mid + 1; }
            else r = mid - 1;
        }
        return ans;
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

class Solution2 {
public:
    int ans = INT_MAX;
    TreeNode* prev = nullptr;
    void inorder(TreeNode* root){
        if(root == nullptr) return;
        inorder(root->left);
        if(prev) ans = std::min(ans, std::abs(root->val - prev->val));
        prev = root;
        inorder(root->right);
    }

    int getMinimumDifference(TreeNode* root) {
        inorder(root);
        return ans;
    }
};

class Solution3 {
public:
    int kthSmallest(TreeNode* root, int k) {
        std::stack<TreeNode*> st;
        TreeNode* cur = root;
        while(cur || !st.empty()){
            while(cur){
                st.push(cur);
                cur = cur->left;
            }
            cur = st.top(); st.pop();
            if(--k == 0) return cur->val;
            cur = cur->right;
        }
        return -1;
    }
};

class Solution4 {
public:
    TreeNode* prev = nullptr;
    bool check = true;
    void inorder(TreeNode* root){
        if(root == nullptr) return;
        inorder(root->left);
        if(prev && prev->val >= root->val) {check = false; return;}
        prev = root;
        inorder(root->right);
    } 
    bool isValidBST(TreeNode* root) {
        inorder(root);
        return check;
    }
};

class Solution5 {
public:
    // 依次遍历网格，找到一个陆地后开始进行dfs，将其四周所有的陆地都进行标记，依次dfs后，陆地数++
    int m = 0, n = 0;
    void dfs(vector<vector<char>>& grid, int i, int j){
        if (i<0||i>=m||j<0||j>=n||grid[i][j]!='1') return;
        grid[i][j] = '0';
        dfs(grid, i, j+1);
        dfs(grid, i, j-1);
        dfs(grid, i+1, j);
        dfs(grid, i-1, j);
    }
    int numIslands(vector<vector<char>>& grid) {
        m = grid.size();
        n = grid[0].size();
        int ans = 0;
        for(int i=0; i<grid.size(); i++){
            for(int j=0; j<grid[0].size(); ++j){
                if(grid[i][j] == '1'){
                    ans++;
                    dfs(grid, i, j);
                }
            }
        }
        return ans;
    }
};

class Solution6 {
public:
    int m, n;
    void dfs(vector<vector<char>>& board, int i, int j) {
        if (i<0||i>=m||j<0||j>=n||board[i][j]!='O') return;
        board[i][j] = '#';              // 标记为安全（与边界相连）
        dfs(board, i+1, j);
        dfs(board, i-1, j);
        dfs(board, i, j+1);
        dfs(board, i, j-1);
    }
    void solve(vector<vector<char>>& board) {
        if (board.empty()) return;
        m = board.size(); n = board[0].size();
        // 第一步：从四条边界上的O出发，标记所有相连的O为'#'
        for (int i = 0; i < m; ++i) {
            dfs(board, i, 0);           // 左边界
            dfs(board, i, n-1);         // 右边界
        }
        for (int j = 0; j < n; ++j) {
            dfs(board, 0, j);           // 上边界
            dfs(board, m-1, j);         // 下边界
        }
        // 第二步：遍历，O→X（被围），#→O（还原安全的）
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j) {
                if (board[i][j] == 'O') board[i][j] = 'X';
                else if (board[i][j] == '#') board[i][j] = 'O';
            }
    }
};

class Solution7 {
public:
    bool findSafeWalk(vector<vector<int>>& grid, int health) {
        int m = grid.size(), n = grid[0].size();
        vector<vector<int>> dis(m, vector<int>(n, -1));
        int dirs[4][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};

        priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;
        pq.emplace(grid[0][0], 0, 0);
        while (!pq.empty()) {
            auto [val, cx, cy] = pq.top(); 
            pq.pop();
            if (dis[cx][cy] >= 0) {
                continue;
            }
            dis[cx][cy] = val;
            for (int k = 0; k < 4; k++) {
                int nx = cx + dirs[k][0];
                int ny = cy + dirs[k][1];
                if (nx < 0 || ny < 0 || nx >= m || ny >= n || dis[nx][ny] >= 0) {
                    continue;
                }
                pq.emplace(val + grid[nx][ny], nx, ny);
            }
        }

        return dis[m - 1][n - 1] < health;
    }


};

class Solution8 {
public:
    int findMaxPathScore(vector<vector<int>>& edges, vector<bool>& online, long long k) {
        int n = online.size();
        vector<vector<pair<int, int>>> g(n);
        int max_wt = -1;
        for (auto& e : edges) {
            int x = e[0], y = e[1], wt = e[2];
            if (online[x] && online[y]) {
                g[x].emplace_back(y, wt);
                if (x == 0) {
                    max_wt = max(max_wt, wt);
                }
            }
        }

        vector<long long> memo(n);
        auto check = [&](int lower) -> bool {
            ranges::fill(memo, -1); // -1 表示没有计算过

            auto dfs = [&](this auto&& dfs, int x) -> long long {
                if (x == n - 1) { // 到达终点
                    return 0;
                }
                auto& res = memo[x]; // 注意这里是引用
                if (res != -1) { // 之前计算过
                    return memo[x];
                }
                res = LLONG_MAX / 2; // 防止加法溢出
                for (auto& [y, wt] : g[x]) {
                    if (wt >= lower) {
                        res = min(res, dfs(y) + wt);
                    }
                }
                return res;
            };

            return dfs(0) <= k;
        };

        int left = -1, right = max_wt + 1;
        while (left + 1 < right) {
            int mid = left + (right - left) / 2;
            (check(mid) ? left : right) = mid;
        }
        return left;
    }
};


class Solution9 {
public:
    vector<vector<int>> shiftGrid(vector<vector<int>>& grid, int k) {
        int m = grid.size(), n = grid[0].size();
        k %= m*n;

        std::vector<std::vector<int>> next(m, std::vector<int>(n));
        for(int i=0; i<m; ++i){
            for(int j=0; j<n; ++j){
                int idx = i*n + j;
                int nid = (idx + k) % (m*n);
                next[nid/n][nid%n] = grid[i][j];
            }
        }
        return next;
    }
};

/*
// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node() {
        val = 0;
        neighbors = vector<Node*>();
    }
    Node(int _val) {
        val = _val;
        neighbors = vector<Node*>();
    }
    Node(int _val, vector<Node*> _neighbors) {
        val = _val;
        neighbors = _neighbors;
    }
};
*/

class Solution {
public:
    std::unordered_map<Node*, Node*> map;
    // dfs
    Node* _cloneGraph(Node* node) {
        if(!node) return nullptr;
        if(map.count(node)) return map[node];
        Node* clone = new Node(node->val);
        map[node] = clone;
        for(Node* neighbor : node->neighbors){
            clone->neighbors.push_back(cloneGraph(neighbor));
        }
        return clone;
    }
    // bfs
    Node* cloneGraph(Node* node){
        if(!node) return nullptr;
        std::unordered_map<Node*, Node*> map;
        map[node] = new Node(node->val);
        std::queue<Node*> q;
        q.push(node);

        while(!q.empty()){
            Node* cur = q.front(); q.pop();
            for(auto nb: cur->neighbors){
                if(!map.count(nb)){
                    map[nb] = new Node(nb->val);
                    q.push(nb);
                }
                map[cur]->neighbors.push_back(map[nb]);
            }
        }
        return map[node];
    }
};


class Solution {
public:
    vector<double> calcEquation(vector<vector<string>>& equations, 
                                vector<double>& values, 
                                vector<vector<string>>& queries) {
        // 1. 建图，题目给出的两个已知数组，表示的就是一副有向带权图
        std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> graph;
        for(int i=0; i<equations.size(); ++i){
            graph[equations[i][0]].push_back({equations[i][1], values[i]});
            graph[equations[i][1]].push_back({equations[i][0], 1.0 / values[i]}); // 反向，权重为倒数
        }

        // 2. dfs 查找问题对应的节点
        function<double(string&, string&, double, unordered_set<string>&)> dfs = 
        [&](string& cur, string& target, double prod, unordered_set<string>& vis) -> double {
            if (cur == target) return prod;        // 找到了
            vis.insert(cur);
            for (auto& [nb, w] : graph[cur]) {
                if (vis.count(nb)) continue;       // 防止走回头路
                double res = dfs(nb, target, prod * w, vis);
                if (res > 0) return res;           // 找到就直接返回
            }
            return -1.0;
        };

        vector<double> ans;
        for (auto& q : queries) {
            string a = q[0], b = q[1];
            // 变量没出现过 → -1.0
            if (!graph.count(a) || !graph.count(b)) {ans.push_back(-1.0); continue; }
            unordered_set<string> vis;
            ans.push_back(dfs(a, b, 1.0, vis));
        }
        return ans;
    }
};

class Solution {
public:
    // 思路：建图+统计入度，一个课程的入度为0表示它不需要前置条件就可以直接学习
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        std::vector<std::vector<int>> graph(numCourses);
        std::vector<int> indegree(numCourses, 0);
        for(auto& classes : prerequisites){
            int a = classes[0], b = classes[1]; // a依赖b
            graph[b].push_back(a); // b->a
            indegree[a]++;
        }
        std::queue<int> q;
        for(int i=0; i<numCourses; ++i)
            if(indegree[i] == 0) 
                q.push(i);

        int taken = 0;
        while(!q.empty()){
            auto a = q.front(); q.pop();
            taken++; // 学完一门课
            for(int classes: graph[a]){
                if(--indegree[classes] == 0) q.push(classes);
            }
        }
        return taken == numCourses;
    }
};

class Solution {
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        std::vector<std::vector<int>> graph(numCourses);
        std::vector<int> indegree(numCourses, 0);
        for(auto& classes : prerequisites){
            int a = classes[0], b = classes[1]; // a依赖b
            graph[b].push_back(a); // b->a
            indegree[a]++;
        }
        std::queue<int> q;
        for(int i=0; i<numCourses; ++i)
            if(indegree[i] == 0) 
                q.push(i);

        std::vector<int> order;
        while(!q.empty()){
            auto a = q.front(); q.pop();
            order.push_back(a);
            for(int classes: graph[a]){
                if(--indegree[classes] == 0) q.push(classes);
            }
        }
        return order.size() < numCourses ? std::vector<int>() : order;
    }
};

class Solution {
public:
    int snakesAndLadders(vector<vector<int>>& board) {
        int n = board.size();
        // 编号 -> 坐标
        auto num2Rc = [&](int num) -> std::pair<int, int>{
            int idx = num - 1;              // 转换为0~based
            int rowInfront = idx / n;       // 整数第几行
            int r = n - 1 - rowInfront;     // 转换为真正的行号
            int offset = idx % n;           // 行内偏移，列号
            int c = (rowInfront % 2 == 0) ? offset : (n - 1 - offset);//列表S型排列，需要根据奇偶决定列号
            return {r, c};
        };

        // 初始化，从编号1开始进行bfs
        // 用vis记录防止重复访问，第一次走到target就是最短路径
        int target = n*n;
        std::queue<std::pair<int, int>> q; // {编号，步数}
        vector<bool> vis(target + 1, false);
        q.push({1, 0});
        vis[1] = true;

        while(!q.empty()){
            auto [num, step] = q.front(); q.pop();
            if(num == target) return step;
            for(int i=1; i<=6; ++i){ // 投骰子，有6种可能
                int nxt = num + i;
                if(nxt > target) break;
                auto [r, c] = num2Rc(nxt);
                if(board[r][c] != -1) nxt = board[r][c];
                if(!vis[nxt]){
                    vis[nxt] = true;
                    q.push({nxt, step + 1});
                }
            }
        }
        return -1;
    }
};