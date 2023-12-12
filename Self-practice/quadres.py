for _ in range(int(input())):
    a, p = map(int, input().split())
    print(['no', 'yes'][pow(a, (p - 1) >> 1, p) != p - 1 or a % p == 0])
