import sys

class PersistentSegmentTree:
    def __init__(self, n):
        self.n = n
        self.roots = [0]
        self.ST = [0]
        self.children = [(0, 0)]
        self.size = [1]

    def modify(self, i, v, pos):
        self.roots.append(self.__modify(self.roots[i], v, pos, 1, self.size[i] + 1))
        self.size.append(self.size[i])
        return len(self.roots) - 1

    def __modify(self, i, v, pos, tl, tr):
        if tl + 1 == tr:
            self.children.append((0, 0))
            self.ST.append(v)
            return len(self.ST) - 1

        cl, cr = self.children[i]
        tm = tl + (tr - tl) // 2
        if pos < tm:
            cl = self.__modify(cl, v, pos, tl, tm)
        else:
            cr = self.__modify(cr, v, pos, tm, tr)

        self.children.append((cl, cr))
        self.ST.append(self.ST[cl] + self.ST[cr])
        return len(self.ST) - 1

    def range_query(self, i, l, r):
        return self.__range_query(self.roots[i], l, r, 1, self.size[i] + 1)

    def __range_query(self, i, l, r, tl, tr):
        if not i or r <= tl or tr <= l: return 0
        if l <= tl and tr <= r: return self.ST[i]

        cl, cr = self.children[i]
        tm = tl + (tr - tl) // 2
        return self.__range_query(cl, l, r, tl, tm) + self.__range_query(cr, l, r, tm, tr)

    def join(self, ver_a, ver_b):
        root_a = self.roots[ver_a]
        root_b = self.roots[ver_b]
        new_root = len(self.ST)
        self.children.append((root_a, root_b))
        self.ST.append(self.ST[root_a] + self.ST[root_b])
        self.roots.append(new_root)
        self.size.append(self.size[ver_a] + self.size[ver_b])
        return len(self.roots) - 1


MOD = 10 ** 9 + 7

P, q, m = map(int, sys.stdin.readline().split())
ops: list[tuple[int, ...]] = [None] * (P + 1)
songs = [0] * (P + 1)
songs[0] = 1
for i in range(1, P + 1):
    lines = sys.stdin.readline().split()
    if lines[0] == "copy":
        a, b = map(int, lines[1:3])
        ops[i] = (a, b)
        songs[i] = songs[a] + songs[b]
    else:
        a, l, t = map(int, lines[1:4])
        ops[i] = (a, l, t)
        songs[i] = songs[a]

pst = PersistentSegmentTree(max(songs) + 1)
version = [0] * (P + 1)
version[0] = pst.modify(0, m, 1)

for i in range(1, P + 1):
    if len(ops[i]) == 2:
        a, b = ops[i]
        version[i] = pst.join(version[a], version[b])
    else:
        a, l, t = ops[i]
        version[i] = pst.modify(version[a], t, l + 1)

for _ in range(q):
    p, a, b = map(int, sys.stdin.readline().split())
    print(pst.range_query(version[p], a + 1, b + 2) % MOD)
