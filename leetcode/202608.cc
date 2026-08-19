class Solution {
public:
    int dfs(vector<int>& nums, int i, int j) {
        if (i == j) return nums[i];              // 只剩一个，直接拿
        return max(nums[i] - dfs(nums, i+1, j),  // 取左端
                   nums[j] - dfs(nums, i, j-1)); // 取右端
    }
    bool predictTheWinner(vector<int>& nums) {
        return dfs(nums, 0, nums.size() - 1) >= 0;
    }
};

class Solution {
public:
    int _mySqrt(int x) {
        long long k = 0;
        while((k+1)*(k+1) <= x) k++;
        return k;
    }
    int mySqrt(int x) {
        int left = 0, right = x, ans = 0;
        while(left <= right){
            int mid = left + (right - left) / 2;
            if((long long )mid*mid > x) right = mid - 1;
            else{
                ans = mid;
                left = mid + 1;
            }
        }
        return ans;
    }
};

class Solution {
public:
    double quickMul(double x, long long N) {
        if (N == 0) return 1.0;
        double half = quickMul(x, N / 2);          // 先算一半
        return (N % 2 == 0) ? half * half          // 偶数：平方
                            : half * half * x;      // 奇数：平方再乘一个x
    }
    double _myPow(double x, int n) {
        long long N = n;                            // 转long long防溢出
        if (N < 0) { x = 1 / x; N = -N; }          // 负指数：取倒数
        return quickMul(x, N);
    }

    double myPow(double x, int n) {
        long long N = n;
        if (N < 0) { x = 1 / x; N = -N; }    // 负指数转正
        double ans = 1.0;
        while (N > 0) {
            if (N & 1) ans *= x;              // 当前二进制位是1，累乘
            x *= x;                            // x → x²，为下一位准备
            N >>= 1;                           // 看下一位
        }
        return ans;
    }
};

func stoneGame(piles []int) bool {
    n := len(piles)
    dp := make([][]int, n)
    for i := range dp{
        dp[i] = make([]int, n)
        dp[i][i] = piles[i]
    }
    
    for length := 2; length <= n; length++{
            for i := 0; i + length - 1 < n; i++{
                j := i + length - 1
                dp[i][j] = max(piles[i] - dp[i+1][j], piles[j] - dp[i][j-1])
            }
    }
    return dp[0][n-1] > 0
}

class Solution {
public:
    int maxPoints(vector<vector<int>>& points) {
        int n = points.size();
        if (n <= 2) return n;

        int ans = 1;
        for (int i = 0; i < n; ++i) {                 // 枚举锚点
            unordered_map<long long, int> cnt;
            for (int j = 0; j < n; ++j) {
                if (i == j) continue;
                int dx = points[j][0] - points[i][0];
                int dy = points[j][1] - points[i][1];

                int g = __gcd(abs(dx), abs(dy));      // 约分成最简
                dx /= g; dy /= g;
                // 统一符号：dx>0，或 dx==0 时 dy>0
                if (dx < 0 || (dx == 0 && dy < 0)) { dx = -dx; dy = -dy; }

                long long key = 1LL * dx * 100000 + dy;
                ans = max(ans, ++cnt[key] + 1);       // +1 是算上锚点自己
            }
        }
        return ans;
    }
};

class Solution {
public:
    // dp[i] 表示以i结尾一共有多少种方法
    // dp[i] = dp[i-1] + dp[i-2]
    int climbStairs(int n) {
        std::vector<int> dp(n+1);
        dp[0] = 1;
        dp[1] = 1;
        for(int i=2; i<=n; ++i){
            dp[i] = dp[i-1] + dp[i-2];
        }
        return dp[n];
    }
};

class Solution {
public:
    // dp[i]表示到第i间的总现金
    // dp[i] = std::max(dp[i-2] + nums[i], dp[i-1])
    int rob(vector<int>& nums) {
        int n = nums.size();
        if(n == 1) return nums[0];

        std::vector<int> dp(n);
        dp[0] = nums[0];
        dp[1] = std::max(nums[0], nums[1]);
        for(int i=2; i<n; ++i){
            dp[i] = std::max(dp[i-2] + nums[i], dp[i-1]);
        }
        return dp[n-1];
    }
};

class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
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
    // dp[i]表示前i个字符
    // dp[i] = dp[j] == true?
};

class Solution {
public:
    vector<int> findMissingElements(vector<int>& nums) {
        std::sort(nums.begin(), nums.end());
        std::unordered_set<int> s(nums.begin(), nums.end());
        std::vector<int> ans;
        int i = nums[0] + 1;
        while(i < nums.back()){
            if(!s.count(i)) ans.push_back(i);
            i++;
        }
        return ans;
    }
};

class Solution {
public:
    // dp[i]表示凑出金额i所需要的最小的硬币数
    
    int coinChange(vector<int>& coins, int amount) {
        std::vector<int> dp(amount+1, amount+1);
        dp[0] = 0;
        for(int i=1; i<=amount; ++i){
            for(int coin : coins){
                if (i>=coin)
                    dp[i] = std::min(dp[i], dp[i-coin] + 1);
            }
        }
        return dp[amount] != amount+1 ? dp[amount] : -1;
    }
};

class Solution {
public:
    // dp[i]表示以nums[i]结尾的最长严格递增子数组
    // dp[i] = max(dp[j] + 1) dp[j]  0 ~ i-1
    int lengthOfLIS(vector<int>& nums) {
        int n = nums.size();
        vector<int> dp(n, 1);              // 每个元素自成长度1
        int ans = 1;

        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < i; ++j) {   // 回看所有更早的位置
                if (nums[j] < nums[i])       // 能接上（严格递增）
                    dp[i] = max(dp[i], dp[j] + 1);
            }
            ans = max(ans, dp[i]);           // 答案是全局最大，不是dp[n-1]
        }
        return ans;
    }
};

class Solution {
public:
    // dp[i][j]表示从i，j出发，到达最底的最小路径和
    int minimumTotal(vector<vector<int>>& triangle) {
        int n = triangle.size();    
        std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));
        
        for(int j=0; j<n; ++j) dp[n-1][j] = triangle[n-1][j];
        for(int i=n-2; i>=0; --i){
            for(int j=0; j<=i; ++j){
                dp[i][j] = std::min(dp[i+1][j], dp[i+1][j+1]) + triangle[i][j];
            }
        }
        return dp[0][0];
    }
};

class Solution {
public:
    // dp[i][j]表示达到nums[i][j]时的最小路径和
    // dp[i][j] = std::min(dp[i][j-1], dp[i-1][j]) + nums[i][j]
    int minPathSum(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();      // 行、列分开
        const int INF = 1e9;
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, INF));  // padding填INF
        dp[0][1] = 0;                                  // 起点旁开个口子

        for (int i = 1; i <= m; ++i)
            for (int j = 1; j <= n; ++j)
                dp[i][j] = min(dp[i-1][j], dp[i][j-1]) + grid[i-1][j-1];

        return dp[m][n];
    }
};

class Solution {
public:
    // dp[i][j]表示走到grid[i-1][j-1]时的不同路径数
    // dp[i][j] = dp[i-1][j] + dp[i][j-1]
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        int m = obstacleGrid.size(), n = obstacleGrid[0].size();
        std::vector<std::vector<int>> dp(m+1, std::vector<int>(n+1, 0));
        dp[0][1] = 1;
        for(int i=1; i<=m; ++i)
            for(int j=1; j<=n; ++j)
                dp[i][j] = obstacleGrid[i-1][j-1] != 1 ? dp[i-1][j] + dp[i][j-1] : 0;
        return dp[m][n];
    }
};

class Solution {
public:
    vector<int> remainingMethods(int n, int k, vector<vector<int>>& invocations) {
        // 建邻接表
        vector<vector<int>> g(n);
        for (auto& e : invocations)
            g[e[0]].push_back(e[1]);

        // 第一步：BFS标记所有可疑方法（k及其直接/间接调用的）
        vector<bool> suspicious(n, false);
        queue<int> q;
        q.push(k);
        suspicious[k] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) {
                if (!suspicious[v]) {      // 去重，防止环导致死循环
                    suspicious[v] = true;
                    q.push(v);
                }
            }
        }

        // 第二步：检查是否有"非可疑 → 可疑"的调用
        for (auto& e : invocations) {
            if (!suspicious[e[0]] && suspicious[e[1]]) {
                // 无法移除，返回全部方法
                vector<int> all(n);
                iota(all.begin(), all.end(), 0);
                return all;
            }
        }

        // 第三步：可以移除，返回所有非可疑方法
        vector<int> ans;
        for (int i = 0; i < n; ++i)
            if (!suspicious[i]) ans.push_back(i);
        return ans;
    }
};

class Solution {
public:
    string _longestPalindrome(string s) {
        // 中心扩展算法，从中心开始向两边进行扩展
        // 要包含奇数 + 偶数
        int n = s.size();
        int start = 0, maxLen = 1;
        auto expand = [&](int l, int r) -> int {
            while(l >= 0 && r < n && s[l] == s[r]){
                l--; r++;
            }
            return r - l - 1;
        };

        for(int i=0; i<n; ++i){
            int len1 = expand(i, i); // 奇数
            int len2 = expand(i, i+1); // 偶数
            int len = max(len1, len2);
            if(len > maxLen){
                maxLen = len;
                start = i - (len - 1) / 2;
            }
        }
        return s.substr(start, maxLen);
    }
    string longestPalindrome(string s) {
        // 动态规划
        // dp[i][j] 表示区间ij是否是回文串
        // dp[i][j] = dp[i+1][j-1] && s[i] == s[j]
        int n = s.size();
        int start = 0, maxLen = 1;

        std::vector<std::vector<bool>> dp(n, std::vector<bool>(n, false));
        for(int i=0; i<n; ++i) dp[i][i] = true; // 长度为1肯定是回文串

        for(int len=2; len<=n; ++len){
            for(int i=0; i+len-1<n; ++i){
                int j = i+len-1;
                if(s[i] != s[j]) continue;
                if(len == 2) dp[i][j] = true;
                else dp[i][j] = dp[i+1][j-1];
                if(dp[i][j] && len > maxLen){
                    maxLen = len;
                    start = i; 
                }
            }
        }
        return s.substr(start, maxLen);
    }
};

class Solution {
public:
    int smallestNumber(int n, int t) {
        auto getMul = [](int n) ->int{
            int ans = 1;
            while(n){
                ans *= n%10;
                n /= 10;
            }
            return ans;
        };

        int ans = n;
        while(1){
            if(getMul(ans) % t == 0) break;
            ans++;
        }
        return ans;
    }
};

class Solution {
public:
    bool isInterleave(string s1, string s2, string s3) {
        int m = s1.size(), n = s2.size();
        if (m + n != (int)s3.size()) return false;      // 长度对不上，直接false

        vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));
        dp[0][0] = true;

        // 第一列：只用s1
        for (int i = 1; i <= m; ++i)
            dp[i][0] = dp[i-1][0] && s1[i-1] == s3[i-1];
        // 第一行：只用s2
        for (int j = 1; j <= n; ++j)
            dp[0][j] = dp[0][j-1] && s2[j-1] == s3[j-1];

        for (int i = 1; i <= m; ++i)
            for (int j = 1; j <= n; ++j)
                dp[i][j] = (dp[i-1][j] && s1[i-1] == s3[i+j-1])   // 这一位来自s1
                        || (dp[i][j-1] && s2[j-1] == s3[i+j-1]);  // 或来自s2

        return dp[m][n];
    }
};

class Solution {
public:
    // dp[i][j] 表示把 word1 的前 i 个字符转换成 word2 的前 j 个字符的最少操作数
    int minDistance(string word1, string word2) {
        int n = word1.size(), m = word2.size();
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));   // n+1 行 m+1 列

        dp[0][0] = 0;
        for (int j = 1; j <= m; ++j) dp[0][j] = j;   // 空串→前j个：插入j次
        for (int i = 1; i <= n; ++i) dp[i][0] = i;   // 前i个→空串：删除i次

        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= m; ++j) {
                if (word1[i-1] == word2[j-1])
                    dp[i][j] = dp[i-1][j-1];         // 字符相同，白嫖
                else
                    dp[i][j] = min({dp[i-1][j],      // 删除
                                    dp[i][j-1],      // 插入
                                    dp[i-1][j-1]})   // 替换
                             + 1;
            }
        }
        return dp[n][m];
    }
};

class Solution {
public:
    // 状态dp，对于每一天来说，有5种状态：什么也没干，买了第一支，卖了，买了第二支，买了
    // 每一天都要维护这5种状态
    int _maxProfit(vector<int>& prices) {
        int n = prices.size();
       std::vector<std::vector<int>> dp(n, std::vector<int>(5)); 
       dp[0][0] = 0; // 什么也不干
       dp[0][1] = -prices[0]; // 持第一笔股
       dp[0][2] = 0; // 卖掉
       dp[0][3] = -prices[0]; // 持第二笔股
       dp[0][4] = 0; // 卖掉
       for(int i=1; i<n; ++i) { 
           dp[i][0] = dp[i-1][0];
           dp[i][1] = std::max(dp[i-1][1], dp[i-1][0] - prices[i]);
           dp[i][2] = std::max(dp[i-1][2], dp[i-1][1] + prices[i]);
           dp[i][3] = std::max(dp[i-1][3], dp[i-1][2] - prices[i]);
           dp[i][4] = std::max(dp[i-1][4], dp[i-1][3] + prices[i]);
       }
        return std::max({dp[n-1][0], dp[n-1][2], dp[n-1][4]});
    }

    // 滚动数组
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        int buy1 = -prices[0], sell1 = 0;
        int buy2 = -prices[0], sell2 = 0;
        for(int i=1; i<n; ++i) { 
            buy1 = std::max(buy1, -prices[i]);
            sell1 = std::max(sell1, buy1 + prices[i]);
            buy2 = std::max(buy2, sell1 - prices[i]);
            sell2 = std::max(sell2, buy2 + prices[i]);
        }
        return sell2;
    }
};

class Solution {
public:
    // buy[j] 手上拿到底j次股票，现在的余额
    // sell[j] 卖出第j次股票，现在的余额
    int maxProfit(int k, vector<int>& prices) {
        int n = prices.size();
        k = std::min(k, n/2);
        std::vector<int> buy(k+1, -prices[0]);
        std::vector<int> sell(k+1, 0);
        for(int i=1; i<n; ++i){
            int price = prices[i];
            for(int j=1; j<=k; ++j){
                buy[j] = std::max(buy[j], sell[j-1] - price);
                sell[j] = std::max(sell[j], buy[j] + price);
            }
        }
        return sell[k];
    }
};

class Solution {
public:
    vector<int> validSequence(string word1, string word2) {
        int n = word1.size(), m = word2.size();
        // 1. 预处理suf数组，suf[i]表示word1[i~n-1]可以匹配word2的几个字符
        std::vector<int> suf(n+1, 0);
        int j = m - 1;
        for(int i=n-1; i>=0; --i){
            if(j >= 0 && word1[i] == word2[j]) --j;
            suf[i] = m - 1 - j;
        }

        // 2. 从word1开始依次匹配word2的字符，如果相同，记录下来。
        // 不相同，则要判断是否需要换，要换的话，需要判断后面的字符能不能凑出word2的后面字符
        std::vector<int> ans;
        j = 0;
        bool isChanged = false;
        for(int i=0; i<n && j<m; ++i){
            if(word1[i] == word2[j]){
                ans.push_back(i);
                j++;
            }else if(!isChanged && suf[i+1] >= m - 1 - j){
                isChanged = true;
                ans.push_back(i);
                j++;
            }
        }
        return j == m ? ans : std::vector<int>();
    }
};

class Solution {
public:
    // 用一个栈来记录前几天的问题，保证前一天肯定在最上面。
    // 然后遍历后面的温度，如果栈不为空，且当前温度，大于前一天的温度，那么比前一天大的就是今天
    // 而且，要一直判断，因为这天可能也是前面几天的答案。
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        int n = temperatures.size();
        std::vector<int> ans(n, 0);
        std::stack<int> st;

        for(int i=0; i<n; ++i){
            while(!st.empty() && temperatures[i] > temperatures[st.top()]){
                int idx = st.top();st.pop();
                ans[idx] = i-idx;
            }
            st.push(i);
        }
        return ans;
    }
};

class Solution {
public:
    int stoneGameII(vector<int>& piles) {
        int n = piles.size();
        vector<int> suf(n + 1, 0);
        for (int i = n - 1; i >= 0; --i) suf[i] = suf[i+1] + piles[i];

        // dp[i][m]: 从第i堆开始、M=m 时，当前玩家能拿的最大石子数
        vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));

        for (int i = n - 1; i >= 0; --i) {
            for (int m = 1; m <= n; ++m) {
                if (i + 2 * m >= n) {              // 能一次全拿走
                    dp[i][m] = suf[i];
                    continue;
                }
                for (int X = 1; X <= 2 * m; ++X)   // 枚举取几堆
                    dp[i][m] = max(dp[i][m],
                                   suf[i] - dp[i + X][max(m, X)]);
            }
        }
        return dp[0][1];
    }
};
class Solution {
public:
    bool winnerSquareGame(int n) {
        std::vector<bool> dp(n+1, false);
        for(int i=1; i<=n; ++i){
            for(int k=1; k*k<=i; ++k){
                if(!dp[i-k*k]){
                    dp[i] = true;
                    break;
                }
            }
        }
        return dp[n];
    }
};

class Solution {
public:
    int missingInteger(vector<int>& nums) {
        int i = 1, n = nums.size();
        int sum = nums[0];
        while(i < n && nums[i] == nums[i-1] + 1){
            sum += nums[i];
            i++;
        }
        std::unordered_set<int> mp(nums.begin(), nums.end());
        while(mp.count(sum)) sum++;
        return sum;
    }
};

class Solution {
public:
    vector<int> nextGreaterElements(vector<int>& nums) {
        int n = nums.size();
        std::vector<int> ans(n, -1);
        std::stack<int> st;

        for(int i=0; i<2*n; ++i){
            int cur = nums[i%n];
            while(!st.empty() && nums[st.top()] < cur){
                ans[st.top()] = cur;
                st.pop();
            }
            if(i < n) st.push(i);
        }
        return ans;
    }
};

class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        int maxArea = 0;
        heights.push_back(0);
        int n = heights.size();


        std::stack<int> stk;
        for(int i=0; i<n; ++i){
            while(!stk.empty() && heights[stk.top()] > heights[i]){
                int cur = stk.top(); stk.pop();  
                int left = stk.empty() ? -1 : stk.top();
                maxArea = std::max(maxArea, (i-left-1)*heights[cur]);
            }
            stk.push(i);
        }
        return maxArea;
    }
};
// 对于每一个高度的柱子来说，分别找到左边和右边第一个比它矮的柱子，两个柱子之间就是该高度柱子所能围成的最大面积
// 然后对每一个柱子都进行该操作，返回最大的面积即可
// 重点就落在了如何找到左右第一个比它矮的柱子：借助单调栈，不一定要按照顺序计算高度，而是按照弹出的顺序。
// 栈为空 || 栈顶元素 < 当前元素，入栈
// 栈不为空 && 栈顶元素 > 当前元素，说明，以栈顶元素为高的柱子，右边第一个比他小的就是i
// 弹栈后，下一个栈顶，就是第一个比柱子小的左边柱子。因为如果大于的话，就会先把它处理了。

class Solution {
public:
    int maxSubarrayLength(vector<int>& nums, int k) {
        std::unordered_map<int, int> cnt;
        int left = 0, right = 0;
        int n = nums.size();
        int ans = 1;
        while(right < n){
            // 进窗口
            cnt[nums[right]]++;
            while(cnt[nums[right]] > k){
                // 出窗口
                cnt[nums[left++]]--;
            }
            // 更新结果
            ans = std::max(ans, right - left + 1);
            right++;
        }
        return ans;
    }
};
// 借助滑动窗口 + 哈希表，维护一个满足要求的最长子数组
// 一个元素进入窗口后，判断当前窗口维护的子数组是否满足要求
// 1. 满足要求，更新长度
// 2. 不满足要求，左边界出窗口，直到重新满足

class Solution {
public:
    int trap(vector<int>& height) {
        int n = height.size();
        std::stack<int> stk;

        int ans = 0;
        for(int i=0; i<n; ++i){
            while(!stk.empty() && height[stk.top()] < height[i]){
                int cur = stk.top(); stk.pop();
                if(stk.empty()) break; // 没有左墙
                int left = stk.top();
                int width = i - left - 1;
                int h = std::min(height[left], height[i]) - height[cur];
                ans += width * h;
            }
            stk.push(i);
        }
        return ans;
    }
};

class Solution {
    vector<int> pre, suf, best;
    string str;

    // 用两个子节点的信息，算出父节点的信息
    void pull(int p, int l, int mid, int r) {
        int lenL = mid - l + 1, lenR = r - mid;
        pre[p]  = pre[2*p];
        suf[p]  = suf[2*p+1];
        best[p] = max(best[2*p], best[2*p+1]);

        if (str[mid] == str[mid+1]) {                   // 边界字符相同才能拼
            best[p] = max(best[p], suf[2*p] + pre[2*p+1]);   // 跨界段
            if (pre[2*p] == lenL)   pre[p] = lenL + pre[2*p+1];
            if (suf[2*p+1] == lenR) suf[p] = lenR + suf[2*p];
        }
    }

    void build(int p, int l, int r) {
        if (l == r) { pre[p] = suf[p] = best[p] = 1; return; }  // 单字符
        int mid = (l + r) / 2;
        build(2*p, l, mid);
        build(2*p+1, mid+1, r);
        pull(p, l, mid, r);
    }

    void update(int p, int l, int r, int idx) {
        if (l == r) return;                    // 叶子的三个值恒为1
        int mid = (l + r) / 2;
        if (idx <= mid) update(2*p, l, mid, idx);
        else            update(2*p+1, mid+1, r, idx);
        pull(p, l, mid, r);                     // 回溯时重新合并
    }

public:
    vector<int> longestRepeating(string s, string queryCharacters,
                                  vector<int>& queryIndices) {
        str = s;
        int n = s.size(), k = queryCharacters.size();
        pre.assign(4*n, 0); suf.assign(4*n, 0); best.assign(4*n, 0);
        build(1, 0, n - 1);

        vector<int> ans;
        for (int i = 0; i < k; ++i) {
            str[queryIndices[i]] = queryCharacters[i];   // 改字符
            update(1, 0, n - 1, queryIndices[i]);        // 只更新一条路径
            ans.push_back(best[1]);                       // 根节点就是答案
        }
        return ans;
    }
};

class StockSpanner {
public:
    StockSpanner() {}
    
    int next(int price) {
        int span = 1;
        while(!stk.empty() && price >= stk.top().first){// 栈不为空 && 今天价格大于等于昨天价格
            span += stk.top().second;
            stk.pop();
        }
        stk.emplace(price, span);
        return span;
    }
private:
    std::stack<std::pair<int, int>> stk;
};
// 如果今天的价格大于昨天的价格，那么昨天永远不会作为未来某一天的阻挡
// 未来的某一天要么被今天阻挡，要么这两天都不会被阻挡

/**
 * Your StockSpanner object will be instantiated and called as such:
 * StockSpanner* obj = new StockSpanner();
 * int param_1 = obj->next(price);
 */

 class Solution {
public:
    int maximumLengthSubstring(string s) {
        int n = s.size();
        std::unordered_map<char, int> cnt;

        int left = 0, right = 0;
        int ans = 0;
        while(right < n){
            // 进窗口
            cnt[s[right]]++;
            // 判断
            while(cnt[s[right]] > 2){
                // 出窗口
                cnt[s[left]]--;
                left++;
            }
            ans = std::max(ans, right-left+1);
            right++;
        }
        return ans;
    }
};

class Solution {
public:
    int maxNumberOfFamilies(int n, vector<vector<int>>& reservedSeats) {
        std::unordered_map<int, int> dict;
        for(auto& r: reservedSeats){
            int row = r[0], seat = r[1];
            dict[row] |= (1 << (seat-1)); // 将列号转化为第几位二进制
        }

        const int A = 0b0000011110;
        const int B = 0b0001111000;
        const int C = 0b0111100000;
        int ans = 0;
        for(auto& [row, mask]: dict){
            bool freeA = (mask & A) == 0;
            bool freeB = (mask & B) == 0;
            bool freeC = (mask & C) == 0;
            if(freeA && freeC) ans += 2;
            else if(freeA || freeB || freeC) ans++;
        }
        return ans + (n - dict.size())*2;
    }
};

class Solution {
public:
    int maxSumMinProduct(vector<int>& nums) {
        const int MOD = 1e9 + 7;
        int n = nums.size();
        std::vector<long long> prefix(n+1, 0);
        for(int i=0; i<n; ++i) prefix[i+1] = prefix[i] + nums[i];

        long long maxMinProduct = 0;
        std::stack<int> stk;

        for(int i=0; i<=n; ++i){
            int cur = (i == n) ? 0 : nums[i];
            while(!stk.empty() && nums[stk.top()] > cur){
                int min = stk.top(); stk.pop();
                int left = stk.empty() ? -1 : stk.top();
                // (left, i)
                long long sum = prefix[i] - prefix[left+1];
                maxMinProduct = std::max(maxMinProduct, sum*nums[min]);
            }
            stk.push(i);
        }
        return maxMinProduct % MOD;
    }
};

// 这道题与84题 柱状图中最大矩形面积类似
// 我们可以枚举每一个位置，当作子数组的最小值，然后依次找到左右第一个比它小的值
// 同时我们还需要需处理出一个前缀和数组，这样借助左右区间减法就可以得到目标区间的和

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int n = s.size();
        int ans = 0;
        int left = 0;
        std::unordered_map<char, int> dict;
        for(int right=0; right<n; ++right){
            // 进窗口
            dict[s[right]]++;
            // 判断
            while(dict[s[right]] > 1){
                // 出窗口
                dict[s[left++]]--;
            }
            // 更新结果
            ans = std::max(ans, right-left+1);
        }
        return ans;
    }
};