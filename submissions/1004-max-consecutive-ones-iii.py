class Solution(object):
    def longestOnes(self, nums, k):
        """
        :type nums: List[int]
        :type k: int
        :rtype: int
        """
        max_len = 0
        cnt_one = 0
        left = 0
        cnt = 0
        for right in range(len(nums)):
            if nums[right] == 1:
                cnt += 1
            window_len = right - left + 1
            while right - left + 1 - cnt > k:
                if nums[left] == 1:
                    cnt -= 1
                left += 1
            max_len = max(max_len, right - left + 1)
        return max_len
