s = input()
dp = [0, len(s) + 1]
for i in range(len(s) >> 1):
    dp = [dp[1], min(dp) + 1] if s[i] != s[-i-1] else [min(dp[0], dp[1] + 1), dp[1] + 1]
print(min(dp))
