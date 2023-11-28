for t in range(int(input())):
    n, m = map(int, input().split())
    mask = [0] * m
    statements = [input().split() for _ in range(m)]
    for i, s in enumerate(statements):
        for p in [s[0]] + s[2:]:
            mask[i] ^= 1 << int(p)
        if s[1] == 'S' or s[1] == 'L':
            mask[i] ^= 1

    for i in range(1, n + 1):
        relevant = [j for j in mask if (j >> i) % 2 and j & ((1 << i) - 2) == 0]
        if relevant:
            mask = [j ^ (0, relevant[0])[j != relevant[0] and (j >> i) % 2] for j in mask]

    mask.sort()
    cities = ['-'] * n
    bit = 1
    i = 0
    for j in mask:
        while bit < j >> 1:
            bit <<= 1
            i += 1
        if j >> 1 == bit:
            cities[i] = ('L', 'T')[j % 2]

    print(f"Case #{t + 1}: {' '.join(cities)}")
