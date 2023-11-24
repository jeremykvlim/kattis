points = [100, 75, 60, 50, 45, 40, 36, 32, 29, 26, 24, 22, 20, 18, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1]

n, m = map(int, input().split())
contests = [sorted(map(int, input().split()), reverse=True)[:4] for _ in range(m)]
rank = sum(sum(scores) > sum(contests[0]) for scores in contests[1:]) + 1
diff = sorted(sum(contests[0]) - sum(scores[:3]) + 1 for scores in contests[1:] if sum(scores) <= sum(contests[0]))

l = 0
r = len(diff) + 1
while l < r - 1:
    m = l + (r - l) // 2
    contestants = total = 0
    for i in range(m):
        contestants += 1
        min = diff[i]
        total += 1 + (points[m - i - 1] if m - i <= 30 else 0)
        if (total + contestants - 1) // contestants >= min: 
            total = contestants = 0
    if contestants == 0:
        l = m
    else:
        r = m

print(rank + l)
