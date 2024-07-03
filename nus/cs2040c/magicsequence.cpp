#include <bits/stdc++.h>
using namespace std;

void radix_sort(vector<long long> &s) {
    auto biggest = *max_element(s.begin(), s.end());
    int radix = 1 << 16, msd = ceil((log(biggest)) / (log(radix)));

    vector<int> count(radix);
    vector<long long> temp(s.size());
    for (int d = 0; d < msd; d++) {
        fill(count.begin(), count.end(), 0);
        for (auto i : s) count[(i >> (d * 16)) & (radix - 1)]++;
        for (int i = 1; i < radix; i++) count[i] += count[i - 1];
        for (int i = temp.size() - 1; ~i; i--) temp[--count[(s[i] >> (d * 16)) & (radix - 1)]] = s[i];
        copy(temp.begin(), temp.end(), s.begin());
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
        radix_sort(s);

        auto v = 0LL;
        for (auto r : s) v = (v * x + r) % y;
        cout << v << "\n";
    }
}
