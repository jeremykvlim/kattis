#include <bits/stdc++.h>
using namespace std;

template <typename I>
void radix_sort(I begin, I end) {
    if (begin == end) return;

    using T = typename iterator_traits<I>::value_type;
    T biggest = *max_element(begin, end);
    int radix = 1 << 16;

    vector<int> count(radix);
    vector<T> temp(distance(begin, end));
    for (int i = 0; i < (__lg(biggest) / __lg(radix)) + 1; i++) {
        fill(count.begin(), count.end(), 0);
        for (auto it = begin; it < end; it++) count[(*it >> (i * 16)) & (radix - 1)]++;
        for (int j = 1; j < radix; j++) count[j] += count[j - 1];
        for (auto it = end - 1; it >= begin; it--) temp[--count[(*it >> (i * 16)) & (radix - 1)]] = *it;
        copy(temp.begin(), temp.end(), begin);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tc;
    cin >> tc;

    while (tc--) {
        long long n, a, b, c, x, y;
        cin >> n >> a >> b >> c >> x >> y;

        vector<long long> s(n);
        s[0] = a;
        for (int i = 1; i < n; i++) s[i] = (s[i - 1] * b + a) % c;
        radix_sort(s.begin(), s.end());

        auto v = 0LL;
        for (auto r : s) v = (v * x + r) % y;
        cout << v << "\n";
    }
}
