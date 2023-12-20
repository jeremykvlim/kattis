from math import *

n, a, b = map(int, input().split())
a, b = a // gcd(a, b), b // gcd(a, b)

if n / a / b > 1:
    slice = isqrt(n // a // b)
    while slice * a * slice * b < n:
        slice += 1
    a, b = a * slice, b * slice

least = 0
while a > 1 or b > 1:
    a, b = max(a, b), min(a, b)
    a = (a + 1) // 2
    least += 1

print(least)
