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