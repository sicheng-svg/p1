#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <climits>
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