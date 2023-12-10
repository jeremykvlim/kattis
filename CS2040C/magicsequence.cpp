#include <bits/stdc++.h>
using namespace std;

void radix_sort(vector<long long> &s) {
    long long radix = 1 << 16, biggest = *max_element(s.begin(), s.end());
    auto msd = (long long) ceil((log(biggest)) / (log(radix)));

    vector<int> bucket(radix, 0), v(s.size());

    for (long long d = 0; d < msd; d++) {
        fill(bucket.begin(), bucket.end(), 0);
        for (long long i : s) bucket[(i >> (d * 16)) & (radix - 1)]++;
        for (int i = 1; i < radix; i++) bucket[i] += bucket[i - 1];
        for (int i = v.size() - 1; i >= 0; i--) v[--bucket[(s[i] >> (d * 16)) & (radix - 1)]] = s[i];
        copy(v.begin(), v.end(), s.begin());
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tc;
    cin >> tc;
    
    while (tc--) {
        long long n, a, b, c, v = 0, x, y;
        cin >> n >> a >> b >> c >> x >> y;
        
        vector<long long> s(n);
        s[0] = a;
        for (int i = 1; i < n; i++) s[i] = (s[i - 1]*b + a) % c;
        radix_sort(s);
        for (int i = 0; i < n; i++) v = (v * x + s[i]) % y;
        cout << v << "\n";
    }
}
