n, p = map(int, input().split())
cards = [list(map(int, input().split())) for _ in range(n)]
sequences = [[False]*13 for _ in range(4)]

for i, (suit, value) in enumerate(cards):
    suit -= 1
    value -= 1
    sequences[suit][value] = True
    if (value <= 10 and sequences[suit][value + 1] and sequences[suit][value + 2]) or \
       (1 <= value <= 11 and sequences[suit][value - 1] and sequences[suit][value + 1]) or \
       (value >= 2 and sequences[suit][value - 2] and sequences[suit][value - 1]):
        print(max(0, i - p) + 1)
        break
else:
    print('Neibb')
