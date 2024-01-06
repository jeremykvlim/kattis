a = input()
p, m = map(int, input().split())
x = 0
b = eval(a)
x = 1
c = eval(a) - b
x = next(x for x in range(1, m) if (c * x + b) % m == p)
print(x)
