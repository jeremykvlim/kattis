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

    int c_max = 0;
    vector<vector<int>> charge(4, vector<int>(10, 1e9));
    unordered_map<char, int> ctoi{{'+', 0}, {'-', 1}, {'*', 2}, {'/', 3}};
    unordered_map<int, char> itoc{{0, '+'}, {1, '-'}, {2, '*'}, {3, '/'}};
    while (a--) {
        char t;
        int y, c;
        cin >> t >> y >> c;

        if (!(((t == '+' || t == '-') && !y) || ((t == '*' || t == '/') && y == 1))) charge[ctoi[t]][y] = min(charge[ctoi[t]][y], c);
    }

    vector<tuple<char, int, int>> ops;
    for (int y = 0; y <= 9; y++)
        for (int i = 0; i <= 3; i++)
            if (charge[i][y] < 1e9) {
                ops.emplace_back(itoc[i], y, charge[i][y]);
                c_max = max(c_max, charge[i][y]);
            }

    if (ops.empty()) {
        cout << "Engin leid!";
        exit(0);
    }

    vector<int> dist(1e8, 1e9);
    dist[0] = 0;
    array<deque<int>, 4> buckets;
    buckets[0].emplace_back(0);
    for (int dv = 0;;) {
        while (buckets[0].empty()) {
            if (all_of(buckets.begin(), buckets.end(), [&](const auto &b) { return b.empty(); })) {
                cout << "Engin leid!";
                exit(0);
            }
            dv++;
            for (int i = 0; i + 1 < 4; i++) buckets[i].swap(buckets[i + 1]);
        }

        int v = buckets[0].front();
        buckets[0].pop_front();

        if (dv != dist[v]) continue;

        if (v == x) {
            cout << dv;
            exit(0);
        }

        for (auto [t, y, c] : ops) {
            int u;
            if (t == '+') u = v + y;
            else if (t == '-') u = v - y;
            else if (t == '/') u = v / y;
            else {
                if (__builtin_mul_overflow(v, y, &u)) u = -1;
                else u = v * y;
            }

            if (!(0 <= u && u < 1e8)) continue;

            if (dist[u] > dv + c) {
                dist[u] = dv + c;
                buckets[c].emplace_back(u);
            }
        }
    }
}
