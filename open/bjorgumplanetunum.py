x = int(input()) * 400

def win(k):
    n = 4 * k * k + 448 * k + 1590
    return n * n >= x

l, r = -1, 1
while not win(r): r *= 2

while l + 1 < r:
    m = l + (r - l) // 2
    if win(m): r = m
    else: l = m

print(r)
