#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int h, w;
    cin >> h >> w;

    vector<string> grid(h);
    for (auto &row : grid) cin >> row;

    int n = h * w;
    vector<int> up(n), down(n), left(n), right(n);
    vector<bool> removed(n, false), queued(n, false);
    for (int r = 0; r < h; r++)
        for (int c = 0; c < w; c++) {
            int v = r * w + c;
            up[v] = r ? v - w : -1;
            down[v] = r + 1 < h ? v + w : -1;
            left[v] = c ? v - 1 : -1;
            right[v] = c + 1 < w ? v + 1 : -1;
        }

    queue<int> q;
    auto add = [&](int v) {
        if (v != -1 && !removed[v] && !queued[v]) {
            char c = grid[v / w][v % w];
            if (c == '^' && !~up[v] ||
                c == 'v' && !~down[v] ||
                c == '<' && !~left[v] ||
                c == '>' && !~right[v]) {
                queued[v] = true;
                q.emplace(v);
            }
        }
    };
    for (int v = 0; v < n; v++) add(v);

    vector<int> order;
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        if (removed[v]) continue;

        removed[v] = true;
        order.emplace_back(v);
        int l = left[v], r = right[v], u = up[v], d = down[v];
        if (~d) up[d] = u;
        if (~u) down[u] = d;
        if (~r) left[r] = l;
        if (~l) right[l] = r;
        add(u);
        add(d);
        add(l);
        add(r);
    }

    if (order.size() != n) cout << "impossible";
    else
        for (int v : order) cout << v / w + 1 << " " << v % w + 1 << "\n";
}
