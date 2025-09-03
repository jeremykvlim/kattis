let
  MODULO = 1 << 20

  (n, x) = parse.(Int, split(readline()))
  m = readline()

  f = Vector{Int}(undef, MODULO)
  f[1] = 1
  for i = 2:MODULO
    f[i] = (33 * f[i - 1] + 1) % MODULO
  end

  s = j = 0
  for _ = 1:x
    s = (s + f[j + 1]) % MODULO
    j = (j + x) % MODULO
  end

  buf = IOBuffer()
  for _ = 1:x
    print(buf, s)
    s = (33 * s + x) % MODULO
  end
  digits27 = string(parse(BigInt, String(take!(buf))), base = 27)

  text = Vector{Char}(undef, n)
  for i = 1:n
    d = digits27[i]
    r = ((m[i] == ' ' ? 26 : m[i] - 'A') + (d in '0':'9' ? d - '0' : d - 'a' + 10)) % 27
    text[i] = r == 26 ? ' ' : 'A' + r
  end
  print(join(text))
end
