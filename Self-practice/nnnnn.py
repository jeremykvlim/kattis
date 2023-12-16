L = list(map(int, input().strip()))
length, remaining = -1, 0
for i, digit in enumerate(L):
    remaining = 10 * remaining + digit
    if length == -1 and remaining >= len(L) - i:
        length = len(L) - i
    if length > 0:
        L[i] = remaining // length
        remaining %= length
print(0 if L == [0] else ''.join(map(str, L[-length:])))
