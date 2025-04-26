#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> p(n + 1);
    vector<vector<int>> adj_list(n + 1);
    vector<bool> giant(n + 1);
    unordered_map<string, vector<int>> berry, bird;
    for (int i = 1; i <= n; i++) {
        char t;
        cin >> p[i] >> t;

        if (t == 'G' || t == 'B') giant[i] = true;
        if (t != 'B' && t != 'S') {
            string L;
            cin >> L;

            if (t == 'E') berry[L].emplace_back(i);
            else bird[L].emplace_back(i);
        }
        adj_list[p[i]].emplace_back(i);
    }

    vector<int> order(n + 1), in(n + 1), out(n + 1), depth(n + 1, 0), leaf_branch(n + 1), leaf_depth(n + 1);
    int count = 1;
    auto dfs = [&](auto &&self, int v = 1, int branch = 1) -> void {
        order[count] = v;
        in[v] = count++;

        if (adj_list[v].empty()) {
            leaf_branch[v] = branch;

            if (giant[v]) leaf_depth[v] = depth[branch];
            else leaf_depth[v] = depth[p[v]];
        } else {
            if (giant[v]) branch = v;
            for (int u : adj_list[v]) {
                depth[u] = depth[v] + 1;
                self(self, u, branch);
            }
        }

        out[v] = count;
    };
    dfs(dfs);

    vector<vector<int>> controlled(n + 1);
    for (auto [label, vertices1] : bird) {
        if (!berry.count(label)) continue;

        auto vertices2 = berry[label];
        set<int> s;
        for (int x : vertices2) s.emplace(in[x]);

        sort(vertices1.begin(), vertices1.end(), [&](int x, int y) { return leaf_depth[x] > leaf_depth[y]; });
        for (int x : vertices1) {
            int l = in[p[x]], r = out[p[x]] - 1;
            if (giant[x]) {
                l = in[leaf_branch[x]];
                r = out[leaf_branch[x]] - 1;
            }

            for (auto it = s.lower_bound(l); it != s.end() && *it <= r; it = s.lower_bound(l)) {
                controlled[x].emplace_back(*it);
                s.erase(it);
            }
        }
    }

    vector<pair<int, string>> labels;
    for (auto [label, vertices] : bird) {
        if (vertices.empty()) continue;

        int m = vertices.size();
        vector<int> overlap(m, 0);
        for (int i = 0; i < m; i++) {
            int x = vertices[i];
            sort(controlled[x].begin(), controlled[x].end());

            for (int j = 0; j < m; j++)
                if (i != j && depth[leaf_branch[vertices[j]]] >= depth[leaf_branch[x]]) {
                    int y = vertices[j];
                    if (leaf_branch[x] == leaf_branch[y]) {
                        overlap[i] |= 1 << j;
                        continue;
                    }

                    auto it = lower_bound(controlled[x].begin(), controlled[x].end(), in[leaf_branch[y]]);
                    if (it != controlled[x].end() && *it < out[leaf_branch[y]]) overlap[i] |= 1 << j;
                }
        }

        int least = 1e9, mask = 0;
        for (int i = 1; i < 1 << m; i++) {
            int changes = 0;
            for (int j = 0; j < m; j++)
                if (!((i >> j) & 1)) changes += controlled[vertices[j]].size() + 1;
                else if (i & overlap[j]) goto next;

            if (least > changes) {
                least = changes;
                mask = i;
            }
            next:;
        }

        for (int i = 0; i < m; i++)
            if (!((mask >> i) & 1)) {
                int x = vertices[i];

                string S(5, ' ');
                do {
                    int temp = count++;
                    for (char &c : S) {
                        c = (char) ('a' + temp % 26);
                        temp /= 26;
                    }
                } while (bird.count(S));

                labels.emplace_back(x, S);
                for (int v : controlled[x]) labels.emplace_back(order[v], S);
            }
    }

    cout << labels.size() << "\n";
    for (auto [x, S] : labels) cout << x << " " << S << "\n";
}
