#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (!n) {
        cout << 0;
        exit(0);
    }

    auto sum_t = 0LL;
    vector<array<int, 3>> assignments(n);
    for (auto &[a, b, t] : assignments) {
        cin >> a >> b >> t;
        b++;

        sum_t += t;
    }
    sort(assignments.begin(), assignments.end());

    auto valid = [&](auto time) {
        vector<int> t(n);
        for (int i = 0; i < n; i++) t[i] = assignments[i][2];

        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
        for (int i = 0, l; i < n || !pq.empty();) {
            if (pq.empty()) l = assignments[i][0];
            for (; i < n && assignments[i][0] == l; i++)
                if (t[i]) pq.emplace(assignments[i][1], i);

            auto r = min(i < n ? assignments[i][0] : LLONG_MAX, !pq.empty() ? pq.top().first : LLONG_MAX);
            if (l < r && !pq.empty()) {
                auto limit = (__int128) (r - l) * time;
                while (limit && !pq.empty()) {
                    auto [d, j] = pq.top();
                    pq.pop();

                    auto used = min((__int128) t[j], limit);
                    t[j] -= used;
                    limit -= used;
                    if (t[j]) {
                        pq.emplace(d, j);
                        break;
                    }
                }
            }
            l = r;
            if (!pq.empty() && pq.top().first <= l) return false;
        }
        return true;
    };

    long long l = 0LL, r = sum_t, mid;
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        if (valid(mid)) r = mid;
        else l = mid;
    }
    cout << r;
}
