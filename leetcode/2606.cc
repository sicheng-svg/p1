#include <vector>

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