import random, math

def ctz(n):
    return (n & -n).bit_length() - 1

def isprime(n):
    if n < 2: return False
    if n in (2, 5, 11): return True
    if n % 6 % 4 != 1: return (n | 1) == 3

    def miller_rabin(a):
        s = ctz(n - 1)
        d = n >> s
        x = pow(a % n, d, n)
        if x == 1 or x == n - 1: return True

        for _ in range(s):
            x = (x * x) % n
            if x == n - 1: return True

        return False

    if not miller_rabin(2) or not miller_rabin(3): return False

    def jacobi_symbol(D, n):
        jacobi = 1
        a = D % n
        
        while a:
            while not a & 1:
                a >>= 1
                if n & 7 in (3, 5): jacobi = -jacobi
            if a & 3 == 3 and n & 3 == 3: jacobi = -jacobi
            a, n = n % a, a
        return jacobi if n == 1 else 0

    def lucas_pseudoprime(n):
        D = -3
        while True:
            D += 2 if D > 0 else -2
            D *= -1

            jacobi = jacobi_symbol(D, n)
            if jacobi == 0: return False
            if jacobi == -1: break

        def div2mod(x):
            return ((x + n) >> 1 if x & 1 else x >> 1) % n

        U, V = 1, 1
        for b in bin(n + 1)[3:]:
            U, V = (U * V) % n, div2mod(V * V + D * U * U)
            if b == "1": U, V = div2mod(U + V), div2mod(D * U + V)
        return U == 0

    return lucas_pseudoprime(n)

def brent(n):
    if n & 1 == 0: return 2

    while True:
        x, y, g, q, xs, c, i = 2, 2, 1, 1, 1, random.randint(1, n - 1), 1
        while g == 1:
            for j in range(1, i): x = (x * x) % n + c
            for j in range(i):
                xs = x
                for _ in range(min(128, i - j)):
                    x = (x * x) % n + c
                    q = (q * abs(x - y)) % n
                g = math.gcd(q, n)
                if g != 1: break
                j += 128
            i <<= 1
            y = x

        if g == n: g = 1
        while g == 1:
            xs = (xs * xs) % n + c
            g = math.gcd(abs(xs - y), n)

        if g != n: return g if isprime(g) else brent(g)

def factorize(n):
    pfs = []
    def dfs(m):
        if m < 2:
            return
        if isprime(m):
            pfs.append(m)
            return
        pf = brent(m)
        pfs.append(pf)
        dfs(m // pf)
    dfs(n)
    return pfs

n = int(input())
print(' '.join(str(p) for p in sorted(factorize(n))))
