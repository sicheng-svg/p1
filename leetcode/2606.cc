#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <queue>
#include <functional>
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <sstream>
using namespace std;

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

class Solution22 {
public:
    std::vector<int> leftRightDifference(std::vector<int>& nums) {
        int n = nums.size();
        std::vector<int> ans(n, 0);
        // 1. 统计左前缀和
        for(int i=1; i<n; ++i){
            ans[i] = ans[i-1] + nums[i-1];
        }
        int right = 0;
        for(int i=n-2; i>=0; --i){
            right += nums[i+1];
            ans[i] = std::abs(ans[i] - right);
        }
        return ans;    }
};

class Solution23 {
public:
    bool isPalindrome(std::string s) {
        auto preProcess = [](const std::string& s) -> std::string {
            std::string ans;
            for(char c: s){
                if(c >= 'A' && c <= 'Z'){
                    ans += (c+32);
                }else if(c >= 'a' && c <= 'z'){
                    ans += c;
                }else if(c >= '0' && c <= '9'){
                    ans += c;
                }else{
                    continue;
                }
            }
            return ans;
        };
        auto isPalindrome = [](const std::string& s) -> bool{
            if(s.size() == 0) return true;
            int left = 0, right = s.size() - 1;
            while(left < right){
                if(s[left] != s[right]) return false;
                left++;
                right--;
            }
            return true;
        };
        std::string tmp = preProcess(s);
        std::cout << tmp << std::endl;
        return isPalindrome(tmp);
    }
};

class Solution24 {
public:
    bool isSubsequence(std::string s, std::string t) {
        if(s.size() > t.size()) return false;
        int i = 0, j = 0;
        while(i<s.size() && j<t.size() && i <= j){
            if(s[i] == t[j]){
                i++;
                j++;
            }else{
                j++;
            }
        }
        return i == s.size();
    }
};

class Solution25 {
public:
    std::vector<int> twoSum(std::vector<int>& numbers, int target) {
        int i = 0, j = numbers.size() - 1;
        while(i<j){
            if(numbers[i] + numbers[j] < target){
                i++;
            }else if(numbers[i] + numbers[j] > target){
                j--;
            }else{
                return {i+1, j+1};
            }
        }
        return {-1, -1};
    }
};

class Solution26 {
public:
    int maxArea(std::vector<int>& height) {
        int i = 0, j = height.size() - 1;
        int ans = 0;
        while(i < j){
            ans = std::max(ans, ((j-i)*std::min(height[i], height[j])));
            if(height[i] < height[j]) i++;
            else j--;
        }
        return ans;
    }
};

class Solution27 {
public:
    std::vector<std::vector<int>> threeSum(std::vector<int>& nums) {
        // // 1. 去重
        // std::set<int> s(nums.begin(), nums.end());
        // nums = std::vector<int>(s.begin(), s.end());
        // 2. 排序后，先固定个数，问题就转化为找出两个数之和等于 target = - nums[i]
        std::sort(nums.begin(), nums.end());
        int n = nums.size();
        std::vector<std::vector<int>> ans;
        for(int i=0; i<n;){
            if(nums[i] > 0) break; // 剪枝

            int j = i+1, k = n-1;
            int target = -nums[i];
            while(j < k){
                if(nums[j] + nums[k] < target) j++;
                else if(nums[j] + nums[k] > target) k--;
                else {
                    ans.push_back({nums[i], nums[j], nums[k]});
                    int old_j = nums[j];
                    int old_k = nums[k];
                    while(j < k && nums[j] == old_j) j++;
                    while(j < k && nums[k] == old_k) k--;
                }
            }
            // 跳过相同的i
            while(i < n && nums[i] == -target) i++;
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
class Solution28 {
public:
    TreeNode* createBinaryTree(std::vector<std::vector<int>>& descriptions) {
        std::unordered_map<int, TreeNode*> nodes;
        std::unordered_set<int> isChild;
        auto get = [&](int x) -> TreeNode* {
            if(nodes.count(x) == 0) nodes[x] = new TreeNode(x);
            return nodes[x];
        };
        for(auto& desc: descriptions){
            TreeNode *child = get(desc[1]);// child不存在则新建
            TreeNode *parent = get(desc[0]);// parent不存在则新建

            if(desc[2])    parent->left = child;
            else           parent->right = child;
            isChild.insert(desc[1]);
        }
        // isChild中不存在的点就是root
        for(auto& [val, node]: nodes){
            if(!isChild.count(val)) return node;
        }
        return nullptr;
    }
};

class Solution29 {
public:
    int minSubArrayLen(int target, std::vector<int>& nums) {
        int i = 0, j = 0, n = nums.size();
        int sum = 0;
        int ans = INT_MAX;
        while(j < n){
            sum += nums[j++];
            while(sum >= target) {
                ans = std::min(ans, j - i);
                sum -= nums[i++];
            }
        }
        return ans == INT_MAX ? 0 : ans;
    }
};

class Solution30 {
public:
    int lengthOfLongestSubstring(std::string s) {
        int left = 0, right = 0, n = s.size();
        int ans = 0;
        std::unordered_map<char, int> dict;
        while(right < n) {
            // 1.进窗口
            dict[s[right]]++;

            // 2.判断
            while(dict[s[right]] > 1){
                // 3.出窗口
                dict[s[left]]--;
                left++;
            }

            // 3.满足条件更新结果
            ans = std::max(ans, right - left + 1);
            right++; // 走到下一个位置
        }
        return ans;
    }
};

class Solution31 {
public:
    std::vector<int> findSubstring(std::string s, std::vector<std::string>& words) {
        int n = s.size(), m = words.size(), z = words[0].size();
        std::vector<int> ans;
        int win = m*z;
        if(n < win) return ans;

        std::unordered_map<std::string, int> need;
        for(auto word: words) need[word]++;

        /* 1. 枚举起点 */
        for(int i=0; i<z; ++i){
            std::unordered_map<std::string, int> cnt;
            int count = 0; // 记录有效字符的个数
            for(int left=i, right=i; right+z<=n; right+=z){
                // 进窗口
                std::string in = s.substr(right, z);
                cnt[in]++;
                if (cnt.count(in) && need.count(in) && cnt[in] <= need[in]) count++;
                // 判断
                if(right+z-left > win){
                    // 出窗口
                    std::string out = s.substr(left, z);
                    if (need.count(out) && cnt[out] <= need[out]) count--;
                    cnt[out]--;
                    left += z;
                }
                // 更新结果
                if(count == m) ans.push_back(left);
            }
        }
        return ans;
    }
};

class Solution32 {
public:
    std::string minWindow(std::string s, std::string t) {
        int m = s.size(), n = t.size();
        std::unordered_map<char, int> needs;
        for(auto& c: t) needs[c]++;

        int left = 0, right = 0;
        std::unordered_map<char, int> cnt;
        int count = 0;
        int len = INT_MAX, pos = 0;
        while(right < m){
            // 进窗口
            cnt[s[right]]++;
            if (needs.count(s[right]) && cnt[s[right]] <= needs[s[right]]) count++;
            // 判断
            while(count == n) {
                // 更新结果
                if(right-left + 1 < len){
                    len = right-left + 1;
                    pos = left;
                }
                // 出窗口
                if (needs.count(s[left]) && cnt[s[left]] <= needs[s[left]]) count--;
                cnt[s[left]]--;
                left++;
            }
            right++;
        }
        return len == INT_MAX ? "" : s.substr(pos, len);
    }
};

class Solution33 {
public:
    vector<int> pivotArray(vector<int>& nums, int pivot) {
        int n = nums.size(); 
        std::vector<int> ans(n, pivot);
        // 1. 统计小于pivot和等于pivot的个数,计算出它们的下标位置
        int less = 0, eq = 0;
        for(int x: nums){
            if(x < pivot) ++less;
            if(x == pivot) ++eq;
        }
        // 2. [0, less-1] [less, less+eq-1] [less+eq, n-1]
        int i=0, j=less+eq;
        for(int x: nums){
            if(x < pivot) ans[i++] = x;
            else if(x > pivot) ans[j++] = x;
            else continue;
        }
        return ans;
    }
};

class Solution34 {
public:
    bool isValidSudoku(vector<vector<char>>& board) {
        bool row[9][9] = {}, col[9][9] = {}, boxes[9][9] = {};
        for(int i=0; i<9; ++i){
            for(int j=0; j<9; ++j){
                if(board[i][j] == '.') continue;
                int number = board[i][j] - '1';
                int b = (i / 3) * 3 + j / 3;
                if(row[i][number] || col[j][number] || boxes[b][number]) return false;
                row[i][number] = col[j][number] = boxes[b][number] = true;
            }
        }
        return true;
    }
};

class Solution35 {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        std::vector<int> ans;
        int top = 0, bottom = m-1, left = 0, right = n-1;
        while(top <= bottom && left <= right){
            // 1. 左到右
            for(int j=left; j<=right;++j) ans.push_back(matrix[top][j]);
            top++;
            if(top > bottom) break;

            // 2. 从上到下
            for(int i=top; i<=bottom; ++i) ans.push_back(matrix[i][right]);
            right--;
            if(left>right) break;

            // 3. 从右到左
            for(int j=right; j>=left; --j) ans.push_back(matrix[bottom][j]);
            bottom--;
            if(top > bottom) break;

            // 4. 从下到上
            for(int i=bottom; i>=top; --i) ans.push_back(matrix[i][left]);
            left++;
            if(left > right) break;
        }
        return ans;
    }
};

class Solution36 {
public:
    void rotate(vector<vector<int>>& matrix) {
        int n = matrix.size();
        for(int i=0; i<n; ++i){
            for(int j=i+1; j<n; ++j){
                std::swap(matrix[i][j], matrix[j][i]); // 先沿对角线反转
            }
        }
        for(int i=0; i<n; ++i){
            for(int j=0; j<n/2; ++j){
                std::swap(matrix[i][j], matrix[i][n-1-j]); // 水平反转
            }
        }
    }
};

class Solution37 {
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

class Solution38 {
public:
    void gameOfLife(vector<vector<int>>& board) {
        int m = board.size(), n = board[0].size();
        int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j) {
                int alive = 0;
                for (auto& d : dirs) {
                    int ni = i + d[0], nj = j + d[1];
                    if (ni>=0 && ni<m && nj>=0 && nj<n && (board[ni][nj] & 1)) ++alive;
                }
                int cur = board[i][j] & 1;
                if ((cur && (alive==2 || alive==3)) || (!cur && alive==3))
                    board[i][j] |= 2;
            }
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j)
                board[i][j] >>= 1;
    }
};

class Solution39 {
public:
    long long maxTotalValue(vector<int>& nums, int k) {
        // 题目要求找到k个子数组，要求这些子数组的和最大。并且子数组可以重复选取
        // 所以，我们只需要找到值最大的子数组，*k
        // 而值最大的子数组肯定存在于原数组中
        int m1 = INT_MIN, m2 = INT_MAX;
        for(int x: nums){
            m1 = std::max(m1, x);
            m2 = std::min(m2, x);
        }
        return static_cast<long long>(m1-m2) * k;
    }
};

class Solution40 {
public:
    bool canConstruct(string ransomNote, string magazine) {
        if(ransomNote.size() > magazine.size()) return false;
        
        int dict[26] = {};
        for(char c: magazine){
            dict[c-'a']++;
        }

        for(char c: ransomNote){
            dict[c-'a']--;
            if(dict[c-'a'] < 0) return false;
        }
        return true;
    }
};

class Solution41 {
public:
    bool isIsomorphic(string s, string t) {
        int s2t[128] = {}, t2s[128] = {};
        int n = s.size();
        for(int i=0; i<n; ++i){
            char a = s[i], b = t[i];
            if(s2t[a] && s2t[a] != b) return false;
            if(t2s[b] && t2s[b] != a) return false;
            s2t[a] = b;
            t2s[b] = a;
        }
        return true;
    }
};

class Solution42 {
public:
    bool wordPattern(string pattern, string s) {
        // 切分 s
        vector<string> words;
        stringstream ss(s);
        string w;
        while (ss >> w) words.push_back(w);

        // 长度必须匹配（这就是 word_pattern 比 isomorphic 多出来的坑）
        if (pattern.size() != words.size()) return false;

        unordered_map<char, string> c2w;
        unordered_map<string, char> w2c;
        for (int i = 0; i < pattern.size(); i++) {
            char c = pattern[i];
            string& word = words[i];
            if (c2w.count(c) && c2w[c] != word) return false;
            if (w2c.count(word) && w2c[word] != c) return false;
            c2w[c] = word;
            w2c[word] = c;
        }
        return true;
    }
};

class Solution43 {
public:
    bool isAnagram(string s, string t) {
        if(s.size() != t.size()) return false;
        int dict[26] = {0};
        for(char c: s){
            dict[c-'a']++;
        }
        for(char c: t){
            if(--dict[c-'a'] < 0) return false;
        }
        return true;
    }
};

class Solution44 {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        // 对于字符异位词来说，他们排序后是相同的
        // 利用哈希表统计，将排序后相同的插入到一个位置上
        std::unordered_map<std::string, std::vector<std::string>> hash;
        for(auto& str : strs) {
            std::string tmp = str;
            sort(tmp.begin(), tmp.end());
            hash[tmp].push_back(str);
        }
        std::vector<std::vector<std::string>> ret;
        for(auto& [key, value] : hash) {
            ret.push_back(value);
        }
        return ret;
    }
};

class Solution45 {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> seen;  // 值 → 下标
        for (int i = 0; i < nums.size(); i++) {
            int need = target - nums[i];
            if (seen.count(need))        // 之前见过另一半?
                return {seen[need], i};
            seen[nums[i]] = i;           // 没见过,把自己存进去
        }
        return {};
    }
};

class Solution46 {
public:
    int next(int x) {
        int sum = 0;
        while (x > 0) {
            int d = x % 10;
            sum += d * d;
            x /= 10;
        }
        return sum;
    }
    bool isHappy(int n) {
        // 对于快乐数而言，要么最终到1，要么会进入一个循环
        // 所以，我们就可以判断序列是否含环即可。
        int slow = n, fast = next(n);
        while (fast != 1 && slow != fast) {
            slow = next(slow);          // 走一步
            fast = next(next(fast));    // 走两步
        }
        return fast == 1;
    }
};

class Solution47 {
public:
    long long maxTotalValue(vector<int>& nums, int k) {
        int n = nums.size();
        int LOG = 1;
        while ((1 << LOG) <= n) LOG++;
        vector<vector<int>> mx(LOG, vector<int>(n)), mn(LOG, vector<int>(n));
        for (int i = 0; i < n; i++) mx[0][i] = mn[0][i] = nums[i];
        for (int j = 1; j < LOG; j++)
            for (int i = 0; i + (1 << j) <= n; i++) {
                mx[j][i] = max(mx[j-1][i], mx[j-1][i + (1 << (j-1))]);
                mn[j][i] = min(mn[j-1][i], mn[j-1][i + (1 << (j-1))]);
            }
        auto query = [&](int l, int r) -> int { // [l, r] 的 max - min
            int s = 31 - __builtin_clz(r - l + 1);
            int mxv = max(mx[s][l], mx[s][r - (1 << s) + 1]);
            int mnv = min(mn[s][l], mn[s][r - (1 << s) + 1]);
            return mxv - mnv;
        };
        // 堆元素: {value, l, r}
        priority_queue<tuple<int,int,int>> pq;
        for (int l = 0; l < n; l++)
            pq.push({query(l, n - 1), l, n - 1});
        long long ans = 0;
        while (k-- > 0) {
            auto [v, l, r] = pq.top(); pq.pop();
            ans += v;
            if (r > l) pq.push({query(l, r - 1), l, r - 1});
        }
        return ans;
    }
};

class Solution48 {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        unordered_set<int> window;
        for (int i = 0; i < nums.size(); ++i) {
            if (i > k) window.erase(nums[i - k - 1]);  // 移出窗口外的最老元素
            if (window.count(nums[i])) return true;     // 窗口内有重复
            window.insert(nums[i]);
        }
        return false;
    }
};

class Solution49 {
public:
    int longestConsecutive(vector<int>& nums) {
        std::unordered_set<int> st;
        for(int e: nums) st.insert(e);
        // 想要找到最长序列，只需要判断当前数是不是序列的开头即可
        // 如果是开头的话，就因此在集合中找+1是否存在，同时记录个数
        int ans = 0;
        for(int num: st){
            if(!st.count(num-1)) {
                // 是一个序列的开头，继续找
                int count = 1;
                while(st.count(num+1)){
                    count++;
                    num++;
                }
                ans = std::max(ans, count);
            }
        }
        return ans;
    }
};

class Solution50 {
public:
    vector<string> summaryRanges(vector<int>& nums) {
        std::vector<std::string> ans;
        int i = 0;
        while(i<nums.size()){
            int j = i;
            while(i + 1 < nums.size() && nums[i] + 1 == nums[i+1]) i++;
            if(i == j) ans.push_back(std::to_string(nums[i]));
            else ans.push_back(std::to_string(nums[j]) + "->" + std::to_string(nums[i]));
            i++; // 走到下一个区间
        }
        return ans;
    }
};

class Solution51 {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        sort(intervals.begin(), intervals.end()); // 默认按第一个元素排序

        vector<vector<int>> ans;
        for (const auto& interval : intervals) {
            int s = interval[0], e = interval[1];
            // 答案为空，或当前区间和上一个合并结果不重叠 -> 新开一段
            if (ans.empty() || ans.back()[1] < s) {
                ans.push_back({s, e});
            } else {
                // 重叠 -> 撑大上一段的右端点
                ans.back()[1] = max(ans.back()[1], e);
            }
        }
        return ans;
    }
};

class Solution52 {
public:
    int assignEdgeWeights(vector<vector<int>>& edges) {
        const int MOD = 1e9 + 7;
        int n = edges.size() + 1;

        // 1. 将edges转换为邻接表
        std::vector<std::vector<int>> adj(n + 1);
        for(auto& edge: edges){
            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]);
        }

        // 2. bfs找到最深节点
        std::vector<int> depth(n+1, -1);
        depth[1] = 0;
        std::queue<int> q;
        q.push(1);
        int k = 0; // 最深节点所对应的边的条数
        while(!q.empty()){
            int x = q.front(); q.pop();
            for(int neighbor: adj[x]){
                if(depth[neighbor] == -1){ // 该节点没有访问
                    depth[neighbor] = depth[x] + 1;
                    k = std::max(k, depth[neighbor]);
                    q.push(neighbor);
                }
            }
        }
        // 快速幂求 2^(k-1)
        long long res = 1, base = 2;
        int e = k - 1;
        while (e > 0) {
            if (e & 1) res = res * base % MOD;
            base = base * base % MOD;
            e >>= 1;
        }
        return (int)res;
    }
};

class Solution53 {
public:
    vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
        vector<vector<int>> res;
        int i = 0;
        int n = intervals.size();
        
        // 1. 左侧不重叠区间
        while (i < n && intervals[i][1] < newInterval[0]) {
            res.push_back(intervals[i]);
            i++;
        }
        
        // 2. 找到重叠区间，不断扩大 newInterval 的范围
        while (i < n && intervals[i][0] <= newInterval[1]) {
            newInterval[0] = min(newInterval[0], intervals[i][0]);
            newInterval[1] = max(newInterval[1], intervals[i][1]);
            i++;
        }
        res.push_back(newInterval); // 把最终合并好的新区间放入结果
        
        // 3. 右侧不重叠区间
        while (i < n) {
            res.push_back(intervals[i]);
            i++;
        }
        
        return res;
    }
};

class Solution54 {
public:
    int findMinArrowShots(vector<vector<int>>& points) {
        // 不能直接合并区间，本质上我们要找到区间的重叠部分
        sort(points.begin(), points.end(), [](const std::vector<int>& v1, const std::vector<int>& v2){
            return v1[1] < v2[1];
        });
        int arrows = 1;
        int cur = points[0][1];
        for(int i=1; i<points.size(); ++i){
            if(cur < points[i][0]){// 前后区间没有重叠，需要新箭
                arrows++;
                cur = points[i][1];
            }  
        }
        return arrows;
    }
};

class Solution55 {
    static const int MOD = 1e9 + 7;
public:
    vector<int> assignEdgeWeights(vector<vector<int>>& edges, vector<vector<int>>& queries) {
        int n = edges.size() + 1;
        int LOG = 1;
        while ((1 << LOG) < n) LOG++;
        LOG++;

        // 将edges转换为邻接表
        vector<vector<int>> g(n + 1);
        for (auto& e : edges) {
            g[e[0]].push_back(e[1]);
            g[e[1]].push_back(e[0]);
        }

        // BFS 求 depth 和直接父亲
        vector<int> depth(n + 1, 0), par(n + 1, 0);
        vector<bool> vis(n + 1, false);
        queue<int> q;
        q.push(1); vis[1] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) if (!vis[v]) {
                vis[v] = true;
                depth[v] = depth[u] + 1;
                par[v] = u;
                q.push(v);
            }
        }

        // 倍增表
        vector<vector<int>> up(LOG, vector<int>(n + 1, 0));
        for (int v = 1; v <= n; v++) up[0][v] = par[v];
        for (int j = 1; j < LOG; j++)
            for (int v = 1; v <= n; v++)
                up[j][v] = up[j-1][ up[j-1][v] ];

        // 预处理 2 的幂
        vector<long long> pow2(n + 1);
        pow2[0] = 1;
        for (int i = 1; i <= n; i++) pow2[i] = pow2[i-1] * 2 % MOD;

        auto lca = [&](int u, int v) -> int {
            if (depth[u] < depth[v]) swap(u, v);
            int d = depth[u] - depth[v];
            for (int j = 0; j < LOG; j++)
                if (d & (1 << j)) u = up[j][u];
            if (u == v) return u;
            for (int j = LOG - 1; j >= 0; j--)
                if (up[j][u] != up[j][v]) { u = up[j][u]; v = up[j][v]; }
            return up[0][u];
        };

        vector<int> ans;
        for (auto& qq : queries) {
            int u = qq[0], v = qq[1];
            int w = lca(u, v);
            int k = depth[u] + depth[v] - 2 * depth[w];
            ans.push_back(k == 0 ? 0 : (int)pow2[k - 1]);
        }
        return ans;
    }
};

class Solution56 {
public:
    bool isValid(string s) {
        std::stack<char> st;
        for(char ch: s){
            if(ch == '(' || ch == '[' || ch == '{'){
                st.push(ch);
            }else{
                if(st.empty()) return false;

                char c = st.top(); st.pop();
                if((ch == ')' && c != '(')
                || (ch == ']' && c != '[')
                || (ch == '}' && c != '{')){
                    return false;
                }
            }
        }
        return st.empty();
    }
};

class Solution57 {
public:
    string simplifyPath(string path) {
        std::vector<std::string> st;   // 用 vector 当栈,方便最后正序拼接
        std::string seg;
        std::stringstream ss(path);

        // 按 '/' 切分,getline 会把两个 '/' 之间的内容(可能为空)给 seg
        while (std::getline(ss, seg, '/')) {
            if (seg == "" || seg == ".") {
                continue;                       // 空段 或 当前目录,跳过
            } else if (seg == "..") {
                if (!st.empty()) st.pop_back();  // 回上级,空栈时不动
            } else {
                st.push_back(seg);               // 真正的目录名
            }
        }

        // 拼接:每段前加一个 '/'
        std::string ans;
        for (const std::string& s : st) {
            ans += "/" + s;
        }
        return ans.empty() ? "/" : ans;          // 全空说明是根目录
    }
};

class Solution58 {
public:
    string mapWordWeights(vector<string>& words, vector<int>& weights) {
        std::string ans;
        for(auto& word: words){
            int sum = 0;
            for(auto c: word){
                sum += weights[c-'a'];
            }
            sum %= 26;
            ans.push_back( 'a' + (25-sum));
        }
        return ans;
    }
};

class MinStack {
public:
    MinStack() {
    }
    
    void push(int val) {
        if(minSt.empty() || val <= minSt.top()){
            st.push(val);
            minSt.push(val);
        }else{
            st.push(val);
        }
    }
    
    void pop() {
        if(st.top() == minSt.top()) {
            st.pop();
            minSt.pop();
        }else{
            st.pop();
        }
    }
    
    int top() {
        return st.top();
    }
    
    int getMin() {
        return minSt.top();
    }
private:
    std::stack<int> minSt;
    std::stack<int> st;
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(val);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */

class Solution60 {
public:
    std::unordered_map<std::string, std::function<int(int, int)>> method {
        {"+", [](int left, int right){ return left + right;}},
        {"-", [](int left, int right){ return left - right;}},
        {"*", [](int left, int right){ return left * right;}},
        {"/", [](int left, int right){ return left / right;}}
    };

    int evalRPN(std::vector<std::string>& tokens) {
        // 遍历数组进行压栈，当遇到操作符时，连续弹出两次，根据操作符进行运算，随后重新压入栈中
        std::stack<int> st;
        for(auto& e : tokens) {
            if(method.count(e)) {
                int right = st.top(); st.pop();
                int left = st.top(); st.pop();
                st.push(method[e](left,right));
            }else {
                st.push(stoi(e));
            }
        }
        return st.top();
    }
};


struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution61 {
public:
    int pairSum(ListNode* head) {
        ListNode* fast = head;
        ListNode* slow = head;
        ListNode* pre = nullptr;
        while(fast && fast->next){
            fast = fast->next->next;
            pre = slow;
            slow = slow->next;
        }
        // slow是后半部分的起始节点,pre是前半部分的结尾
        pre->next = nullptr;
        ListNode* cur = slow;
        ListNode* prev = nullptr;
        while(cur){
            ListNode* nxt = cur->next;
            cur->next = prev;
            prev = cur;
            cur = nxt;
        }

        int ans = 0;
        while(head && prev){
            ans = std::max(ans, head->val + prev->val);
            head = head->next;
            prev = prev->next;
        }
        return ans;
    }
};

class Solution62 {
public:
    int calculate(string s) {
        std::stack<int> st;
        int res = 0, sign = 1;
        long long num = 0;
        for(char c: s){
            if(isdigit(c)){
                num = num * 10 + (c - '0'); // 数字可能有好几位
            }else if(c == '+'){
                res += num * sign;
                sign = 1;
                num = 0;
            }else if(c == '-'){
                res += num * sign;
                sign = -1;
                num = 0;
            }else if(c == '('){
                // 遇到(，先将前面的临时存储起来用一组新的sign和res计算
                st.push(res);
                st.push(sign);
                res = 0;
                sign = 1;
            }else if(c == ')') {
                res += sign * num;
                num = 0;

                // 遇到),说明括号内要计算完了，和前面的进行计算
                res = res * st.top(); st.pop();
                res += st.top(); st.pop();
                sign = 1;
            }else{}
        }
        return res + num*sign; // 将最后一个数加上
    }
};

class Solution63 {
public:
    bool hasCycle(ListNode *head) {
        ListNode *fast = head, *slow = head;
        while(fast && fast->next){
            fast = fast->next->next;
            slow = slow->next;
            if(fast == slow) return true;
        }   
        return false;
    }
};

class Solution64 {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode* dummy = new ListNode();
        ListNode* tail = dummy;
        int carry = 0;
        while(l1 || l2 || carry){
            int sum = (l1 ? l1->val : 0) + (l2 ? l2->val : 0) + carry;   
            carry = sum / 10;
            ListNode* newNode = new ListNode(sum%10);
            tail->next = newNode;
            tail = newNode;
            if (l1) l1 = l1->next;           
            if (l2) l2 = l2->next;
        }
        tail = dummy->next;
        delete dummy;
        return tail;
    }
};

class Solution65 {
public:
    // 递归
    ListNode* _mergeTwoLists(ListNode* list1, ListNode* list2) {
        if(!list1 || !list2) return list1 ? list1 : list2;
        if(list1->val < list2->val){
            list1->next = mergeTwoLists(list1->next, list2);
            return list1;
        }else{
            list2->next = mergeTwoLists(list1, list2->next);
            return list2;
        }
    }
    // 迭代
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode* dummy = new ListNode();
        ListNode* tail = dummy;
        while(list1 && list2){
            if(list1->val < list2->val){
                tail->next= list1;
                tail = list1;
                if(list1)
                    list1 = list1->next;
            }else{
                tail->next = list2;
                tail = list2;
                if(list2)
                    list2 = list2->next;
            }
        }
        ListNode* other = list1 ? list1 : list2;
        tail->next = other;
        tail = dummy->next;
        delete dummy;
        return tail;
    }
};

class Node {
public:
    int val;
    Node* next;
    Node* random;
    
    Node(int _val) {
        val = _val;
        next = NULL;
        random = NULL;
    }
};

class Solution66 {
public:
    Node* copyRandomList(Node* head) {
        if(!head) return nullptr;
        // 1. 将copy链表插入到对应原节点的后面
        for(Node* cur=head; cur; cur=cur->next->next){
            Node* copy = new Node(cur->val);
            copy->next = cur->next;
            cur->next = copy;
        }

        // 2. 设置copy节点的random指针
        for(Node* cur=head; cur; cur=cur->next->next){
            if(cur->random){
                cur->next->random = cur->random->next;
            }
        }

        // 3. 将两个链表拆开
        Node* dummy = new Node(0);
        Node* tail = dummy;
        for(Node* cur=head; cur; cur=cur->next){
            tail->next = cur->next;
            tail = cur->next;
            cur->next = cur->next->next; // 还原原链表
        }
        tail = dummy->next;
        delete dummy;
        return tail;
    }

    Node* __copyRandomList(Node* head) {
        std::unordered_map<Node*, Node*> map;
        for(Node *cur=head; cur; cur=cur->next){
            map[cur] = new Node(cur->val);
        }
        for(Node* cur = head; cur; cur=cur->next){
            map[cur]->next = map[cur->next];
            map[cur]->random = map[cur->random];
        }
        return map[head];
    }

    Node* _copyRandomList(Node* head) {
        if(!head) return head;

        std::unordered_map<Node*, Node*> hash;
        Node dummy(0);
        Node *cur = head, *tail = &dummy;
        while(cur){
            Node *newNode = new Node(cur->val);
            newNode->next = nullptr;
            tail->next = newNode;
            tail = newNode;

            hash[cur] = newNode; // 旧节点 -> 新节点
            cur = cur->next;
        }

        cur = head;
        Node *copy = dummy.next;
        while(cur){
            if(cur->random == nullptr) copy->random = nullptr;
            else{
                copy->random = hash[cur->random]; // cur->random指向的节点，映射的就是我们需要的新random
            }
            cur = cur->next;
            copy = copy->next;
        }
        return dummy.next;
    }
};

class Solution67 {
public:
    ListNode* deleteMiddle(ListNode* head) {
        if(head->next == nullptr) return nullptr;
        ListNode* fast = head, *slow=head,*prev = nullptr;
        while(fast && fast->next){
            fast=fast->next->next;
            prev = slow;
            slow=slow->next;
        }
        if(prev) prev->next=slow->next;
        return head;
    }
};

class Solution68 {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        // 借助虚拟头节点，这里是left和right指的是第几个节点，下标从1开始
        // 1. 创建一个虚拟头节点，找到left的前一个节点
        // 2. 从left开始，反转right-left个节点即可
        // 3. 此时cur指向after right，prev是新头，更新指向即可
        ListNode dummy(0, head);
        ListNode* beLeft = &dummy;
        for (int i = 1; i < left; ++i) beLeft = beLeft->next;

        ListNode* prev = nullptr;
        ListNode* cur = beLeft->next;
        for (int i = 0; i <= right - left; ++i) {
            ListNode* next = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next;
        }
        // cur 现在是 afRight，prev 是反转段新头
        beLeft->next->next = cur;
        beLeft->next = prev;
        return dummy.next;
    }
};

class Solution69 {
public:
    string processStr(string s) {
        std::string result;
        for(char c: s){
            if(isalpha(c)){
                result.push_back(c);
            }else if(c == '*' && result.size() > 0){
                result.pop_back();
            }else if(c == '#'&& result.size() > 0){
                result += result;
            }else if(c == '%' && result.size() > 0){
                reverse(result.begin(), result.end());
            }else{
                continue;
            }
        }
        return result;
    }
};

class Solution70 {
public:
    ListNode* reverseKGroup(ListNode* head, int k){
        // 1.先检查够不都k个，不够直接返回
        ListNode *check = head;
        for(int i=0; i<k; ++i){
            while(!check) return head;
            check = check->next;
        }

        // 2.我们无条件相信该函数可以将后面剩余的部分反转好。
        // 我们只需要反转当前的即可。
        ListNode *prev = nullptr, *cur = head;
        for(int i=0; i<k; ++i){
            ListNode *next = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next;
        }

        // 3.此时prev是反转后的新头，head是尾，让head->next指向后面部分
        head->next = reverseKGroup(cur, k);
        return prev;
    }
    ListNode* _reverseKGroup(ListNode* head, int k) {
        ListNode dummy(0, head);
        ListNode *groupPrev = &dummy;
        for(;;){
            // 1. 判断链表够不够k个
            ListNode *check = groupPrev;
            for(int i=0; i<k; ++i){
                check = check->next;
                if(!check) return dummy.next;
            }
            // 2. 反转k个
            ListNode *prev = nullptr, *cur = groupPrev->next;
            for(int i=0; i<k; ++i){
                ListNode *next = cur->next;
                cur->next = prev;
                prev = cur;
                cur = next;
            }
            // 3. 更新指针
            // groupPrev prev cur 
            // groupPrev是前一组的头， prev是反转后的头，cur是待操作的头
            ListNode *groupTail = groupPrev->next;  // groupPrev->next原是头，反转后为尾
            groupTail->next = cur;  // 反转后的尾接到下一次的头
            groupPrev->next = prev; // 前一组的头接反转后的头
            groupPrev = groupTail;  // groupPrev走到下一次的前一个节点
        }
    }
};

class Solution71 {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        // 快慢指针，让快指针先走n，然后fast和slow同步开始走，保证两个指针之间的距离差一直是n
        // 当fast走到最后一个节点时，此时slow位置就是待删除节点的前一个
        ListNode dummy(0, head);
        ListNode *fast = &dummy, *slow = &dummy;
        for(int i=0; i<n; ++i){
            fast = fast->next;
        }
        while(fast->next){
            slow = slow->next;
            fast = fast->next;
        }
        ListNode *del = slow->next;
        slow->next = del->next;
        delete del;
        return dummy.next;
    }

    ListNode* _removeNthFromEnd(ListNode* head, int n) {
        if(!head->next) return nullptr;
        // 1. 先统计节点数
        int count = 0;
        ListNode *cur = head;
        while(cur){
            cur = cur->next;
            count++;
        }
        // 2. 将倒数第n个转换为整数第count-n个
        ListNode* prev = nullptr;
        cur = head;
        for(int i=0; i<count-n; ++i){
            prev = cur;
            cur = cur->next;
        }
        // 3. 删除cur节点
        ListNode* next = cur->next;
        if(!prev) head = next;
        else prev->next = next;
        delete cur;
        return head;
    }
};

class Solution72 {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if(head == nullptr) return head;
        ListNode dummy(0, head);
        ListNode *cur = head, *prev = &dummy;
        while(cur){
            int val = cur->val;
            int count = 1;
            ListNode *next = cur->next;
            while(next && next->val == val){
                next = next->next;
                count++;
            }
            // prev ... next
            if(count > 1){
                prev->next = next;
                while(cur != next){
                    ListNode *del = cur;
                    cur = cur->next;
                    delete del;
                }
            }else{
                prev = cur;
                cur = next;
            }
        }
        return dummy.next;
    }
};

class Solution73 {
 public:
  char processStr(string s, long long k) {
    long long cnts = 0;
    for (char c : s) {
      switch (c) {
        case '*': {
          if (cnts) --cnts;
        } break;
        case '#': {
          cnts *= 2;
        } break;
        case '%': {
        } break;
        default: {
          cnts++;
        } break;
      }
    }
    if (cnts <= k) return '.';
    for (int i = s.size() - 1; i >= 0; --i) {
      char c = s[i];
      switch (c) {
        case '*': {
          cnts++;
        } break;
        case '#': {
          cnts /= 2;
          if (k >= cnts) k -= cnts;
        } break;
        case '%': {
          k = cnts - 1 - k;
        } break;
        default: {
          if (--cnts == k) return c;
        } break;
      }
    }
    return '.';
  }
};

class Solution74 {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if(!head) return nullptr;
        ListNode *cur = head;
        int count = 1;
        while(cur->next){
            cur = cur->next;
            count++;
        }
        // 让链表成环
        cur->next = head;
        k %= count;
        int step = count - k;
        ListNode* prev = nullptr;
        while(step--){
            prev = head;
            head = head->next;
        }
        prev->next = nullptr;
        return head;
    }
};

class Solution75 {
public:
    ListNode* partition(ListNode* head, int x) {
        ListNode dummy1(0), dummy2(0);
        ListNode *tail1 = &dummy1, *tail2 = &dummy2;
        ListNode *cur = head;
        while(cur){
            if(cur->val < x) {
                tail1->next = cur;
                tail1 = cur;
            }else{
                tail2->next = cur;
                tail2 = cur;
            }
            cur = cur->next;
        }
        tail1->next = dummy2.next;
        tail2->next = nullptr;
        return dummy1.next;
    }
};

class LRUCache {
    struct LRUNode{
        int key, value;
        LRUNode* prev;
        LRUNode* next;
        LRUNode():key(0), value(0), prev(nullptr), next(nullptr){}
        LRUNode(int key, int value):key(key), value(value), prev(nullptr), next(nullptr){}
    };
public:
    LRUCache(int capacity): capacity(capacity), 
                            head(new LRUNode()), 
                            tail(new LRUNode()) {
        head->next = tail;
        head->prev = nullptr;
        tail->prev = head;
        tail->next = nullptr;
    }
    ~LRUCache(){
        while(head){
            auto next = head->next;
            head->next = nullptr;
            head->prev = nullptr;
            delete head;
            head = next;
        }
        head = tail = nullptr;
    }
    
    int get(int key) {
        if(!cache.count(key)) return -1;
        //访问存在的缓存，需要将其插入到头部，表示最近访问过
        move2Head(cache[key]);
        return cache[key]->value;
    }
    
    void put(int key, int value) {
        if(cache.count(key)){
            cache[key]->value = value;
            move2Head(cache[key]);
            return;
        }
        if(cache.size() + 1 > capacity){
            removeLRU();
        }
        LRUNode *newNode = new LRUNode(key, value);
        cache[key] = newNode;
        insert(newNode);
    }
private:
    void move2Head(LRUNode *node){
        if(node->prev == head) return;
        // 先从双向链表上切下来
        LRUNode *prev = node->prev, *next = node->next;
        prev->next = next;
        next->prev = prev;

        // 插入到头部
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

    void insert(LRUNode *node){
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

    void removeLRU(){
        // 插入元素后，大于capacity，删除最近最少使用的节点，也就是tail->prev
        // prev need tail
        LRUNode *need = tail->prev;
        LRUNode *prev = need->prev;
        prev->next = tail;
        tail->prev = prev;
        cache.erase(need->key);
        delete need;
    }

    std::unordered_map<int, LRUNode*> cache;
    LRUNode *head;
    LRUNode *tail;
    int capacity;
};

class Solution76 {
public:
    double angleClock(int hour, int minutes) {
        double minAngle = minutes * 6.0;
        double hourAngle = (hour % 12) * 30.0 + minutes * 0.5;
        double diff = abs(hourAngle - minAngle);
        return min(diff, 360.0 - diff);
    }
};

class Solution77 {
public:
    int maxDepth(TreeNode* root) {
        if(root == nullptr) return 0;
        return 1 + std::max(maxDepth(root->left), maxDepth(root->right));
    }
    int _maxDepth(TreeNode* root) {
        if(!root) return 0;
        std::queue<TreeNode*> q;
        q.push(root);
        int depth = 0;
        while(!q.empty()){
            int sz = q.size();
            while(sz--){
                TreeNode *node = q.front(); q.pop();
                if(node->left) q.push(node->left);
                if(node->right) q.push(node->right);
            }
            depth++;
        }
        return depth;
    }
};

class Solution78 {
public:
    bool isSameTree(TreeNode* p, TreeNode* q) {
        if (!p && !q) return true;       // 都空:相同
        if (!p || !q) return false;      // 一空一非空:不同
        return p->val == q->val
            && isSameTree(p->left, q->left)
            && isSameTree(p->right, q->right);
    }
    bool _isSameTree(TreeNode* p, TreeNode* q) {
        std::queue<TreeNode*> q1, q2;
        q1.push(p);
        q2.push(q);
        while (!q1.empty()) {
            TreeNode* node1 = q1.front(); q1.pop();
            TreeNode* node2 = q2.front(); q2.pop();
            if (!node1 && !node2) continue;         // 都空,这对没问题,看下一对
            if (!node1 || !node2) return false;     // 一空一非空,结构不同
            if (node1->val != node2->val) return false;
            q1.push(node1->left);  q2.push(node2->left);
            q1.push(node1->right); q2.push(node2->right);
        }
        return true;
    }
};

class Solution79 {
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

class Solution80 {
public:
    bool isSymmetric(TreeNode* root) {
        return check(root->left, root->right);
    }
private:
    bool check(TreeNode *left, TreeNode *right){
        if(!left && !right) return true; // 节点都为空，返回true
        if(!left || !right) return false; // 一个节点为空
        return left->val == right->val 
        && check(left->left, right->right) 
        && check(left->right, right->left);
    }
};

class Solution81 {
public:
    int largestAltitude(vector<int>& gain) {
        int ans = 0, sum = 0;
        for (int x: gain) {
            sum += x;
            ans = max(ans, sum);
        }
        return ans;
    }
};

class Solution82 {
    unordered_map<int, int> pos;  // 值 -> 在 inorder 中的下标
    vector<int> pre, in;

    TreeNode* build(int preL, int preR, int inL, int inR) {
        if (preL > preR) return nullptr;
        int rootVal = pre[preL];
        TreeNode* root = new TreeNode(rootVal);
        int k = pos[rootVal];          // 根在 inorder 中的位置
        int leftSize = k - inL;        // 左子树的节点数
        root->left  = build(preL + 1, preL + leftSize, inL, k - 1);
        root->right = build(preL + leftSize + 1, preR, k + 1, inR);
        return root;
    }
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        pre = preorder; in = inorder;
        for (int i = 0; i < in.size(); i++) pos[in[i]] = i;
        return build(0, pre.size() - 1, 0, in.size() - 1);
    }
};

class Solution83 {
public:
    int maxBuilding(int n, vector<vector<int>>& restrictions) {
        restrictions.push_back({1, 0});
        sort(restrictions.begin(), restrictions.end());
        // 末尾的 n 号楼没有上限,但为了统一处理可不加(它会被左侧约束)
        int m = restrictions.size();

        // 更新限制点高度
        // 从左到右传播
        for (int i = 1; i < m; i++) {
            int d = restrictions[i][0] - restrictions[i-1][0];
            restrictions[i][1] = min(restrictions[i][1], restrictions[i-1][1] + d);
        }
        // 从右到左传播
        for (int i = m - 2; i >= 0; i--) {
            int d = restrictions[i+1][0] - restrictions[i][0];
            restrictions[i][1] = min(restrictions[i][1], restrictions[i+1][1] + d);
        }

        int ans = 0;
        // 相邻限制点之间求峰值
        for (int i = 1; i < m; i++) {
            int l = restrictions[i-1][0], hl = restrictions[i-1][1];
            int r = restrictions[i][0],   hr = restrictions[i][1];
            int d = r - l;
            int peak = (hl + hr + d) / 2;
            ans = max(ans, peak);
        }
        // 最后一个限制点到 n 号楼:右侧无约束,可一路往上爬
        int lastId = restrictions[m-1][0], lastH = restrictions[m-1][1];
        ans = max(ans, lastH + (n - lastId));

        return ans;
    }
};

class Solution84 {
    std::unordered_map<int, int> pos; // 值->inorder下标
    std::vector<int> in, post; 

    TreeNode* build(int inL, int inR, int postL, int postR){
        if(postL > postR) return nullptr; // 区间不存在
        int rootVal = post[postR];
        TreeNode *root = new TreeNode(rootVal);
        int k = pos[rootVal];           // 根在中序数组中的位置
        int leftSize = k - inL;         // 左子树节点数
        root->left = build(inL, k - 1, postL, postL + leftSize - 1);
        root->right = build(k + 1, inR, postL + leftSize, postR - 1);
        return root;
    }
public:
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        in = inorder;
        post = postorder;
        for(int i=0; i<inorder.size(); ++i) pos[inorder[i]] = i;
        return build(0, in.size() - 1, 0, post.size() - 1);
    }
};

class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* next;

    Node() : val(0), left(NULL), right(NULL), next(NULL) {}

    Node(int _val) : val(_val), left(NULL), right(NULL), next(NULL) {}

    Node(int _val, Node* _left, Node* _right, Node* _next)
        : val(_val), left(_left), right(_right), next(_next) {}
};

class Solution85 {
public:
    Node* connect(Node* root) {
        Node* cur = root;
        while (cur) {
            Node dummy(0);          // 哑节点,作为下一层链表的"假头"
            Node* tail = &dummy;    // 尾指针,逐个挂上下一层的节点
            while (cur) {
                if (cur->left)  { tail->next = cur->left;  tail = tail->next; }
                if (cur->right) { tail->next = cur->right; tail = tail->next; }
                cur = cur->next;
            }
            cur = dummy.next;       // 下一层的真正头节点
        }
        return root;
    }

    Node* _connect(Node* root) {
        if(!root) return nullptr;
        std::queue<Node*> q;
        q.push(root);
        while(!q.empty()){
            int sz = q.size();
            while(sz--){
                Node *front = q.front(); q.pop();
                if(sz == 0) front->next = nullptr;
                else front->next = q.front();
                if(front->left) q.push(front->left);
                if(front->right) q.push(front->right);
            }
        }
        return root;
    }
};

class Solution86 {
public:
    int maxIceCream(vector<int>& costs, int coins) {
        sort(costs.begin(), costs.end());
        int count = 0;
        int n = costs.size();
        for (int i = 0; i < n; i++) {
            int cost = costs[i];
            if (coins >= cost) {
                coins -= cost;
                count++;
            } else {
                break;
            }
        }
        return count;
    }
};

class Solution87 {
public:
    int maxNumberOfBalloons(string text) {
        vector<int> cnt(5);
        for (auto & ch : text) {
            if (ch == 'b') {
                cnt[0]++;
            } else if (ch == 'a') {
                cnt[1]++;
            } else if (ch == 'l') {
                cnt[2]++;
            } else if (ch == 'o') {
                cnt[3]++;
            } else if (ch == 'n') {
                cnt[4]++;
            }
        }
        cnt[2] /= 2;
        cnt[3] /= 2;
        return *min_element(cnt.begin(), cnt.end());
    }
};

// ready for exam

class Solution89 {
public:
    void flatten(TreeNode* root) {
        std::function<void(TreeNode*, TreeNode*&)> dfs = [&](TreeNode* root, TreeNode*& prev){
            if(root == nullptr) return;
            dfs(root->right, prev);
            dfs(root->left, prev);
            root->right = prev;
            root->left = nullptr;
            prev = root; 
        };
        TreeNode *prev = nullptr;
        dfs(root, prev);
    }
    
};

class Solution90 {
public:
    bool hasPathSum(TreeNode* root, int targetSum) {
        if(root == nullptr) return false;
        if(root->left == nullptr && root->right == nullptr)
            return root->val == targetSum;
        return hasPathSum(root->left, targetSum-root->val)
        || hasPathSum(root->right, targetSum-root->val);
    }
};

class Solution91 {
public:
    int dfs(TreeNode* root, int cur){
        if(root == nullptr) return 0;
        cur = cur*10 + root->val;
        if(root->left == nullptr && root->right == nullptr)
            return cur;
        return dfs(root->left, cur) + dfs(root->right, cur);
    }
    int sumNumbers(TreeNode* root) {
        return dfs(root, 0);
    }
};