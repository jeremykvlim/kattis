import sys

n = int(sys.stdin.readline())
e = [0] + [0 if p == -1 else p for p in map(int, sys.stdin.readline().split())]
s = sys.stdin.readline().strip()

val, exp = [0] * (n + 1), [0] * (n + 1)
for i in range(n, 0, -1):
    p, q = val[i], exp[i]
    sgn = 1 if s[i - 1] == 'B' else -1
    p *= sgn
    if p < 0:
        p2 = 1 << q
        k = (2 * p2 - p - 1) // p2
        p += (1 << q) * k
        q += k - 1
    else: p += 1 << q
    p *= sgn

    P, Q = val[e[i]], exp[e[i]]
    if not P: P, Q = p, q
    elif p:
        if Q >= q: p <<= Q - q
        else:
            P <<= q - Q
            Q = q
        P += p
    val[e[i]], exp[e[i]] = P, Q

P, Q = val[0], exp[0]
while Q and not P & 1:
    P >>= 1
    Q -= 1

print(P if not Q else f"{P}/2^{Q}")
