class Solution:
    def sortColors(self, nums: List[int]) -> None:
        """
        Do not return anything, modify nums in-place instead.
        """
        left, right = 0, len(nums)-1
        i = 0
        while i <= right:
            if nums[i] == 0:
                nums[i], nums[left] = nums[left], nums[i]
                i += 1
                left += 1
            elif nums[i] == 1:
                i += 1
            else:
                nums[i], nums[right] = nums[right], nums[i]
                right -= 1

                class Solution {
public:
    int majorityElement(vector<int>& nums) {
        int candidate = 0, count = 0;
        for(int val:nums){
            if(count == 0){
                candidate = val;
            }
            count = candidate == val ? count+1: count-1;
        }
        return candidate;
    }
};

class Solution:
    def singleNumber(self, nums: List[int]) -> int:
        ans = 0
        for i in nums:
            ans ^= i
        return ans

        class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        int n = nums.size();
        int i = n - 2;
        while(i >= 0 && nums[i] >= nums[i+1]){
            i--;
        }
        if(i>=0){
            int j = n - 1;
            while(nums[j] <= nums[i]) {
                j--;
            }
            std::swap(nums[i], nums[j]);
        }
        std::sort(nums.begin() + i + 1, nums.end());
    }
};

class Solution:
    def findDuplicate(self, nums: List[int]) -> int:
        slow, fast = nums[0], nums[nums[0]]
        while slow != fast:
            slow = nums[slow]
            fast = nums[nums[fast]]
        slow = 0
        while slow != fast:
            slow = nums[slow]
            fast = nums[fast]
        return slow

class Solution:
    def maxSubArray(self, nums: List[int]) -> int:
        # dp[i]表示以i结尾的最大连续子数组的和
        # dp[i] = max(dp[i-1]+nums[i], nums[i])
        cur_sum = max_sum = nums[0]
        for num in nums[1:]:
            cur_sum = max(cur_sum+num, num)
            max_sum = max(max_sum, cur_sum)
        return max_sum

class Solution:
    def merge(self, intervals: List[List[int]]) -> List[List[int]]:
        intervals.sort()
        ans = []
        for cur in intervals:
            if ans and ans[-1][1] >= cur[0]:
                ans[-1][1] = max(ans[-1][1], cur[1])
            else:
                ans.append(cur)
        return ans

        class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        int n = nums.size();
        k %= n;
        std::reverse(nums.begin(), nums.end());
        std::reverse(nums.begin(), nums.begin() + k);
        std::reverse(nums.begin() + k, nums.end());
    }
};

class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int size = nums.size();
        std::vector<int> result(size, 1);

        for(int i=1; i<size; i++) result[i] = result[i-1] * nums[i-1];
        int tmp = 1;
        for(int i = size - 2; i>=0; i--) {
            tmp *= nums[i+1];
            result[i] *= tmp;
        }
        return result;
    }
};