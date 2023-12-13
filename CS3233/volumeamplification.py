for _ in range(int(input())):
    _, y = map(int, input().split())
    levels = {1}
    for a in map(int, input().split()):
        levels.update({v * a for v in levels if v * a <= y})
    print(max(levels))
