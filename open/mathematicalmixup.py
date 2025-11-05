import sys

t = 1
for line in sys.stdin.read().splitlines():
    if line.strip() == '0': exit(0)
    lhs, rhs = line.split('=', 1)

    i, n = 0, len(rhs)
    def parse(bracket=False):
        global i
        block = []
        while i < n:
            c = rhs[i]
            if c.isspace():
                i += 1
            elif c == '(':
                i += 1
                block.append(parse(True))
            elif c == ')':
                i += 1
                break
            elif c.isdigit():
                j = i
                while j < n and rhs[j].isdigit(): j += 1
                block.append(rhs[i:j])
                i = j
            else:
                i += 1
        return bracket, tuple(block)

    def generate(root):
        if isinstance(root, str): return [root]
        bracket, block = root
        if not block: return []

        all = []
        def dfs(k=0, e=''):
            if k == len(block):
                all.append('(' + e + ')' if bracket and len(block) > 1 else e)
                return
            if k == 0:
                for f in generate(block[0]): dfs(1, f)
            else:
                for f in generate(block[k]):
                    for op in ['+', '-', '*']: dfs(k + 1, e + op + f)
        dfs()
        return all

    l, r = int(lhs.strip()), None
    for e in generate(parse()):
        if eval(e) == l:
            r = e
            break
    print(f'Equation #{t}:\n{"Impossible." if r is None else f"{l}={r}"}\n')
    t += 1
