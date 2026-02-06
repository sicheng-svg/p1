#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <functional>
#include <iostream>
#include <algorithm>

class Solution1 {
public:
    int findMaxLength(std::vector<int>& nums) {
        // 解题思路：将0看作-1，题目则转换为最长连续子数组，和为0
        // 如果0~i，下标的前缀和为x，0~j下标的其前缀和也为x
        // 则，i~j这个区间的和就为0.

        // 前缀和数组，key：前缀和，value，出现该前缀和的下标
        std::unordered_map<int, int> mp;
        mp[0] = -1;
        int counter = 0, size = nums.size();
        int ret = 0;

        for(int i=0; i<size; ++i) {
            counter += (nums[i] == 0 ? -1 : 1);
            if(mp.count(counter)){
                // 说明本次前缀和，出现过一次了。可以计算长度了
                ret = std::max(ret, i - mp[counter]);
            }else {
                // 说明这个前缀和之间并没有出现过，存入到map中
                mp[counter] = i;
            }
        }
        return ret;
    }
};

class Solution2 {
public:
    std::vector<std::vector<int>> merge(std::vector<std::vector<int>>& intervals) {
        int size = intervals.size();
        if(size <= 1) return intervals;

        // 1.先按照每个数组的第一个元素进行排序，将区间开头小的放在前面
        std::sort(intervals.begin(), intervals.end(), 
            [](std::vector<int>& v1, std::vector<int>& v2){
                return v1[0] < v2[0];
        });

        // 2.让后进行判断，如果前一个数组的第二个元素，小于后一个数组的第一个元素，说明可以合并
        std::vector<std::vector<int>> ret;
        ret.emplace_back(intervals[0]);

        for(int i=1; i<size; ++i) {
            if(intervals[i][0] > ret.back()[1]) {
                ret.emplace_back(intervals[i]);
            }else{
                ret.back()[1] = std::max(ret.back()[1], intervals[i][1]);
            }
        }
        return ret;
    }
};

class Solution3 {
public:
    bool isNumber(const std::string& str) {
        return !(str == "+" || str == "-" || str == "*" || str == "/");
    }

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

class Solution4 {
public:
    bool visited[128] = {false};
    bool isUnique(std::string astr) {
        if(astr.size() > 128) return false;
        for(char e : astr) {
            if(visited[e]) {
                return false;
            }
            visited[e] = true;
        }
        return true;
    }
};

class Solution5 {
public:
    int missingNumber1(std::vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int n = nums.size();
        int i = 0;
        for(i=0; i<n; ++i) {
            if(nums[i] != i) return i;
        }
        return i;
    }

    int missingNumber2(std::vector<int>& nums) {
        std::unordered_set<int> hash;
        int n = nums.size();
        for(int v : nums) hash.emplace(v);
        for(int i=0; i<=n; ++i) {
            if(!hash.count(i)) return i;
        }
        return -1;
    }

        int missingNumber(std::vector<int>& nums) {
            int n = nums.size();
            int ret = 0;
            for(int i=0; i<=n; i++) ret ^= i;
            for(int v : nums) ret ^= v;
            return ret;
    }
};

class MinStack6 {
public:
    MinStack6() {
    }
    
    void push(int val) {
        if(_min.empty() || val <= _min.top()) {
            _st.push(val);
            _min.push(val);
        }else {
            _st.push(val);
        }
    }
    
    void pop() {
        if(_st.top() == _min.top()) {
            _st.pop();
            _min.pop();
        }else{
            _st.pop();
        }
    }
    
    int top() {
        return _st.top();
    }
    
    int getMin() {
        return _min.top();
    }
private:
    std::stack<int> _st;
    std::stack<int> _min;
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(val);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */

int main()
{
    return 0;
}