import sys

sys.set_int_max_str_digits(2000000)

def parse(s):
    if '/2^' in s:
        P, Q = s.split('/2^')
        return int(P), int(Q)
    return int(s), 0

x_numer, x_denom_exp = parse(sys.stdin.readline())
y_numer, y_denom_exp = parse(sys.stdin.readline())

def scale(p, q, Q):
    if q >= Q: return p >> q - Q
    return p << Q - q

l, r = -1, max(x_denom_exp, y_denom_exp) + 1
while l + 1 < r:
    m = l + (r - l) // 2
    if scale(x_numer, x_denom_exp, m) + 1 < -scale(-y_numer, y_denom_exp, m): r = m
    else: l = m

P, Q = scale(x_numer, x_denom_exp, r) + 1, r
while Q and not P & 1:
    P >>= 1
    Q -= 1

print(P if not Q else f"{P}/2^{Q}")
