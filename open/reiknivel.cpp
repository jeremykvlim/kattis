#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, x;
    cin >> a >> x;

    if (!x) {
        cout << 0;
        exit(0);
    }

    vector<tuple<char, int, int>> ops;
    while (a--) {
        char t;
        int y, c;
        cin >> t >> y >> c;

        if (!(((t == '+' || t == '-') && !y) || ((t == '*' || t == '/') && y == 1))) ops.emplace_back(t, y, c);
    }
    sort(ops.begin(), ops.end());
    ops.erase(unique(ops.begin(), ops.end(), [&](auto op1, auto op2) { return get<0>(op1) == get<0>(op2) &&
                                                                              get<1>(op1) == get<1>(op2); }), ops.end());

    vector<int> dist(1e8, 1e9);
    dist[0] = 0;
    vector<queue<int>> buckets(4);
    buckets[0].emplace(0);
    for (int queued = 1, b = 0, dv = 0; queued;) {
        while (buckets[b].empty()) {
            b = (b + 1) % 4;
            dv++;
        }

        int v = buckets[b].front();
        buckets[b].pop();
        queued--;

        if (dv != dist[v]) continue;

        if (v == x) {
            cout << dv;
            exit(0);
        }

        for (auto [t, y, c] : ops) {
            long long u;
            if (t == '+') u = v + y;
            else if (t == '-') u = v - y;
            else if (t == '*') u = (long long) v * y;
            else u = v / y;

            if (!(0 <= u && u < 1e8)) continue;

            if (dist[u] > dv + c) {
                dist[u] = dv + c;
                buckets[(b + c) % 4].emplace(u);
                queued++;
            }
        }
    }

    cout << "Engin leid!";
}
