import math

def C(n, k):
    if k == 0: return 1
    return C(n - 1, k - 1) * n // k


def search(x):
    k = 1
    while (2 * k + 1) * x > 4**k:
        n, c = 1, math.factorial(k) * x
        while n**k < c: n <<= 1

        l, r = 1, n
        while l + 1 < r:
            m = l + (r - l) // 2
            if m**k < c: l = m
            else: r = m

        l = max(r, 2 * k)
        c = C(l, k)
        for n in range(l, r + k + 1):
            if c == x: yield n, min(k, n - k)
            c *= n + 1
            c //= n + 1 - k

        k += 1


x = int(input())
if x == 1:
    print(0, 0)
    exit(0)

n, k = x, 1
for new_n, new_k in search(x):
    if new_n < n: n, k = new_n, new_k

print(n, k)
