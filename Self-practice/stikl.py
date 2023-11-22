def nge(n, tiles, queries):
    stack, size = [0] * n, 0
    for i in range(n):
        while size > 0 and tiles[stack[size - 1]] <= tiles[i]:
            queries[stack[size - 1]] = i
            size -= 1
        stack[size] = i
        size += 1
    while size > 0:
        queries[stack[size - 1]] = -1
        size -= 1


n, q = map(int, input().split())
tiles = list(map(int, input().split()))
queries = [[0] * n for _ in range(18)]
nge(n, tiles, queries[0])

for i in range(1, 18):
    for j in range(n):
        queries[i][j] = -1 if queries[i - 1][j] == -1 else queries[i - 1][queries[i - 1][j]]

for _ in range(q):
    source, steps = map(int, input().split())
    source -= 1
    for i in range(17, -1, -1):
        if steps >= (1 << i):
            steps -= (1 << i)
            source = -1 if source == -1 else queries[i][source]
    print(source + 1 if source != -1 else "leik lokid")
