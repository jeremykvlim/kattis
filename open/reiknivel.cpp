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

    vector<vector<char>> charge(4, vector<char>(10, 4));
    while (a--) {
        char op;
        int y, c;
        cin >> op >> y >> c;

        int t = (op == '/') ? 0 : ((op == '*') ? 1 : ((op == '+') ? 2 : 3));
        if (!((t > 1 && !y) || (t < 2 && y == 1))) charge[t][y] = min(charge[t][y], (char) c);
    }

    vector<tuple<char, int, char>> ops;
    for (int t = 0; t <= 3; t++)
        for (int y = 0; y <= 9; y++)
            if (charge[t][y] < 4) ops.emplace_back(t, y * (t == 3 ? -1 : 1), charge[t][y]);

    if (ops.empty()) {
        cout << "Engin leid!";
        exit(0);
    }

    vector<char> dist(1e8, -1);
    dist[0] = 0;
    array<queue<unsigned>, 4> buckets;
    buckets[0].emplace(0);
    for (int d = 0, b = 0;;) {
        while (buckets[b].empty()) {
            if (all_of(buckets.begin(), buckets.end(), [&](const auto &b) { return b.empty(); })) {
                cout << "Engin leid!";
                exit(0);
            }
            d++;
            ++b &= 3;
        }

        auto v = buckets[b].front();
        buckets[b].pop();

        if (dist[v] == -2 || dist[v] != b) continue;
        dist[v] = -2;

        if (v == x) {
            cout << d;
            exit(0);
        }

        for (auto [t, y, c] : ops) {
            auto u = v;
            if (!t) {
                switch (y) {
                    case 2:
                        u /= 2;
                        break;
                    case 3:
                        u /= 3;
                        break;
                    case 4:
                        u /= 4;
                        break;
                    case 5:
                        u /= 5;
                        break;
                    case 6:
                        u /= 6;
                        break;
                    case 7:
                        u /= 7;
                        break;
                    case 8:
                        u /= 8;
                        break;
                    case 9:
                        u /= 9;
                        break;
                }
            } else if (t == 1) u *= y;
            else u += y;

            if (u < 1e8 && dist[u] != -2)
                if (dist[u] == -1 || ((dist[u] - b) & 3) > c) buckets[dist[u] = (b + c) & 3].emplace(u);
        }
    }
}
