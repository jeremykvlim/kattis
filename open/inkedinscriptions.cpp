#include <bits/stdc++.h>
using namespace std;

template <typename... I>
auto hilbert_index(I... c) {
    using T = common_type_t<I...>;
    constexpr int D = sizeof...(I);
    array<T, D> coords{(T) c...};
    T c_max = max({c...});
    int b = 0;
    if (c_max)
        while (c_max >>= 1) b++;

    for (T mask = ((T) 1) << b; mask > 1; mask >>= 1)
        for (int i = D - 1; ~i; i--)
            if (coords[i] & mask) coords[0] ^= mask - 1;
            else {
                T m = (coords[0] ^ coords[i]) & (mask - 1);
                coords[0] ^= m;
                coords[i] ^= m;
            }

    for (int i = 1; i < D; i++) coords[i] ^= coords[i - 1];
    T m = 0;
    for (T mask = ((T) 1) << b; mask > 1; mask >>= 1)
        if (coords[D - 1] & mask) m ^= mask - 1;
    for (int i = 0; i < D; i++) coords[i] ^= m;

    T h = 0;
    for (; ~b; b--)
        for (int i = 0; i < D; i++) h = (h << 1) | ((coords[i] >> b) & 1);
    return h;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> p(n);
    for (int &pi : p) {
        cin >> pi;
        pi--;
    }

    int m = bit_ceil((unsigned) n);
    vector<int> hilbert_order(n);
    for (int i = 0; i < n; i++) hilbert_order[i] = hilbert_index(i + m, p[i] + m);

    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) { return hilbert_order[i] < hilbert_order[j]; });
    for (int i : order) cout << i + 1 << " " << p[i] + 1 << "\n";
}
