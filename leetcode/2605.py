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