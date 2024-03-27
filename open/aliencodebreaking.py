import sys

def convert(n, radix, digits, index, power=-1):
    if digits[-1] != -1: return

    if power == 0:
        if digits[0] == -1 and not n: return
        digits[index[0]] = n
        index[0] += 1
        return

    if power == -1:
        power = 1
        while radix ** (power + 1) <= n: power <<= 1

    l, r = divmod(n, radix ** power)
    power >>= 1

    convert(l, radix, digits, index, power)
    convert(r, radix, digits, index, power)

def itoc(i):
    return ' ' if i == 26 else chr(i + 65)

def ctoi(c):
    return 26 if c == ' ' else ord(c) - 65


MODULO = 1 << 20

sys.set_int_max_str_digits(2000000)
n, x = map(int, input().split())
cipher = input()

f = [1] * MODULO
for i in range(1, MODULO):
    f[i] = (33 * f[i - 1] + 1) % MODULO

sum = j = 0
for i in range(x):
    sum = (sum + f[j]) % MODULO
    j = (j + x) % MODULO

digits10 = []
for i in range(x):
    digits10.append(str(sum))
    sum = (33 * sum + x) % MODULO
digits10 = ''.join(digits10)

digits27, index = [-1] * (len(cipher)), [0]
convert(int(digits10), 27, digits27, index)
print(*[itoc((ctoi(cipher[i]) + d + 27) % 27) for i, d in enumerate(digits27)], sep='')
