import random, math

def ctz(n):
    return (n & -n).bit_length() - 1

def isprime(n):
    if n < 2: return False
    if n % 6 % 4 != 1: return (n | 1) == 3

    bases = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41]
    s = ctz(n - 1)
    d = n >> s
    for a in bases:
        p = pow(a % n, d, n)
        i = s
        while 1 < p < n - 1 and (a % n) != 0 and i > 0:
            p = pow(p, 2, n)
            i -= 1
        if p != n - 1 and i != s: return False
    return True

def brent(n):
    if n & 1 == 0: return 2
    if isprime(n): return n

    while True:
        x, y, g, q, xs, c, i = 2, 2, 1, 1, 1, random.randint(1, n - 1), 1
        while g == 1:
            for j in range(1, i): x = (x * x) % n + c
            for j in range(i):
                xs = x
                for _ in range(min(128, i - j)):
                    x = (x * x) % n + c
                    q = (q * (max(x, y) - min(x, y))) % n
                g = math.gcd(q, n)
                if g != 1: break
                j += 128
            i <<= 1
            y = x

        if g == n: g = 1
        while g == 1:
            xs = (xs * xs) % n + c
            diff = max(xs, y) - min(xs, y)
            g = math.gcd(diff, n)

        if g != n:
            return g if isprime(g) else brent(g)

def factorize(n):
    pfs = []
    def dfs(m):
        if m < 2:
            return
        if isprime(m):
            pfs.append(m)
            return
        d = brent(m)
        dfs(d)
        dfs(m // d)
    dfs(n)
    return pfs

n = int(input())
pfs = factorize(n)
pfs.sort()
print(' '.join(str(p) for p in pfs))

