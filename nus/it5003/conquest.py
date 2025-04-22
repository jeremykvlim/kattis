import heapq

n, m = map(int, input().split())
adj_list, visited = [[] for _ in range(n)], [False] * n

for _ in range(m):
    u, v = map(int, input().split())
    adj_list[u - 1].append(v - 1)
    adj_list[v - 1].append(u - 1)

size = [int(input()) for _ in range(n)]
total, visited[0] = size[0], True
pq = [(size[v], v) for v in adj_list[0]]
heapq.heapify(pq)
while pq and pq[0][0] < total:
    _, v = heapq.heappop(pq)
    if not visited[v]:
        total += size[v]
        visited[v] = True
        for u in adj_list[v]:
            if not visited[u]:
                heapq.heappush(pq, (size[u], u))

print(total)
