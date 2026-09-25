let
    line, text = eachline()
    x = parse(Int, split(line)[2])
    s = sum(i -> 33^i >> 5, 0:x:x^2 - 1)
    digits27 = digits(parse(BigInt, join(s = mod(33s + x, 2^20) for _ = 1:x)), base = 27)
    print(map(text) do l
        cipher = Int(l) & 31
        shift = pop!(digits27)
        c = (cipher + shift) % 27
        c == 0 ? ' ' : c + '@'
    end)
end
