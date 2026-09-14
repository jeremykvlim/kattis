#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> t(q), d(q), pos(q);
    for (int i = 0; i < q; i++) {
        int s, j;
        cin >> t[i] >> d[i] >> s >> j;

        pos[i] = (s - j + n) % n;
    }

    set<int> s;
    vector<long long> time(q);
    vector<vector<int>> sweep(n);
    auto now = 0LL;
    for (int i = 0, p = 0; i < q || !s.empty();) {
        if (s.empty() && now < t[i]) now = t[i];

        for (; i < q && now >= t[i]; i++) {
            sweep[pos[i]].emplace_back(i);
            s.emplace(i);
        }

        if (!sweep[p].empty()) {
            auto end = now;
            for (int j : sweep[p]) {
                time[j] = now - t[j];
                s.erase(j);
                end = max(end, now + d[j]);
            }
            sweep[p].clear();

            for (; i < q && t[i] <= end; i++)
                if (pos[i] == p) {
                    time[i] = 0;
                    end = max(end, (long long) t[i] + d[i]);
                } else {
                    sweep[pos[i]].emplace_back(i);
                    s.emplace(i);
                }
            now = end;
        } else {
            int next = pos[*s.begin()], dist = abs(next - p);
            now += min(dist, n - dist);
            p = next;
        }
    }
    for (auto ti : time) cout << ti << "\n";
}