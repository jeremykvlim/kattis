import heapq

n, m = map(int, input().split())
adjList, visited = [[] for _ in range(n)], [False] * n

for _ in range(m):
    u, v = map(int, input().split())
    adjList[u - 1].append(v - 1)
    adjList[v - 1].append(u - 1)

size = [int(input()) for _ in range(n)]
total, visited[0] = size[0], True
pq = [(size[v], v) for v in adjList[0]]
heapq.heapify(pq)
while pq and pq[0][0] < total:
    _, v = heapq.heappop(pq)
    if not visited[v]:
        total += size[v]
        visited[v] = True
        for u in adjList[v]:
            if not visited[u]:
                heapq.heappush(pq, (size[u], u))

print(total)
