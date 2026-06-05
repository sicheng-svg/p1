#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <algorithm>
#include <climits>
#include <cstring>

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

class RandomizedSet {
public:
    RandomizedSet() {}
    
    bool insert(int val) {
        if(map.count(val)) return false;
        map[val] = arr.size(); // 存储值和下标的映射关系
        arr.push_back(val);
        return true;
    }
    
    bool remove(int val) {
        if(map.count(val) == 0) return false;
        // 将待删除的下标用back覆盖，随后删除back即可
        int i = map[val]; // 获取待删除元素的下标
        int ready = arr.back(); // 记录末尾元素
        arr[i] = ready; // 覆盖i位置的元素
        map[ready] = i; // 更新映射关系
        
        arr.pop_back();
        map.erase(val);
        return true;
    }
    
    int getRandom() {
        return arr[rand() % arr.size()];
    }
private:
    std::vector<int> arr;
    std::unordered_map<int, int> map;
};

/**
 * Your RandomizedSet object will be instantiated and called as such:
 * RandomizedSet* obj = new RandomizedSet();
 * bool param_1 = obj->insert(val);
 * bool param_2 = obj->remove(val);
 * int param_3 = obj->getRandom();
 */

class Solution9 {
public:
    std::vector<int> productExceptSelf(std::vector<int>& nums) {
        int n = nums.size();
        std::vector<int> ans(n, 1);
        // 借助前缀积 和 后缀积实现。因为i位置就是除自身外，左右数组的乘积
        // 第一遍，获取左积
        for(int i=1; i<n; ++i){
            ans[i] = ans[i-1] * nums[i-1];
        }
        // 第二遍，获取右积
        int right = 1;
        for(int i=n-1; i>=0; --i){
            ans[i] *= right;
            right *= nums[i];
        }
        return ans;
    }
};

class Solution10 {
public:
    int canCompleteCircuit(std::vector<int>& gas, std::vector<int>& cost) {
        int n = gas.size(), tank = 0, total = 0;
        int start = 0;
        for(int i=0;i <n; ++i){
            int diff = gas[i] - cost[i];
            total += diff;
            tank += diff;
            if (tank < 0){
                start = i + 1;
                tank = 0;
            }
        }
        return total >= 0 ? start : -1;
    }
};

class Solution11 {
public:
    int candy(std::vector<int>& ratings) {
        int n = ratings.size();
        std::vector<int> count1(n, 1);
        std::vector<int> count2(n, 1);
        // 1.从左往右，只要比左边评分高，就+1
        for(int i=1; i<n; ++i){
            if(ratings[i] > ratings[i-1]) count1[i] = count1[i-1] + 1;
        }
        // 2.从右往左，只要比右边评分高，就+1
        for(int i=n-2; i>=0; --i){
            if(ratings[i] > ratings[i+1]) count2[i] = count2[i+1] + 1;
        }
        int ans = 0;
        for(int i=0; i<n; ++i){
            ans += std::max(count1[i], count2[i]);
        }
        return ans;
    }
};

class Solution12 {
public:
    int trap(std::vector<int>& height) {
        int left = 0, right = height.size() - 1;
        int leftMax = 0, rightMax = 0;
        int ans = 0;
        while(left < right){
            if(height[left] < height[right]){
                // 右边柱子高，则该位置的水位取决于左边
                leftMax = std::max(leftMax, height[left]);
                ans += leftMax - height[left];
                left++;
            }else{
                rightMax = std::max(rightMax, height[right]);
                ans += rightMax - height[right];
                right--;
            }
        }
        return ans;
    }
};

class Solution12 {
    std::unordered_map<char, int> dict{
        {'I', 1},
        {'V', 5},
        {'X', 10},
        {'L', 50},
        {'C', 100},
        {'D', 500},
        {'M', 1000}
    };
public:
    int romanToInt(std::string s) {
        int ans = 0;
        for(int i=0; i<s.size(); ++i){
            if(i + 1 < s.size() && dict[s[i]] < dict[s[i+1]]){
                ans -= dict[s[i]];
            }else{
                ans += dict[s[i]];
            }
        }
        return ans;
    }
};

class Solution13 {
public:
    std::string intToRoman(int num) {
        const int val[]  = {1000,900,500,400,100,90,50,40,10,9,5,4,1};
        const std::string sym[] = {"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};
        std::string ans;
        for (int i = 0; i < 13; ++i) {
            while (num >= val[i]) {
                ans += sym[i];
                num -= val[i];
            }
        }
        return ans;
    }
};

class Solution14 {
public:
    int lengthOfLastWord(std::string s) {
        int idx = s.size() - 1;
        // 1. 绕开后置空格
        while(s[idx] == ' ') idx--;
        // 2. 开始记录单词长度
        int ans = 0;
        while(idx >= 0 && s[idx] != ' '){
            idx--;
            ans++;
        }
        return ans;
    }
};

class Solution15 {
public:
    std::string longestCommonPrefix(std::vector<std::string>& strs) {
        std::string prefix;
        // 纵向比较
        for(int i=0; i<strs[0].size(); ++i){
            char c = strs[0][i];
            for(int j=1; j<strs.size(); ++j){
                if(i >= strs[j].size() || strs[j][i] != c)
                    return strs[0].substr(0, i);
            }
        }
        return strs[0];
    }
};

class Solution16 {
public:
    std::string reverseWords(std::string s) {
        std::stack<std::string> st;
        int i = 0, n = s.size();
        while (i < n) {
            while (i < n && s[i] == ' ') i++;        // 跳过空格
            if (i >= n) break;
            int start = i;
            while (i < n && s[i] != ' ') i++;          // 找单词结尾
            st.push(s.substr(start, i - start));
        }
        std::string ans;
        while (!st.empty()) {
            ans += st.top(); st.pop();
            if (!st.empty()) ans += ' ';               // 词间才加空格
        }
        return ans;
    }
};

class Solution17 {
public:
    std::string convert(std::string s, int numRows) {
        int n = s.size();
        if(numRows == 1) return s;
        std::vector<std::string> rows(std::min(n, numRows));
        int row = 0, dir = -1;
        for(char ch: s){
            rows[row] += ch;
            if(row == 0 || row == numRows - 1) dir = -dir;
            row += dir;
        }
        std::string ans;
        for(auto row: rows){
            ans += row;
        }
        return ans;
    }
};

class Solution18 {
public:
    int strStr(std::string haystack, std::string needle) {
        int n = haystack.size(), m = needle.size();
        for(int i=0; i + m <= n; ++i){
            int j = 0;
            while(j < m && haystack[i + j] == needle[j]) j++;
            if(j == m) return i;
        }
        return -1;
    }
};

class Solution19 {
public:
    std::vector<std::string> fullJustify(std::vector<std::string>& words, int maxWidth) {
        std::vector<std::string> res;
        int n = words.size(), i = 0;
        while (i < n) {
            // 1. 贪心确定本行 [i, j) 放哪些词
            int j = i, lineLen = 0;
            while (j < n && lineLen + words[j].size() + (j - i) <= maxWidth) {
                lineLen += words[j].size();
                ++j;
            }
            int cnt = j - i;              // 本行词数
            int gaps = cnt - 1;
            int spaces = maxWidth - lineLen;  // 要填的总空格

            std::string line;
            if (j == n || cnt == 1) {
                // 情况 A：最后一行 / 单词行 —— 左对齐
                for (int k = i; k < j; ++k) {
                    line += words[k];
                    if (k != j - 1) line += ' ';
                }
                line += std::string(maxWidth - line.size(), ' ');  // 右补空格
            } else {
                // 情况 B：两端对齐
                int base = spaces / gaps, extra = spaces % gaps;
                for (int k = i; k < j; ++k) {
                    line += words[k];
                    if (k != j - 1) {
                        int sp = base + (k - i < extra ? 1 : 0);  // 左边间隙多分
                        line += std::string(sp, ' ');
                    }
                }
            }
            res.push_back(line);
            i = j;
        }
        return res;
    }
};

class Solution20 {
public:
    int totalWaviness(int num1, int num2) {
        auto getWaviness = [](int x) -> int{
            std::string s = std::to_string(x);
            int waviness = 0;
            for(size_t i=1; i<s.size()-1; ++i){
                bool isPeak = s[i] > s[i-1] && s[i] > s[i+1];
                bool isValley = s[i] < s[i-1] && s[i] < s[i+1];
                if(isPeak || isValley) waviness++;
            }
            return waviness;
        };

        int total = 0;
        for(int i=num1; i<=num2; ++i){
            total += getWaviness(i);
        }
        return total;
    }
};

class Solution21 {
public:
    long long totalWaviness(long long num1, long long num2) {
        return f(num2) - f(num1 - 1);
    }
private:
    std::string s;
    // memo[i][pre2][pre1] -> {cnt, sum}
    std::pair<long long,long long> memo[20][11][11];
    bool vis[20][11][11];

    long long f(long long x) {
        if (x < 0) return 0;
        s = std::to_string(x);
        memset(vis, 0, sizeof(vis));
        return dfs(0, 10, 10, true, false).second;
    }

    // 返回 {该路径下数字个数, 波动值总和}
    std::pair<long long,long long> dfs(int i, int pre2, int pre1, bool isLimit, bool isNum) {
        if (i == s.size())
            return {isNum ? 1 : 0, 0};
        if (!isLimit && isNum && vis[i][pre2][pre1])
            return memo[i][pre2][pre1];

        long long cnt = 0, sum = 0;
        // 跳过当前位（前导零，还没开始填数字）
        if (!isNum) {
            auto [c, s2] = dfs(i + 1, 10, 10, false, false);
            cnt += c; sum += s2;
        }
        int lo = isNum ? 0 : 1;          // 已是数字则可填0；否则首个非零位从1起
        int hi = isLimit ? s[i] - '0' : 9;
        for (int cur = lo; cur <= hi; cur++) {
            // 判断 pre1 是否成峰/谷（pre2、pre1 都必须真实存在）
            int contrib = 0;
            if (pre2 != 10 && pre1 != 10) {
                if (pre1 > pre2 && pre1 > cur) contrib = 1;
                else if (pre1 < pre2 && pre1 < cur) contrib = 1;
            }
            auto [c, s2] = dfs(i + 1, pre1, cur, isLimit && cur == hi, true);
            cnt += c;
            sum += s2 + (long long)contrib * c;   // 贡献乘以后续数字个数
        }
        if (!isLimit && isNum) {
            vis[i][pre2][pre1] = true;
            memo[i][pre2][pre1] = {cnt, sum};
        }
        return {cnt, sum};
    }
};