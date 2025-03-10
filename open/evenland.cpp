#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U, typename V>
T mul(U x, V y, T mod) {
    return (unsigned __int128) x * y % mod;
}

template <typename T, typename U>
T pow(T base, U exponent, T mod) {
    T value = 1;
    while (exponent) {
        if (exponent & 1) value = mul(base, value, mod);
        base = mul(base, base, mod);
        exponent >>= 1;
    }
    return value;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    int exponent = m - n;
    vector<vector<int>> adj_list(n);
    while (m--) {
        int a, b;
        cin >> a >> b;

        adj_list[a - 1].emplace_back(b - 1);
        adj_list[b - 1].emplace_back(a - 1);
    }

    vector<bool> visited(n, false);
    auto dfs = [&](auto &&self, int v) -> void {
        visited[v] = true;
        for (int u : adj_list[v])
            if (!visited[u]) self(self, u);
    };

    for (int v = 0; v < n; v++)
        if (!visited[v]) {
            exponent++;
            dfs(dfs, v);
        }

    cout << pow(2, exponent, (int) 1e9 + 9);
}
