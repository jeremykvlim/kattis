#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int total = 2 * n;
    vector<int> count(total, 0);
    vector<pair<int, int>> pins(n);
    for (auto &[a, b] : pins) {
        cin >> a >> b;

        count[(a + b) % total]++;
    }

    int most = -1, sum = -1;
    for (int s = 1; s < total; s += 2)
        if (most < count[s]) {
            most = count[s];
            sum = s;
        }

    if (n == most) {
        cout << 0;
        exit(0);
    }

    cout << n - most << "\n";
    vector<int> as, id(total, -1);
    for (int a = 0, i = 0; a < total; a++) {
        int b = sum - a;
        if (b < 0) b += total;
        if (a < b) {
            as.emplace_back(a);
            id[a] = i++;
        }
    }

    vector<int> match(total);
    for (int a = 0; a < total; a++) {
        int b = sum - a;
        if (b < 0) b += total;
        match[a] = id[min(a, b)];
    }

    vector<bool> same(n, false);
    for (int i = 0; i < n; i++) {
        auto [a, b] = pins[i];
        same[i] = ((a + b) % total) == sum;
    }

    vector<array<int, 2>> edges(n);
    vector<vector<int>> indices(n);
    for (int i = 0; i < n; i++)
        if (!same[i]) {
            auto [a, b] = pins[i];
            int u = match[a], v = match[b];
            edges[i] = {u, v};
            indices[u].emplace_back(i);
            indices[v].emplace_back(i);
        }

    vector<int> order(n, -1);
    vector<bool> visited(n, false);
    for (int i = 0; i < n; i++) {
        if (same[i] || visited[i]) continue;
        for (int p = i, prev = -1; !visited[p];) {
            visited[p] = true;
            auto [u, v] = edges[p];
            int curr = (u != prev ? u : v);
            p = (p == indices[curr][0] ? indices[curr][1] : indices[curr][0]);
            order[p] = prev = curr;
        }
    }

    for (int p = 0; p < n; p++) {
        if (same[p]) continue;

        int a0 = as[order[p]], b0 = sum - a0;
        if (b0 < 0) b0 += total;

        auto [a1, b1] = pins[p];
        int r = a1, s = b1;
        if (a1 != a0 && a1 != b0) swap(r, s);
        cout << p << " " << s << " " << (a0 + b0 - r) % total << "\n";
    }
}
