MODULO = 10**9 + 7

n, T = map(int, input().split())
a, b, c, t0 = map(int, input().split())

count = [0] * (c + 1)
t, visited = [], {}
ti, i = t0, 0
while ti not in visited:
    visited[ti] = i
    count[ti] += 1
    t.append(ti)
    ti = (a * ti + b) % c + 1
    i += 1

j, k = divmod(n - visited[ti], i - visited[ti])
for i in range(visited[ti], len(t)): count[t[i]] += j - 1
for i in range(k): count[t[i + visited[ti]]] += 1

problems = penalty = elapsed = 0
for i in range(1, len(count)):
    if elapsed + count[i] * i <= T:
        problems += count[i]
        penalty += (elapsed * count[i] + count[i] * (count[i] + 1) // 2 * i) % MODULO
        elapsed += count[i] * i
    else:
        solved = (T - elapsed) // i
        problems += solved
        penalty += (elapsed * solved + solved * (solved + 1) // 2 * i) % MODULO
        break
        
print(problems, penalty % MODULO)
