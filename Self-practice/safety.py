for x in range(int(input())):
    n, *p = map(int, input().split())
    curr, prev = 2 * sum(p) / n, 201
    while curr < prev:
        prev, curr = curr, (sum(p) + sum(s for s in p if s <= curr)) / sum(1 for s in p if s <= curr)
    print(f'Case #{x + 1}:', *("{:.5f}".format(max((curr - s) / sum(p) * 100, 0)) for s in p))
