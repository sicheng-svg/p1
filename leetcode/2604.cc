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