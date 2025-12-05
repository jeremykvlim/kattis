import sys

sys.set_int_max_str_digits(100000)

def parse(s):
    if '/2^' in s:
        P, Q = s.split('/2^')
        return int(P), int(Q)
    return int(s), 0

px, qx = parse(sys.stdin.readline())
py, qy = parse(sys.stdin.readline())

def scale(p, q, Q):
    if q >= Q: return p >> q - Q
    return p << Q - q

l, r = -1, max(qx, qy) + 1
while l + 1 < r:
    m = l + (r - l) // 2
    if scale(px, qx, m) + 1 < -scale(-py, qy, m): r = m
    else: l = m

P, Q = scale(px, qx, r) + 1, r
while Q and not P & 1:
    P >>= 1
    Q -= 1

print(P if not Q else f"{P}/2^{Q}")
