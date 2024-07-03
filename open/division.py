import sys, math

for line in sys.stdin:
    t, a, b = map(int, line.split())
    f = f"({t}^{a}-1)/({t}^{b}-1)"
    if t < 2 or a % b or (a - b) * math.log10(t) >= 99: print(f, "is not an integer with less than 100 digits.")
    elif a == b: print(f, 1)
    elif (t**a - 1) // (t**b - 1) >= 10**99: print(f, "is not an integer with less than 100 digits.")
    else: print(f, (t**a - 1) // (t**b - 1))
