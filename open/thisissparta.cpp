#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long k;
    cin >> n >> k;
    k--;

    deque<long long> v(n);
    for (auto &vi : v) cin >> vi;

    while (v.size() > 3 && k) {
        sort(v.begin(), v.end());
        while (!v.empty() && !v.front()) v.pop_front();
        for (int i = 1; i < v.size(); i++) v[i] -= v[i - 1];
        k--;
    }

    if (v.size() == 3) {
        sort(v.begin(), v.end());
        while (v[0] && k) {
            long long l = -1, r = v[1] / v[0] + 1, m;
            while (l + 1 < r) {
                m = l + (r - l) / 2;

                if (v[1] - (__int128) m * v[0] <= v[2] - (__int128) m * v[1] + (__int128) v[0] * m * (m + 1) / 2) l = m;
                else r = m;
            }

            r = min(r, k);
            if (r == v[1] / v[0] + 1) {
                v[2] -= l * v[1] - v[0] * l * (l + 1) / 2;
                v[1] -= l * v[0];
                k -= l;
            } else {
                v[2] -= r * v[1] - v[0] * r * (r + 1) / 2;
                v[1] -= r * v[0];
                k -= r;
            }
            sort(v.begin(), v.end());
        }
    }
    while (!v.empty() && !v.front()) v.pop_front();

    if (v.size() == 2) {
        sort(v.begin(), v.end());
        while (v[0] && k) {
            auto r = v[1] / v[0];
            r = min(r, k);
            v[1] -= r * v[0];
            k -= r;
            sort(v.begin(), v.end());
        }
    }

    sort(v.begin(), v.end());
    for (int i = 1; i < v.size(); i++) v[i] -= v[i - 1];
    for (int i = 0; i < n - v.size(); i++) cout << "0 ";
    for (auto vi : v) cout << vi << " ";
}
