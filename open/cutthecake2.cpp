#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    auto query = [&](int x1, int x2, int y1, int y2) {
        cout << "? " << x1 << " " << x2 << " " << y1 << " " << y2 << "\n" << flush;

        int v;
        cin >> v;

        return v;
    };

    auto cut = [&](int a, int b, int c, int d, auto ask) {
        auto search = [&](int l, int r, auto f) {
            while (l + 1 < r) {
                int m = l + (r - l) / 2;

                if (f(m)) l = m;
                else r = m;
            }
            return l;
        };

        if (d - c > b - a || d - c == 1) return search(a + 1, b, [&](int m) { return ask(a, m, c, d) != d - c; });

        if (a + d - c + 1 < b) {
            int v = ask(a, a + d - c + 1, c, d);
            if (!v || v == d - c + 1) return search(a + d - c + 1, b, [&](int m) {
                                          int v = ask(a, m, c, d);
                                          return !v || v == m - a;
                                      });

        }

        if (d - c == 2) return a + 1 + (!ask(a, a + 2, c, c + 1) || !ask(a, a + 2, c + 1, d));
        if (ask(a, a + d - c - 1, c, d) == d - c) return search(a + 1, a + d - c - 1, [&](int m) { return ask(a, m, c, d) != d - c; });
        return a + d - c - (ask(a + 1, a + d - c, c, d) == d - c);
    };

    vector<tuple<char, int, int, int>> cuts;
    auto dfs = [&](auto &&self, int x1, int x2, int y1, int y2) -> void {
        int v = query(x1, x2, y1, y2);
        if (!v) return;

        bool verti = false;
        int w = x2 - x1, h = y2 - y1;
        if (w != h) verti = w < h ? v == h : v != w;
        else if (w == 2) verti = query(x1, x2, y1, y1 + 1) && query(x1, x2, y1 + 1, y2);
        else verti = query(x1, x2 - 1, y1, y2) == w || query(x2 - 2, x2, y1, y2) == w;

        if (verti) {
            int x = cut(x1, x2, y1, y2, [&](int a, int b, int c, int d) { return query(a, b, c, d); });
            cuts.emplace_back('v', y1, y2, x);
            self(self, x1, x, y1, y2);
            self(self, x, x2, y1, y2);
        } else {
            int y = cut(y1, y2, x1, x2, [&](int a, int b, int c, int d) { return query(c, d, a, b); });
            cuts.emplace_back('h', x1, x2, y);
            self(self, x1, x2, y1, y);
            self(self, x1, x2, y, y2);
        }
    };
    dfs(dfs, 0, n, 0, m);

    cout << "! " << cuts.size() << "\n";
    for (auto [t, a, b, c] : cuts) cout << t << " " << a << " " << b << " " << c << "\n";
}
