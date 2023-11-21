import heapq

n, m = map(int, input().split())
adjList = [[] for _ in range(n)]
visited = [0] * n

for _ in range(m):
    u, v = map(int, input().split())
    adjList[u - 1].append(v - 1)
    adjList[v - 1].append(u - 1)

s = [int(input()) for _ in range(n)]

visited[0] = 1
total = s[0]

pq = [(s[v], v) for v in adjList[0]]
heapq.heapify(pq)

while pq and pq[0][0] < total:
    _, v = heapq.heappop(pq)
    if not visited[v]:
        total += s[v]
        visited[v] = 1
        for u in adjList[v]:
            if not visited[u]:
                heapq.heappush(pq, (s[u], u))

print(total)
