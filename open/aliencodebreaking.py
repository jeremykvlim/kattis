import sys

def convert(x: int, radix: int, digits: list[int]) -> None:
    radix_pows = [radix]
    value, exponent = radix, 1
    while value * radix <= x:
        value = value * value
        radix_pows.append(value)
        exponent <<= 1
    m = len(digits)

    def dnc(x: int, exponent: int, index):
        if index >= m: return index

        if not exponent:
            if not index and not x: return index
            digits[index] = x
            return index + 1

        l, r = divmod(x, radix_pows[exponent.bit_length() - 1])
        exponent >>= 1
        
        index = dnc(l, exponent, index)
        if index < m: index = dnc(r, exponent, index)
        return index

    dnc(x, exponent, 0)


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

digits = [0] * len(cipher)
convert(int(digits10), 27, digits)

text = []
for d, c in enumerate(cipher):
    i = ((26 if c == ' ' else ord(c) - 65) + digits[d] + 27) % 27
    text.append(' ' if i == 26 else chr(i + 65))
sys.stdout.write(''.join(text))
