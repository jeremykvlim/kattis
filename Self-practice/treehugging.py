from io import StringIO

n = int(input())
adj_list = [set() for _ in range(2 * n)]
leaves = []

for e in range(2 * (n - 1)):
    u, v = map(int, input().split())
    adj_list[v].add((n + u, e))
    adj_list[n + u].add((v, e))

for u in range(2, 2 * n):
    if not adj_list[u]:
        print('impossible')
        exit(0)
    if len(adj_list[u]) == 1:
        leaves.append(u)

solution = StringIO()

while leaves:
    u = leaves.pop()
    if not adj_list[u]:
        print('impossible')
        exit(0)
    (v, e) = adj_list[u].pop()
    solution.seek(e)
    solution.write('L') if u <= n else solution.write('R')
    adj_list[v].remove((u, e))
    if len(adj_list[v]) == 1:
        leaves.append(v)

for u in range(2, 2 * n):
    while adj_list[u]:
        (v, e) = adj_list[u].pop()
        solution.seek(e)
        solution.write('L') if u <= n else solution.write('R')
        adj_list[v].remove((u, e))
        u = v

print(solution.getvalue())
