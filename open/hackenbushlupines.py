import sys

it = iter(sys.stdin.read().split())
P = Q = 0
for _ in range(int(next(it))):
    s = next(it)
    l = len(s)
    i = 1
    while i < l and s[i] == s[0]: i += 1
    
    q = l - i
    p = i << q
    v = 1 << (q - 1) if q else 0
    for j in range(i, l):
        p += v if s[j] == s[0] else -v
        v >>= 1
    if s[0] == 'R': p = -p
    if Q >= q: p <<= Q - q
    else:
        P <<= q - Q
        Q = q
    P += p

while Q and not P & 1:
    P >>= 1
    Q -= 1

print(P if not Q else f"{P}/2^{Q}")