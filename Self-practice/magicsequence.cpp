#include<bits/stdc++.h>
using namespace std;

void radix_sort(vector<long long>& s) {
    long long radix = 1 << 16, biggest = *max_element(s.begin(), s.end());
    auto max_d = (long long)((log(biggest)) / (log(radix)) + 1.5);

    vector<int> bucket(radix, 0), v(s.size());

    for (long long d = 0; d < max_d; d++) {
        fill(bucket.begin(), bucket.end(), 0);
        for (long long j : s) bucket[(j >> (d * 16)) & (radix - 1)]++;
        for (int j = 1; j < radix; j++) bucket[j] += bucket[j - 1];
        for (int j = v.size() - 1; j >= 0; j--) v[--bucket[(s[j] >> (d * 16)) & (radix - 1)]] = s[j];
        copy(v.begin(), v.end(), s.begin());
    }

}

int main() {
    ios_base::sync_with_stdio(false);
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
        for (int i = 0; i < n; i++) v = (v*x + s[i]) % y;
        cout << v << "\n";
    }
}
