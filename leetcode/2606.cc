#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
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