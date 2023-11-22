n, tiles = int(input()), sorted(map(int, input().split()))
print(sum((tiles[i + 1] - tiles[i])**2 for i in range(n - 1)))
