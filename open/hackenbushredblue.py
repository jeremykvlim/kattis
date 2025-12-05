import sys
from functools import lru_cache
from collections import deque

it = iter(sys.stdin.read().split())
n, m = int(next(it)), int(next(it))

edges = [None] * m
color = [''] * m
adj_list = [[] for _ in range(n + 1)]
for i in range(m):
    x = int(next(it))
    y = int(next(it))
    c = next(it)[0]
    if x == -1: x = 0
    if y == -1: y = 0

    edges[i] = (x, y)
    color[i] = c
    adj_list[x].append((y, i))
    adj_list[y].append((x, i))

@lru_cache(None)
def prune(m0):
    visited = [False] * (n + 1)
    visited[0] = True
    q = deque([0])
    while q:
        v = q.popleft()
        for u, i in adj_list[v]:
            if m0 >> i & 1 and not visited[u]:
                visited[u] = True
                q.append(u)

    m1 = 0
    for i in range(m):
        if m0 >> i & 1 and visited[edges[i][0]] and visited[edges[i][1]]: m1 |= 1 << i
    return m1

@lru_cache(None)
def dfs(m0):
    if not m0: return 0, 0

    visited = [False] * (n + 1)
    comps = []
    for s in range(1, n + 1):
        if visited[s] or not any(m0 >> i & 1 for _, i in adj_list[s]): continue
        m1 = 0
        visited[s] = True
        q = deque([s])
        while q:
            v = q.popleft()
            for u, i in adj_list[v]:
                if m0 >> i & 1:
                    m1 |= 1 << i
                    if u and not visited[u]:
                        visited[u] = True
                        q.append(u)

        comps.append(m1)
    if len(comps) > 1:
        p0, q0 = 0, 0
        for m1 in sorted(comps):
            p1, q1 = dfs(m1)
            if q0 < q1:
                p0 <<= q1 - q0
                q0 = q1
            else: p1 <<= q0 - q1
            p0 += p1
        return p0, q0

    x = y = False
    px = qx = py = qy = 0
    m1 = m0
    while m1:
        lsb = m1 & -m1
        m1 ^= lsb
        p, q = dfs(prune(m0 ^ lsb))

        def cmp(p1, q1, p2, q2):
            if q1 < q2: p1 <<= q2 - q1
            elif q1 > q2: p2 <<= q1 - q2
            return p1 - p2

        if color[lsb.bit_length() - 1] == 'B':
            if not x or cmp(p, q, px, qx) > 0:
                px, qx = p, q
                x = True
        else:
            if not y or cmp(p, q, py, qy) < 0:
                py, qy = p, q
                y = True

    def scale(p, q, Q):
        if q >= Q: return p >> q - Q
        return p << Q - q

    if x and y:
        l, r = -1, max(qx, qy) + 1
        while l + 1 < r:
            m = l + (r - l) // 2
            if scale(px, qx, m) + 1 < -scale(-py, qy, m): r = m
            else: l = m
        return scale(px, qx, r) + 1, r
    if x:
        if px < 0: return 0, 0
        return scale(px, qx, 0) + 1, 0
    if y:
        if py > 0: return 0, 0
        P = scale(py, qy, 0)
        return P - 1 if not qy and P == py else P, 0
    return 0, 0

P, Q = dfs(prune((1 << m) - 1))
while Q and not P & 1:
    P >>= 1
    Q -= 1

print(P if not Q else f"{P}/2^{Q}")
