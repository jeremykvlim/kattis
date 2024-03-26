import heapq, math

n = int(input())
points = [list(map(int, input().split())) for _ in range(n + 2)]
d = [[(dx := xi - xj) * dx + (dy := yi - yj) * dy for xj, yj in points] for xi, yi in points]

parent, dist = [-1] * (n + 2), [math.inf] * (n + 2)
dist[n] = 0
pq = [(0, n)]
while pq:
    w, v = heapq.heappop(pq)
    if dist[v] < w: continue
    for u in range(n + 2):
        if dist[u] > w + d[v][u]:
            dist[u] = w + d[v][u]
            heapq.heappush(pq, (w + d[v][u], u))
            parent[u] = v

path = []
curr = parent[-1]
while parent[curr] != -1:
    path.append(curr)
    curr = parent[curr]

print("-" if not path else "\n".join(map(str, reversed(path))))
