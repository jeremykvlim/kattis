#include <bits/stdc++.h>
#include <tr2/dynamic_bitset>
using namespace std;
using namespace tr2;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int k_max = 0;
    vector<int> k(n);
    vector<string> name;
    vector<vector<int>> ids(n);
    unordered_map<string, int> compress;
    for (int i = 0; i < n; i++) {
        cin >> k[i];

        k_max = max(k_max, k[i]);
        ids[i].resize(k[i]);
        for (int &j : ids[i]) {
            string s;
            cin >> s;

            auto [it, inserted] = compress.emplace(s, compress.size());
            if (inserted) name.emplace_back(s);
            j = it->second;
        }
    }

    vector<vector<int>> indices(k_max + 1);
    for (int i = 0; i < n; i++) indices[k[i]].emplace_back(i);

    int m = compress.size();
    vector<dynamic_bitset<>> bs(n, dynamic_bitset<>(m));
    for (int i = 0; i < n; i++)
        for (int j : ids[i]) bs[i][j] = true;

    vector<int> degree(n, 0), next(n, 0);
    for (int s = 0; s < k_max; s++)
        for (int i : indices[s])
            for (int j : indices[s + 1])
                if (abs(k[i] - k[j]) == 1 && (bs[i] ^ bs[j]).count() == 1) {
                    if (degree[i] >= 2 || degree[j] >= 2) {
                        cout << "IMPOSSIBLE";
                        exit(0);
                    }
                    degree[i]++;
                    degree[j]++;
                    next[i] ^= j;
                    next[j] ^= i;
                }

    for (int s = 0; s <= k_max; s++)
        for (int l = 0; l < indices[s].size(); l++) {
            int i = indices[s][l];
            for (int r = l + 1; r < indices[s].size(); r++) {
                int j = indices[s][r];
                if (k[i] > 1 && k[j] > 1 && degree[i] < 2 && degree[j] < 2 && k[i] == k[j] && (bs[i] ^ bs[j]).count() == 2) {
                    degree[i]++;
                    degree[j]++;
                    next[i] ^= j;
                    next[j] ^= i;
                }
            }
        }

    vector<int> l(m, 0), r(m, 0);
    vector<bool> visited(n, false);
    for (int i = 0, pos = 1; i < n; i++) {
        if (visited[i] || k[i] != 1 || degree[i] > 1) continue;

        l[bs[i].find_first()] = pos++;
        int v = i;
        for (;;) {
            visited[v] = true;
            if (!degree[v]) break;

            int u = next[v];
            degree[u]--;
            next[u] ^= v;

            if (k[v] == k[u] - 1) l[(bs[v] ^ bs[u]).find_first()] = pos++;
            else if (k[v] == k[u]) {
                auto diff = bs[v] ^ bs[u];
                int first = diff.find_first(), second = diff.find_next(first);
                if (l[first]) r[first] = l[second] = pos++;
                else l[first] = r[second] = pos++;
            } else r[(bs[v] ^ bs[u]).find_first()] = pos++;

            v = u;
        }

        if (k[v] != 1) {
            cout << "IMPOSSIBLE";
            exit(0);
        }
        r[bs[v].find_first()] = pos++;
    }
    if (any_of(visited.begin(), visited.end(), [&](bool v) { return !v; })) {
        cout << "IMPOSSIBLE";
        exit(0);
    }

    for (int i = 0; i < m; i++) cout << name[i] << " " << l[i] << " " << r[i] << "\n";
}
