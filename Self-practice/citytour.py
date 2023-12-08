for x in range(int(input())):
    n = int(input())
    points, matrix, y = [list(map(int, input().split())) for _ in range(n - 3)], [[0] * n for _ in range(n)], 0
    for i in range(3):
        for j in range(i + 1, 3):
            matrix[i][j] = 1
    for i, (a, b) in enumerate(points):
        matrix[a - 1][i + 3] = matrix[b - 1][i + 3] = 1
    for i in range(2, n)[::-1]:
        a, b = [j for j in range(i) if matrix[j][i] > 0]
        y, matrix[a][b] = max(y, matrix[a][b] + matrix[a][i] + matrix[b][i]), max(matrix[a][b], matrix[a][i] + matrix[b][i])
    print(f"Case #{x + 1}: {y}")
