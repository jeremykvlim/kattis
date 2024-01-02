import math

def move(i, j):
    mask = int(''.join(map(str, visited)), 2)
    if dp[i][j][mask] != -1: return dp[i][j][mask]

    dp[i][j][mask] = math.inf
    for k in range(len(coordinates)):
        if not visited[k]:
            visited[k] = 1
            dp[i][j][mask] = min(dp[i][j][mask], abs(i - coordinates[k][0]) + abs(j - coordinates[k][1]) + move(*coordinates[k]))
            visited[k] = 0

    if dp[i][j][mask] == math.inf: dp[i][j][mask] = abs(i - start[0]) + abs(j - start[1])
    return dp[i][j][mask]


for _ in range(int(input())):
    x, y = map(int, input().split())
    start, beepers = list(map(int, input().split())), int(input())
    coordinates = [list(map(int, input().split())) for _ in range(beepers)]

    dp, visited = [[[-1 for _ in range(1 << beepers)] for _ in range(y + 1)] for _ in range(x + 1)], [0] * beepers
    print(move(*start))
