MODULO = 10 ** 9 + 7

def f(x, dp):
    if x not in dp:
        odd, even = (x + 1) // 2, x // 2
        dp[x] = (even + 2 * (odd - 1) + f(odd, dp) + f(even, dp)) % MODULO
    return dp[x]

l, r = map(int, input().split())
dp = {0: 0, 1: 0}
print((f(r, dp) - f(l - 1, dp) + MODULO) % MODULO)
