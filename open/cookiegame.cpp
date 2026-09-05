#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int &ai : a) {
        cin >> ai;
        ai++;
    }

    vector<long long> pref(n + 1, 0), prev(n + 1, 1e18), curr(n + 1, 0);
    for (int i = 0; i < n; i++) pref[i + 1] = pref[i] + a[i];
    prev[0] = curr[0] = -1e18;

    for (int cookies = n; ~cookies; cookies--) {
        auto sum = [&](int c) {
            return pref[cookies] - pref[cookies - c];
        };

        deque<int> mono{0};
        auto extra = 0LL;
        for (int c = 1; c <= cookies; c++) {
            int l = -1, r = mono.size(), m, i = cookies - c;

            while (l + 1 < r) {
                m = l + (r - l) / 2;

                if (-curr[mono[m]] < a[i]) l = m;
                else r = m;
            }

            int j = mono[r];
            auto s = -sum(j);
            if (~l) {
                j = mono[l];
                s = min(s, curr[j] - sum(j) + a[i]);
            }

            curr[c] = min(s + extra, prev[c] - a[i]);
            extra += a[i];
            while (curr[c] - extra <= curr[mono.front()] - sum(mono.front())) mono.pop_front();
            mono.emplace_front(c);
        }

        if (curr[cookies] <= 0) {
            cout << cookies;
            exit(0);
        }

        swap(prev, curr);
    }
}
