s, n = input(), int(input())

first, valid = {}, []

for _ in range(n):
    name = input()
    first[name[0]] = first.get(name[0], 0) + 1
    if name[0] == s[-1]: valid.append(name)

if not valid:
    print("?")
    exit(0)

for name in valid:
    if (first.get(name[-1], 0) - (1 if name[0] == name[-1] else 0)) == 0:
        print(name + "!")
        exit(0)

print(valid[0])
