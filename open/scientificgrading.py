import sys
from decimal import Decimal, getcontext

getcontext().Emax, getcontext().Emin = 2_000_000_001, -2_000_000_001

x, y, epsilon = Decimal(sys.stdin.readline()), Decimal(sys.stdin.readline()), Decimal("1e-9")

def sgn(v):
    return (v > 0) - (v < 0)

def add(sum):
    z = x + y
    if not z: return not sum
    err = abs(sum - x - y)
    if err >= epsilon: return False
    if err < abs(z) * epsilon or (err == abs(z) * epsilon and sgn(x) == sgn(y) and (z == x or z == y)): return True
    return False

def sub(diff):
    z = x - y
    if not z: return not diff
    err = abs(diff - x + y)
    if err >= epsilon: return False
    if err < abs(z) * epsilon or (err == abs(z) * epsilon and sgn(x) != sgn(y) and (z == x or z == -y)): return True
    return False

def mul(product):
    z = x * y
    if not z: return not x or not y
    err = abs((product / x - y) * x)
    return err < epsilon and err < abs(z) * epsilon

def div(quotient):
    z = x / y
    if not z: return False
    err = abs((quotient * y - x) / y)
    return err < epsilon and err < abs(z) * epsilon

print("\n".join("Correct" if f(Decimal(sys.stdin.readline())) else "Incorrect" for f in [add, sub, mul, div]))
