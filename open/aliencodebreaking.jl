let
    MOD = 1 << 20
    
    (n, x) = parse.(Int, split(readline()))
    m = readline()
    
    f = Vector{Int}(undef, MOD)
    f[1] = 1
    for i = 2:MOD
      f[i] = (33 * f[i - 1] + 1) % MOD
    end
    
    sum = j = 0
    for _ = 1:x
      sum = (sum + f[j + 1]) % MOD
      j = (j + x) % MOD
    end
    
    parts = Vector{String}(undef, x)
    for i in 1:x
        parts[i] = string(sum)
        sum = (33 * sum + x) % MOD
    end
    digits27 = string(parse(BigInt, join(parts)); base = 27)
    
    text = Vector{Char}(undef, n)
    for i = 1:n
      r = ((m[i] == ' ' ? 26 : m[i] - 'A') + (digits27[i] in '0':'9' ? digits27[i] - '0' : digits27[i] - 'a' + 10)) % 27
      text[i] = r == 26 ? ' ' : 'A' + r
    end
    print(join(text))
end
