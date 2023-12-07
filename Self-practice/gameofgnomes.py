n, m, k = map(int, input().split())
print(max((((n - i) // k + 1) * (n + i) - 2 * i + (i // m) * (m*m + m) + (i % m) * (i % m + 1)) >> 1 for i in range(n % k, min(n + 1, m * k), k)))
