class Solution:
    def sortColors(self, nums: List[int]) -> None:
        """
        Do not return anything, modify nums in-place instead.
        """
        left, right = 0, len(nums)-1
        i = 0
        while i <= right:
            if nums[i] == 0:
                nums[i], nums[left] = nums[left], nums[i]
                i += 1
                left += 1
            elif nums[i] == 1:
                i += 1
            else:
                nums[i], nums[right] = nums[right], nums[i]
                right -= 1

                class Solution {
public:
    int majorityElement(vector<int>& nums) {
        int candidate = 0, count = 0;
        for(int val:nums){
            if(count == 0){
                candidate = val;
            }
            count = candidate == val ? count+1: count-1;
        }
        return candidate;
    }
};

class Solution:
    def singleNumber(self, nums: List[int]) -> int:
        ans = 0
        for i in nums:
            ans ^= i
        return ans

        class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        int n = nums.size();
        int i = n - 2;
        while(i >= 0 && nums[i] >= nums[i+1]){
            i--;
        }
        if(i>=0){
            int j = n - 1;
            while(nums[j] <= nums[i]) {
                j--;
            }
            std::swap(nums[i], nums[j]);
        }
        std::sort(nums.begin() + i + 1, nums.end());
    }
};

class Solution:
    def findDuplicate(self, nums: List[int]) -> int:
        slow, fast = nums[0], nums[nums[0]]
        while slow != fast:
            slow = nums[slow]
            fast = nums[nums[fast]]
        slow = 0
        while slow != fast:
            slow = nums[slow]
            fast = nums[fast]
        return slow

class Solution:
    def maxSubArray(self, nums: List[int]) -> int:
        # dp[i]表示以i结尾的最大连续子数组的和
        # dp[i] = max(dp[i-1]+nums[i], nums[i])
        cur_sum = max_sum = nums[0]
        for num in nums[1:]:
            cur_sum = max(cur_sum+num, num)
            max_sum = max(max_sum, cur_sum)
        return max_sum

class Solution:
    def merge(self, intervals: List[List[int]]) -> List[List[int]]:
        intervals.sort()
        ans = []
        for cur in intervals:
            if ans and ans[-1][1] >= cur[0]:
                ans[-1][1] = max(ans[-1][1], cur[1])
            else:
                ans.append(cur)
        return ans

        class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        int n = nums.size();
        k %= n;
        std::reverse(nums.begin(), nums.end());
        std::reverse(nums.begin(), nums.begin() + k);
        std::reverse(nums.begin() + k, nums.end());
    }
};

class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int size = nums.size();
        std::vector<int> result(size, 1);

        for(int i=1; i<size; i++) result[i] = result[i-1] * nums[i-1];
        int tmp = 1;
        for(int i = size - 2; i>=0; i--) {
            tmp *= nums[i+1];
            result[i] *= tmp;
        }
        return result;
    }
};

class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        // 借助前缀积 和 后缀积 解决
        // 前缀积 [0, i)位置的积
        // 后缀积 [i+1, end]
        int n = nums.size();
        std::vector<int> ans(n, 1);
        // 前缀积
        for(int i=1; i<n; ++i){
            ans[i] = ans[i-1]*nums[i-1];
        }

        // 后缀积
        int suffix = 1;
        for(int i=n-2; i>=0; --i){
            suffix *= nums[i+1];
            ans[i] *= suffix;
        }
        return ans;
    }
};

class Solution:
    def firstMissingPositive(self, nums: List[int]) -> int:
        n = len(nums)
        for i in range(n):
            if nums[i] <= 0:
                nums[i] = n + 1

        for i in range(n):
            num = abs(nums[i])
            if num <= n:
                nums[num-1] = -abs(nums[num-1])

        for i in range(n):
            if nums[i] > 0:
                return i+1
        return n+1  
    
        class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        bool isrow = false, iscol = false;
        // 判断第一行和第一列是否含0
        for(int i=0; i<m; ++i){
            if(matrix[i][0] == 0) iscol = true;
        }
        for(int j=0; j<n; ++j){
            if(matrix[0][j] == 0) isrow = true;
        }

        // 从中间开始，进行标记
        for(int i=1; i<m; ++i){
            for(int j=1; j<n; ++j){
                if(matrix[i][j] == 0) {
                    matrix[0][j] = 0;
                    matrix[i][0] = 0;
                }
            }
        } 

        // 将标记的位置全部变成0
        for(int i=1; i<m; ++i){
            for(int j=1; j<n; ++j){
                if(matrix[0][j] == 0 || matrix[i][0] == 0){
                    matrix[i][j] = 0;
                }
            }
        }

        // 最后判断第一行和第一列是否含0，如果有则变0
        if(isrow)
            for(int j=0; j<n; ++j)
                matrix[0][j] = 0;
        if(iscol)
            for(int i=0; i<m; ++i)
                matrix[i][0] = 0;
    }
};

class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        std::vector<int> ans;
        std::stack<TreeNode*> stk;
        TreeNode* cur = root;

        while(cur || !stk.empty()){
            while(cur){
                stk.push(cur);
                cur = cur->left;
            }

            cur = stk.top(); stk.pop();
            ans.push_back(cur->val);
            cur = cur->right;
        }
        return ans;
    }
};

class Solution {
public:
    // 广度优先遍历
    int maxDepth(TreeNode* root) {
        if(root == nullptr) return 0;
        std::queue<TreeNode*> floor;
        int ans = 0;
        floor.push(root);
        while(!floor.empty()){
            int sz = floor.size();
            while(sz--){
                TreeNode* head = floor.front();floor.pop();
                if(head->left)
                    floor.push(head->left);
                if(head->right)
                    floor.push(head->right);
            }
            ans++;
        }
        return ans;
    }
};

class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if(root==nullptr) return nullptr;
        TreeNode* cur = root->left;
        root->left = root->right;
        root->right = cur;
        invertTree(root->left);
        invertTree(root->right);
        return root;
    }
};

class Solution:
    def isSymmetric(self, root: Optional[TreeNode]) -> bool:
        q = deque([(root.left, root.right)])
        while q:
            l, r = q.popleft()
            if not l and not r: continue
            if not l or not r: return False
            if l.val != r.val: return False

            q.append((l.left, r.right))
            q.append((l.right, r.left))
        return True


    # 递归
    def _isSymmetric(self, root: Optional[TreeNode]) -> bool:
        def check(left, right: Optional[TreeNode]) -> bool:
            if not left and not right: return True
            if not left  or not right: return False
            return left.val == right.val and check(left.left, right.right) and check(left.right, right.left)
        return check(root.left, root.right)

        class Solution {
public:
    int ans = 0;
    int depth(TreeNode* node){
        if(node == nullptr) return 0;
        int l = depth(node->left);
        int r = depth(node->right);
        ans = std::max(l+r, ans);
        return std::max(l, r) + 1;
    }
    int diameterOfBinaryTree(TreeNode* root) {
        depth(root);
        return ans;
    }
};

class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        if(root == nullptr) return {};
       std::vector<std::vector<int>> ans;
       std::queue<TreeNode*> q;
       q.push(root);
       while(!q.empty()){
            int sz = q.size();
            std::vector<int> tmp;
            while(sz--){
                auto top = q.front(); q.pop();
                tmp.push_back(top->val);
                if(top->left) q.push(top->left);
                if(top->right) q.push(top->right);
            }
            ans.push_back(tmp);
       }
       return ans;
    }
};

class Solution:
    def sortedArrayToBST(self, nums: List[int]) -> Optional[TreeNode]:
        def build(nums, left, right):
            if left > right:
                return None
            mid = (left+right)//2
            node = TreeNode(nums[mid])
            node.left = build(nums, left, mid-1)
            node.right = build(nums, mid+1, right)
            return node
        return build(nums, 0, len(nums)-1)

        class Solution {
public:
    bool check(TreeNode* node, long lo, long hi){
        if(node == nullptr) return true;
        if(node->val <= lo || node->val >= hi) return false;
        return check(node->left, lo, node->val) && check(node->right, node->val, hi);
    }
    bool isValidBST(TreeNode* root) {
        return check(root, LONG_MIN, LONG_MAX);
    }
};

class Solution {
public:
    int kthSmallest(TreeNode* root, int k) {
        std::stack<TreeNode*> stk;
        TreeNode* cur = root;

        while(cur || !stk.empty()){
            // 走到最左节点
            while(cur){
                stk.push(cur);
                cur = cur->left;
            }
            // 处理根
            cur = stk.top(); stk.pop();
            if(--k == 0) return cur->val;
            cur = cur->right; // 走右子树的左节点
        }
        return -1;
    }

    std::vector<int> nums;
    void dfs(TreeNode* node){
        if(node == nullptr) return ;
        dfs(node->left);
        nums.push_back(node->val);
        dfs(node->right);
    }
    int _kthSmallest(TreeNode* root, int k) {
        dfs(root);
        return *(nums.begin()+k-1);
    }
};


class Solution {
public:
    vector<int> rightSideView(TreeNode* root) {
        std::queue<TreeNode*> q;
        std::vector<int> ans;
        if(root == nullptr) return ans;
        q.push(root);
        while(!q.empty()){
            int sz = q.size();

            while(sz--){
                auto top = q.front(); q.pop();
                if(top->left)
                    q.push(top->left);
                if(top->right)
                    q.push(top->right);
                if(sz == 0){
                    ans.push_back(top->val);
                }
            }
        }
        return ans;
    }
};

class Solution {
public:
    void dfs(TreeNode* root, int depth, std::vector<int>& ans){
        if(root == nullptr) return;
        if(depth == static_cast<int>(ans.size())) ans.push_back(root->val);
        dfs(root->right, depth+1, ans);
        dfs(root->left, depth+1, ans);
    }

    vector<int> rightSideView(TreeNode* root) {
        std::vector<int> ans;
        dfs(root, 0, ans);
        return ans;
    }
};
public:
    void dfs(TreeNode* root, TreeNode*& prev){
        if(root == nullptr) return;
        dfs(root->right, prev);
        dfs(root->left, prev);
        root->right = prev;
        root->left = nullptr;
        prev = root;
    }
    void flatten(TreeNode* root) {
        TreeNode* prev = nullptr;
        dfs(root, prev);
    }
};

class Solution {
public:
    TreeNode* build(std::vector<int>& pre, std::unordered_map<int, int>& inMap, 
    int preLeft, 
    int preRight, 
    int inLeft, 
    int inRight){
        // 前序数组的第一个就是当前子树的根
        if(preLeft > preRight) return nullptr;
        int rootVal = pre[preLeft];
        int idx = inMap[rootVal];
        int leftSize = idx - inLeft; // 在中序数组中，查看根左边有多少个

        // 构造节点
        TreeNode* root = new TreeNode(rootVal);
        root->left = build(pre, inMap, preLeft + 1, preLeft + leftSize, inLeft, idx - 1);
        root->right = build(pre, inMap, preLeft + leftSize + 1, preRight, idx + 1, inRight);
        return root;
    }
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        std::unordered_map<int, int> inMap;
        for(int i=0; i<inorder.size(); ++i) inMap[inorder[i]] = i;

        return build(preorder, inMap, 0, preorder.size() - 1, 0, inorder.size() - 1);
    }
};

class Solution {
public:
    std::unordered_map<long long, int>prefixSum;
    int dfs(TreeNode* root, long long curSum, int target){
        if(!root) return 0;
        curSum += root->val;
        int count = prefixSum[curSum-target];

        prefixSum[curSum]++;
        count += dfs(root->left, curSum, target);
        count += dfs(root->right, curSum, target);
        // 完成当前节点，回到上一个节点时，需要将该节点的前缀和删除
        prefixSum[curSum]--;
        return count;
    }

    int pathSum(TreeNode* root, int targetSum) {
        prefixSum[0] = 1;
        return dfs(root, 0, targetSum);
    }
};

class Solution:
    def lowestCommonAncestor(self, root: 'TreeNode', p: 'TreeNode', q: 'TreeNode') -> 'TreeNode':
        if not root or root == p or root == q:
            return root
        
        left = self.lowestCommonAncestor(root.left, p, q)
        right = self.lowestCommonAncestor(root.right, p, q)
        
        if left and right:
            return root
        return left if left else right

class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        /* 1. 将wordlist转换为哈希表，方便后续O(1)查找 */
        std::unordered_set<std::string> wordSet(wordList.begin(), wordList.end());

        /* 如果目标字符串不在wordset中，这不可能完成转换*/
        if(wordSet.count(endWord) == 0) return 0;

        /* 2. 隐式图，依次将beginWord 中每个位置的元素进行变换，查找变换后的字符是否在set中出现*/
        std::queue<std::string> q;
        const std::string a2z = "abcdefghijklmnopqrstuvwxyz";
        int level = 1; // 起始单词也算一个
        q.push(beginWord);
        while(!q.empty()){
            int sz = q.size();
            while(sz--){
                std::string word = q.front(); q.pop();
                if(word == endWord) return level;

                /* 3.依次将word的每一位都进行枚举a~z*/
                for(int i=0; i<word.size(); ++i){
                    char origin = word[i];
                    for(char ch: a2z){
                        if(ch == origin) continue; // 枚举字符与原始字符一致，不需要继续走
                        word[i] = ch;
                        if(wordSet.count(word)) {
                            q.push(word);
                            wordSet.erase(word);
                        }
                    }
                    word[i] = origin; // 恢复当前字符，枚举下一个位置
                }
            }
            ++level;
        }
        return 0;
    }
};

class Solution {
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        /* 1. 使用邻接表方式建图， 同时统计每个顶点的入度*/
        std::vector<std::vector<int>> adj(numCourses);
        std::vector<int> inDegree(numCourses, 0);
        for(const auto& edge: prerequisites){
            adj[edge[1]].push_back(edge[0]);
            ++inDegree[edge[0]];
        }

        /* 2. 所有入度为0的点进队列*/
        std::queue<int> q;
        for(int i=0; i<inDegree.size(); ++i){
            if(inDegree[i] == 0) {
                q.push(i);
            }
        }

        /* 3. 开始学习，依次消费入度为0的顶点，随后让其的邻居入度-1
        如果在这个过程中入度变为0，则要插入到队列中*/
        /* tip: 维护一个已经学习过的数组，如果发现学习过的数量已经大于课程数量，则return false*/
        int hasLearned = 0;
        while(!q.empty()){
            int sz = q.size();
            hasLearned += sz;
            while(sz--){
                /* 4. 本轮中全是入度为0的顶点，在邻接表中找到对应的邻居列表，将其中所有的入度-1
                如果入度变为0，则要加入到队列中*/
                int vertex = q.front(); q.pop();
                for(int& neighbor: adj[vertex]){
                    inDegree[neighbor]--;
                    if(inDegree[neighbor] == 0) q.push(neighbor);
                }
            }
        }
        return hasLearned == numCourses;
    }
};

class Solution {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int maxArea = 0;

        for(int i=0; i<m; ++i){
            for(int j=0; j<n; ++j){
                if(grid[i][j] == 1){
                    int area = dfs(grid, i, j, m, n);   
                    maxArea = std::max(maxArea, area);            
                }
            }
        }
        return maxArea;
    }
    int dfs(std::vector<std::vector<int>>& grid, int i, int j, int m, int n){
        if(i<0 || i>=m || j<0 || j>=n || grid[i][j] != 1) return 0;

        grid[i][j] = 0; // 标记当前为已访问
        return 1 + dfs(grid, i+1, j, m, n) + 
        dfs(grid, i-1, j, m, n) + 
        dfs(grid, i, j-1, m, n) +
        dfs(grid, i, j+1, m, n);
    }
};

class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        int count = 0;

        for(int i=0; i<m; ++i){
            for(int j=0; j<n; ++j){
                if(grid[i][j] == '1'){
                    // 是陆地，开始dfs，将附近所有的陆地都遍历一遍
                    bfs(grid, i, j, m, n);
                    count++;
                }
            }
        }
        return count;
    }
    void bfs(std::vector<std::vector<char>>& grid, int i, int j, int m, int n){
        std::queue<std::pair<int, int>> q;
        q.push({i, j});
        grid[i][j] = '0';

        int dirs[4][4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        while(!q.empty()){
            auto [a, b] = q.front();q.pop();
            for(int di=0; di<4; ++di){
                int ni = a + dirs[di][0];
                int nb = b + dirs[di][1];
                if(ni >= 0 && ni < m && nb >= 0 && nb < n && grid[ni][nb] == '1'){
                    grid[ni][nb] = '0';
                    q.push({ni, nb});
                }
            }
        }
    }

    int _numIslands(vector<vector<char>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        int count = 0;

        for(int i=0; i<m; ++i){
            for(int j=0; j<n; ++j){
                if(grid[i][j] == '1'){
                    // 是陆地，开始dfs，将附近所有的陆地都遍历一遍
                    dfs(grid, i, j, m, n);
                    count++;
                }
            }
        }
        return count;
    }
    void dfs(std::vector<std::vector<char>>& grid, int i, int j, int m, int n) {
        if(i<0 || i>=m || j<0 || j>=n || grid[i][j] != '1'){
            return;
        }
        // 标记当前为已访问
        grid[i][j] = '0';
        // 递归访问附近的所有陆地
        dfs(grid, i+1, j, m, n);
        dfs(grid, i-1, j, m, n);
        dfs(grid, i, j-1, m, n);
        dfs(grid, i, j+1, m, n);
    }
};

class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int freshCount = 0, mins = 0;
        std::queue<std::pair<int, int>> q;
        /* 1. 找到所有腐烂的橘子加入到队列中，同时进行bfs，并且记录新鲜橘子的个数*/
        for(int i=0; i<m; ++i){
            for(int j=0; j<n; ++j){
                if(grid[i][j] == 2){
                    q.push({i,j});
                }else if(grid[i][j] == 1){
                    freshCount++;
                }
            }
        }
        /* 如果刚开始就没有新鲜橘子，则不需要传染*/
        if(freshCount == 0) return 0;

        /* 2. 所有腐烂的橘子同时开始进行传染 */
        int dirs[4][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
        while(!q.empty()){
            /* 3. 一次性将本次所有的腐烂橘子都执行一遍传染*/
            int sz = q.size();
            int infected = false;
            while(sz--){
                auto [a, b] = q.front(); q.pop();
                for(auto dir: dirs){
                    int ni = a + dir[0];
                    int nj = b + dir[1];
                    if(ni >= 0 && ni < m && nj >= 0 && nj < n && grid[ni][nj] == 1){
                        grid[ni][nj] = 2;
                        q.push({ni, nj});
                        freshCount--;
                        infected = true;
                    }
                }               
            }
            if(infected) mins++;
        }  
        return freshCount == 0 ? mins : -1;
    }
};:wchar_t：w:wchar_t

class Solution:
    def canFinish(self, numCourses: int, prerequisites: List[List[int]]) -> bool:
        # 1. 使用邻接表建图，并统计每个顶点的入度
        adj = defaultdict(list)
        inDegree = [0] * numCourses

        for a, b in prerequisites:
            adj[b].append(a)
            inDegree[a] += 1
        
        # 2. 将入度为0的顶点加入到顶点中
        q = deque( i for i in range(numCourses) if inDegree[i] == 0)

        # 3.开始学习，从入度为0的开始学习，最后要么将所有的课程学完，要么有的课程怎么也学不到。提前返回
        hasLearned = 0
        while q:
            u = q.popleft()
            hasLearned += 1
            for v in adj[u]: # 找到课程u的邻居，将其的入度-1，如果入度为0，则添加进队列
                inDegree[v] -= 1
                if inDegree[v] == 0:
                    q.append(v)
        return hasLearned == numCourses


        class Trie {
public:
    Trie() :root(new TrieNode){}
    
    void insert(string word) {
        TrieNode* node = root;
        for(char c: word){
            int idx = c - 'a'; // 计算当前字符对应的下标
            if(node->children[idx] == nullptr){
                node->children[idx] = new TrieNode;
            }
            node = node->children[idx]; // 走到当前元素的位置，从这个位置开始找下一个字符
        }
        /* word遍历完成后，此时node处于最后一个字符的节点，标记其为end */
        node->isEnd = true;
    }
    
    bool search(string word) {
        /* search 则要精准找到word，word最后一个字符对应的节点isend必须为true*/
        TrieNode* node = findNode(word);
        return node != nullptr && node->isEnd;
    }
    
    bool startsWith(string prefix) {
        TrieNode* node = findNode(prefix);
        return findNode(prefix) != nullptr;
    }
private:
    struct TrieNode {
        TrieNode* children[26] = {nullptr};  // 默认全部 nullptr
        bool isEnd = false;
    };
    TrieNode* root;

    TrieNode* findNode(const string& s)const {
        TrieNode* node = root;
        for(char c: s){
            int idx = c - 'a';
            if(node->children[idx] == nullptr) return nullptr; // 当前字符没有下一个节点，但word还没结束
            node = node->children[idx];
        }
        return node;
    }
};

/**
 * Your Trie object will be instantiated and called as such:
 * Trie* obj = new Trie();
 * obj->insert(word);
 * bool param_2 = obj->search(word);
 * bool param_3 = obj->startsWith(prefix);
 */