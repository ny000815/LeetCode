class Solution(object):
    def coinChange(self, coins, amount):
        """
        :type coins: List[int]
        :type amount: int
        :rtype: int
        """
        INF = float('inf')
        dp = [INF]* (amount+1)
        dp[0] = 0
        for i in range(amount+1):
            for c in coins:
                if c <= i and dp[i-c] != INF:
                    dp[i] = min(dp[i], dp[i-c]+1)
        return dp[amount] if dp[amount]!= INF else -1
        
        
