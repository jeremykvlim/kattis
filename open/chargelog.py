MOD = 10**9 + 7

def mul(x, y, mod):
    return (x[0] * y[1] + y[0]) % mod, x[1] * y[1] % mod

def pow(base, exponent, mod):
    value = (0, 1)
    while exponent:
        if exponent & 1: value = mul(value, base, mod)
        base = mul(base, base, mod)
        exponent >>= 1
    return value


ha, hb, c = map(int, input().split())
a, b = map(int, input().split())
s = (ha, c)
r = (hb, c)

while a and b:
    if a >= b:
        q, a = divmod(a, b)
        s = mul(pow(r, q, MOD), s, MOD)
    else:
        q, b = divmod(b, a)
        r = mul(r, pow(s, q, MOD), MOD)

print(s[0] if not a else r[0])