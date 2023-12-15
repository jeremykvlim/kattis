n, xy, x1, x2, y1, y2 = int(input()), 0, 0, 0, 0, 0
for _ in range(n):
    x, y = map(int, input().split())
    xy += x * y
    x1 += x
    x2 += x**2
    y1 += y
    y2 += y**2
c = n * (x2 * y2 - xy**2) + 2 * x1 * y1 * xy - y2 * x1**2 - x2 * y1**2
print(f'{c // 4}.{2500 * (c % 4):04d}' if c % 4 else f'{c // 4}.0000')
