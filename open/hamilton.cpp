#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    auto query = [&](int i, int j) {
        cout << "? " << i << " " << j << "\n" << flush;

        int d;
        cin >> d;

        return d;
    };

    vector<int> order(n);
    iota(order.begin(), order.end(), 1);
    shuffle(order.begin(), order.end(), mt19937(random_device{}()));
    auto dfs = [&](auto &&self, const vector<int> &order) -> vector<int> {
        int m = order.size();
        if (m < 2) return order;

        vector<int> match(n + 1, -1), path;
        for (int i = 0; i + 1 < m; i += 2) {
            int u = order[i], v = order[i + 1];
            if (query(u, v)) swap(u, v);

            path.emplace_back(u);
            match[u] = v;
            match[v] = u;
        }
        path = self(self, path);

        vector<int> temp;
        for (int v : path) temp.emplace_back(match[v]);
        if (m & 1) temp.emplace_back(order.back());
        path.insert(path.begin(), temp[0]);

        for (int i = 1, x = 1, y = 3; i < temp.size(); x = exchange(y, y + 2 * x)) {
            int j = min((int) temp.size(), y);
            for (int k = j - 1; k >= i; k--) {
                int l = -1, r = !~match[temp[k]] ? path.size() : find(path.begin(), path.end(), match[temp[k]]) - path.begin(), mid;
                while (l + 1 < r) {
                    mid = l + (r - l) / 2;

                    if (query(path[mid], temp[k])) l = mid;
                    else r = mid;
                }
                path.insert(path.begin() + r, temp[k]);
            }
            i = j;
        }
        return path;
    };
    order = dfs(dfs, order);

    cout << "! ";
    for (int p : order) cout << p << " ";
}
