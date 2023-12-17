x = int(input())
(n, k) = (x, 1) if x > 1 else (0, 0)

for i in range(2, min(x + 1, 169)):
    l, r = i, x
    while l + 1 < r:
        m = l + (r - l) // 2
        c = 1
        for j in range(i):
            c *= m - j
            c //= j + 1
            if c > x:
                r = m
                break
        else:
            if c == x:
                (n, k) = min((n, k), (m, i))
                break
            elif c < x:
                l = m
            else:
                r = m

print(n, k)
