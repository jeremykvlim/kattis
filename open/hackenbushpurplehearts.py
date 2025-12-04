import sys

n = int(sys.stdin.readline())
e = [0] + [0 if p == -1 else p for p in map(int, sys.stdin.readline().split())]
s = sys.stdin.readline().strip()

def normalize(value, exponent):
    if not value: return 0, 0
    while exponent and not value & 1:
        value >>= 1
        exponent -= 1
    return value, exponent

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
    p, q = normalize(p, q)
    p *= sgn

    P, Q = val[e[i]], exp[e[i]]
    if not P: P, Q = p, q
    elif p:
        if Q >= q: p <<= Q - q
        else:
            P <<= q - Q
            Q = q
        P, Q = normalize(P + p, Q)
    val[e[i]], exp[e[i]] = P, Q

P, Q = val[0], exp[0]
print(P if not Q else f"{P}/2^{Q}")
