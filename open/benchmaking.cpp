#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r;
    string _;
    cin >> r >> _;

    unordered_map<string, int> indices;
    vector<string> items;
    vector<vector<pair<int, int>>> c_in;
    vector<int> c_out;
    vector<long long> need;
    auto get_id = [&](const string &s) -> int {
        if (indices.count(s)) return indices[s];

        items.emplace_back(s);
        c_in.emplace_back();
        c_out.emplace_back(0);
        need.emplace_back(0);
        return indices[s] = items.size() - 1;
    };

    while (r--) {
        int co, q;
        string so;
        cin >> co >> so >> q;

        int ido = get_id(so);
        c_out[ido] = co;

        while (q--) {
            int ci;
            string si;
            cin >> ci >> si;

            int idi = get_id(si);
            c_in[ido].emplace_back(idi, ci);
        }
        cin >> _;
    }

    int n;
    cin >> n;

    while (n--) {
        int cf;
        string sf;
        cin >> cf >> sf;

        need[get_id(sf)] += cf;
    }

    int m = items.size();
    vector<vector<int>> adj_list(m);
    for (int v = 0; v < m; v++)
        if (c_out[v])
            for (auto [u, c] : c_in[v]) adj_list[u].emplace_back(v);

    vector<int> order;
    vector<bool> visited(m, false);
    auto dfs = [&](auto &&self, int v) -> void {
        visited[v] = true;
        for (int u : adj_list[v])
            if (!visited[u]) self(self, u);
        order.emplace_back(v);
    };
    for (int i = 0; i < m; i++)
        if (!visited[i]) dfs(dfs, i);

    for (int v : order)
        if (need[v] && c_out[v]) {
            auto times = (need[v] + c_out[v] - 1) / c_out[v];
            for (auto [u, c] : c_in[v]) need[u] += times * c;
        }

    for (int i = 0; i < m; i++)
        if (need[i] && !c_out[i]) cout << need[i] << " " << items[i] << "\n";
}
