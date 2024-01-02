import heapq, math

while True:
    n, m, q, s = map(int, input().split())
    if m == n == q == s == 0: break

    adjList = [[] for i in range(n)]

    for _ in range(m):
        u, src, w = map(int, input().split())
        adjList[u].append((src, w))

    d = [math.inf for i in range(n)]
    pq = [(math.inf, i) for i in range(n)]
    heapq.heapify(pq)

    visited = [False for i in range(n)]
    d[s] = 0
    heapq.heappush(pq, (0, s))

    for i in range(n):
        while True:
            _, src = heapq.heappop(pq)
            if not visited[src]: break

        visited[src] = True
        for dest, weight in adjList[src]:
            if d[src] + weight < d[dest]:
                d[dest] = d[src] + weight
                heapq.heappush(pq, (d[dest], dest))

    for _ in range(q):
        src = int(input())
        print("Impossible" if d[src] == math.inf else d[src])

    print()
