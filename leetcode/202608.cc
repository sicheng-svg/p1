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