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

