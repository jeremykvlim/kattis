from math import gcd


def build():
    for i in range(n - 1, 0, -1):
        segment_tree[i] = gcd(segment_tree[i << 1], segment_tree[i << 1 | 1])


def update(i, val):
    i += n
    segment_tree[i] = val
    while i > 1:
        segment_tree[i >> 1] = gcd(segment_tree[i], segment_tree[i ^ 1])
        i >>= 1


def query(l, r):
    res = 0
    l += n
    r += n
    while l < r:
        if l & 1:
            res = gcd(res, segment_tree[l])
            l += 1
        if r & 1:
            r -= 1
            res = gcd(res, segment_tree[r])
        l >>= 1
        r >>= 1
    return res


n, q = map(int, input().split())
turns = list(map(int, input().split()))
segment_tree = [0] * (n << 1)
for i in range(n):
    segment_tree[i + n] = turns[i]
build()
for _ in range(q):
    x, y, z = map(int, input().split())
    if x == 1:
        update(y - 1, z)
    elif x == 2:
        print(query(y - 1, z))
