class Solution:
    def maxProfit(self, prices: List[int]) -> int:
        minPrice = prices[0]
        ans = 0
        for i in range(len(prices)):
            minPrice = min(minPrice, prices[i])
            ans = max(ans, prices[i] - minPrice)
        return ans

class Solution:
    def canJump(self, nums: List[int]) -> bool:
        maxReach = 0
        n = len(nums)
        for i in range(n):
            if i > maxReach:
                return False
            maxReach = max(maxReach, i + nums[i])
            if maxReach >= len(nums) - 1:
                return True
        return True

class Solution:
    def jump(self, nums: List[int]) -> int:
        n = len(nums)
        end = 0
        farthest = 0
        jumps = 0
        for i in range(n - 1):
            farthest = max(farthest, i + nums[i])
            if i == end:
                jumps += 1
                end = farthest
        return jumps

class Solution:
    def partitionLabels(self, s: str) -> List[int]:
        last_index = {c: i for i, c in enumerate(s)}

        result = []
        start = end = 0
        for i, c in enumerate(s):
            end = max(end, last_index[c])
            if i == end:
                result.append(i - start + 1)
                start = i + 1
        return result

class Solution:
    def uniquePaths(self, m: int, n: int) -> int:
        # dp[i][j] 表示达到该位置是一共有多少种走法
        # dp[i][j] = dp[i][j-1] + dp[i-1][j]
        dp = [[0]*(n+1) for _ in range(m+1)]
        dp[0][1] = 1
        for i in range(1, m+1):
            for j in range(1, n+1):
                dp[i][j] = dp[i-1][j] + dp[i][j-1]
        return dp[m][n]

class Solution:
    def longestPalindrome(self, s: str) -> str:
        n = len(s)
        if n < 2:
            return s
        
        dp = [[False]*n for _ in range(n)]
        start = 0
        max_len = 1
        
        # 单字符回文
        for i in range(n):
            dp[i][i] = True
        
        # 填表
        for j in range(1, n):       # 右边界
            for i in range(j-1, -1, -1):  # 左边界，从 j-1 到 0
                if s[i] == s[j]:
                    if j - i <= 2:
                        dp[i][j] = True
                    else:
                        dp[i][j] = dp[i+1][j-1]
                # 更新最长回文
                if dp[i][j] and (j-i+1 > max_len):
                    max_len = j - i + 1
                    start = i
    
        return s[start:start+max_len]
        
class Solution:
    def trap(self, height: List[int]) -> int:
        left, right = 0, len(height) - 1
        leftMax, rightMax, ans = 0, 0, 0
        while left < right:
            if height[left] < height[right]:
                leftMax = max(leftMax, height[left])
                ans += leftMax - height[left]
                left += 1
            else:
                rightMax = max(rightMax, height[right])
                ans += rightMax - height[right]
                right -= 1
        return ans

class Solution {
public:
    bool isGood(vector<int>& nums) {
        int n =nums.size() - 1;
        std::vector<int> cnt(n+2, 0);
        // 统计频次
        for(int x: nums){
            if(x<1 || x > n) return false;
            cnt[x]++;
        }
        for(int i=0; i<n; ++i){
            if(cnt[i] != 1) return false;
        }
        return cnt[n] == 2;
    }
};

class Solution:
    def rotate(self, matrix: List[List[int]]) -> None:
        """
        Do not return anything, modify matrix in-place instead.
        """
        n = len(matrix)
        for i in range(n):
            for j in range(i + 1, n):
                matrix[i][j], matrix[j][i] = matrix[j][i], matrix[i][j]
        for row in matrix:
            row.reverse()

class Solution {
public:
    bool canReach(vector<int>& arr, int start) {
        int n = arr.size();
        std::vector<bool> visited(n, false);
        std::queue<int> q;
        q.push(start);
        while(!q.empty()){
            int idx = q.front(); q.pop();
            if(arr[idx] == 0) return true;
            visited[idx] = true;
            if(idx - arr[idx] >= 0 && !visited[idx-arr[idx]]) q.push(idx - arr[idx]);
            if(idx + arr[idx] < n && !visited[idx+arr[idx]]) q.push(idx + arr[idx]);
        }
        return false;
    }
};

class Solution:
    def minJumps(self, arr: List[int]) -> int:
        n = len(arr)
        if n == 1:
            return 0
        
        # 将相同value的下标存储在一起，方便查找j位置
        idx = defaultdict(list)
        for i, v in enumerate(arr):
            idx[v].append(i)
        
        q = deque([0])
        visited = [False]*n
        visited[0] = True
        steps = 0

        while q:
            for _ in range(len(q)):
                i = q.popleft()
                if i == n-1:
                    return steps
                
                # 值相同的邻居
                for j in idx[arr[i]]:
                    if not visited[j]:
                        visited[j] = True
                        q.append(j)
                idx[arr[i]].clear()

                # 还可以走到i+1/i-1位置
                for j in (i-1, i+1):
                    if 0 <= j and j < n and not visited[j]:
                        visited[j] = True
                        q.append(j)
            steps += 1 # 将一层处理完后， steps++
        return -1
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int i = m-1, j = n-1, tail = m+n-1;
        while(i>=0 && j>=0){
            if(nums1[i] > nums2[j]) nums1[tail--] = nums1[i--];
            else nums1[tail--] = nums2[j--];
        }
        while(i>=0){
            nums1[tail--] = nums1[i--];
        }
        while(j>=0){
            nums1[tail--] = nums2[j--];
        }
    }
};