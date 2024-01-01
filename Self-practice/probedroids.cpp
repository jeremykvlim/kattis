#include <bits/stdc++.h>
using namespace std;

long long destroyed(long long n, long long m, long long mid) {
    if (mid < n) return (__gcd(m - 1, mid - 1) + m * mid - 1) / 2;
    else if (mid < n + m) return (__gcd(n - 1, n + m - mid - 1) + mid * n - (n - m) * n - 1) / 2;
    else return n * m;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, m;
    int q;
    cin >> n >> m >> q;

    vector<pair<long long, long long>> drdc(max(n, m));
    while (q--) {
        long long i;
        cin >> i;

        if (i < m) cout << "1 " << i + 1 << "\n";
        else if (i > n * (m - 1)) cout << i - n * (m - 1) + 1 << " 1\n";
        else {
            long long l = 0, r = INT_MAX, mid;
            while (l + 1 < r) {
                mid = l + (r - l) / 2;
                if (destroyed(n, m, mid) >= i) r = mid;
                else l = mid;
            }
            i -= destroyed(n, m, l);

            auto curr = make_pair(0LL, 1LL), remaining = (l < n) ? make_pair(l - 1, m - 1) : make_pair(n - 1, m - 1 + n - l);
            while (curr.first + 1 < n && curr.second < m && (curr.first + 1) * remaining.second <= curr.second * remaining.first) curr.first++;

            drdc[0] = {1, 0};
            int pos = 1;
            for (;;) {
                auto next = make_pair(curr.first + drdc[pos - 1].first, curr.second + drdc[pos - 1].second);
                if (next.first >= n || next.second >= m) {
                    if (curr.first * remaining.second > curr.second * remaining.first) {
                        auto destroy = min((n - 1) / curr.first, (m - 1) / curr.second);
                        if (destroy > i - 1) {
                            cout << curr.first * i + 1 << " " << curr.second * i + 1 << "\n";
                            break;
                        }

                        i -= destroy;
                    }
                    curr = drdc[--pos];
                } else if (next.first * remaining.second > next.second * remaining.first) drdc[pos++] = next;
                else curr = next;
            }
        }
    }
}
