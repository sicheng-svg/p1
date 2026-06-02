#include <vector>
#include <algorithm>
#include <climits>

class Solution1 {
public:
    int removeDuplicates(std::vector<int>& nums) {
        int n = nums.size();
        if(n <= 2) return n;

        /* fast 用来扫描数组， slow是当前的待写入位置，0~slow是结果区间，因为每个数最多出现两次，所以比较时要和slow-2比较，即倒数第二个*/
        int slow = 2;
        for(int fast = 2; fast<n; ++fast){
            if(nums[fast] != nums[slow - 2]){
                nums[slow++] = nums[fast];
            }
        }
        return slow;
    }
};

class Solution2 {
public:
    int majorityElement(std::vector<int>& nums) {
        int candidate = 0, count = 0;
        for(int x: nums){
            if(count == 0){
                candidate = x;
            }
            count = candidate == x ? count + 1 : count - 1;
        }
        return candidate;
    }
};

class Solution3 {
public:
    void reverse(std::vector<int>& nums, int start, int end){
        while(start < end){
            std::swap(nums[start++], nums[end--]);
        }
    }
    void rotate(std::vector<int>& nums, int k) {
        int n = nums.size();
        k %= n;
        reverse(nums, 0, n-1);
        reverse(nums, 0, k-1);
        reverse(nums, k, n-1);
    }
};

class Solution4 {
public:
    int hIndex(std::vector<int>& citations) {
        // 先对数组进行降序排序，我们要找一个数k，满足第k个论文的引用次数大于等与k，我们要找到最大的k
        // 所以就转化为找到一个h，要求 citation[i] >= i + 1，因为i从0开始，i+1表示第几篇论文
        // 当出现不满足的情况后，直接返回，因为是按照降序排序，该位置不满足，后续位置值变小，但i+1还变大，肯定不满足。
        std::sort(citations.rbegin(), citations.rend());
        int h = 0;
        for(int i=0; i<citations.size(); ++i){
            if(citations[i] >= i+1) h = i + 1;
            else break;
        }
        return h;
    }
};

class Solution5{
    /*def jump(self, nums: List[int]) -> int:
        n = len(nums)
        end = 0 # 这一跳的边界，想要走到下一个边界，就得跳了
        farthest = 0 # 可以到达的最远位置
        jumps = 0 # 跳数
        for i in range(n - 1):
            farthest = max(farthest, i + nums[i])
            if i == end: # 该跳了
                jumps += 1
                end = farthest
        return jumps
    */
};

class Solution6 {
public:
    bool canJump(std::vector<int>& nums) {
        // 可以维护一个当前可以到达的最远位置
        // 如果到达一个位置后，发现i>maxRearch，则说明到不了当前位置
        // 如果maxRearch大于nums.size()-1 说明可以达到最后一个位置
        int maxRearch = 0;
        for(int i=0; i<nums.size(); ++i){
            if(i <= maxRearch){
                maxRearch = std::max(maxRearch, i + nums[i]);
                if(maxRearch >= nums.size() - 1) return true;
            }
        }
        return false;
    }
};

class Solution7 {
public:
    int maxProfit(std::vector<int>& prices) {
        int n = prices.size();
        int dp[n][2];
        // dp[0] 表示不持股的最大利润，dp[1]表示持股的最大利润
        dp[0][0] = 0, dp[0][1] = -prices[0];
        for(int i=1; i<n; ++i){
            dp[i][0] = std::max(dp[i-1][0], dp[i-1][1] + prices[i]);
            dp[i][1] = std::max(dp[i-1][1], dp[i-1][0] - prices[i]);
        }
        return dp[n-1][0];
    }
};

class Solution8 {
public:
    int maxProfit(std::vector<int>& prices) {
        int minPrice = INT_MAX, maxProfit = 0;
        for(int p: prices){
            minPrice = std::min(minPrice, p);
            maxProfit = std::max(maxProfit, p-minPrice);
        }
        return maxProfit;
    }
};