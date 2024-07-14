import math

def C(n, k):
    if k == 0: return 1
    return C(n - 1, k - 1) * n // k

x = int(input())
if x == 1:
    print(0, 0)
    exit(0)

n, k, K = x, 1, 1
while (2 * K + 1) * x > 4**K:
    l, r, c = 1, 1, math.factorial(K) * x
    while r**K < c:
        l = r
        r <<= 1

    while l + 1 < r:
        m = l + (r - l) // 2
        if m**K < c: l = m
        else: r = m

    l = max(r, 2 * K)
    c = C(l, K)
    for N in range(l, r + K + 1):
        if c == x and n > N: n, k = N, min(K, N - K)
        c *= N + 1
        c //= N + 1 - K

    K += 1

print(n, k)
