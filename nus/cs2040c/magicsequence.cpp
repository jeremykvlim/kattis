#include <bits/stdc++.h>
using namespace std;

template <typename I>
void radix_sort(I l, I r) {
    if (l == r) return;

    using T = typename iterator_traits<I>::value_type;
    int radix = 1 << 16;

    vector<int> count(radix);
    vector<T> temp(distance(l, r));
    for (int i = 0; i <= (__lg(*max_element(l, r)) / __lg(radix)); i++) {
        fill(count.begin(), count.end(), 0);
        for (auto it = l; it < r; it++) count[(*it >> (i * 16)) & (radix - 1)]++;
        for (int j = 1; j < radix; j++) count[j] += count[j - 1];
        for (auto it = r - 1; it >= l; it--) temp[--count[(*it >> (i * 16)) & (radix - 1)]] = *it;
        copy(temp.begin(), temp.end(), l);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tc;
    cin >> tc;

    vector<long long> s;
    while (tc--) {
        int n, a, b, c, x, y;
        cin >> n >> a >> b >> c >> x >> y;

        s.resize(n);
        s[0] = a;
        for (int i = 1; i < n; i++) s[i] = (s[i - 1] * b + a) % c;
        radix_sort(s.begin(), s.end());

        int v = 0;
        for (auto r : s) v = ((long long) v * x + r) % y;
        cout << v << "\n";
    }
}
