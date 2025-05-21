#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        vector<int> parent{-1}, size{1}, prev_size{1};
        vector<string> name{"root"};
        vector<vector<string>> last{{"root"}};
        vector<map<string, int>> children(1);
        stack<pair<int, int>> undo;
        int v = 0, nodes = 1;

        auto rebuild = [&]() {
            last[v].clear();
            for (auto it = children[v].rbegin(); it != children[v].rend(); it++)
                for (auto d : last[it->second]) {
                    last[v].emplace_back(d);
                    if (last[v].size() == 5) goto done;
                }
            last[v].emplace_back(name[v]);
            done:;
        };

        int q;
        cin >> q;

        while (q--) {
            string c;
            cin >> c;

            if (c == "MKDIR") {
                string s;
                cin >> s;

                if (children[v].count(s)) {
                    cout << "ERR\n";
                    continue;
                }

                cout << "OK\n";
                parent.emplace_back(v);
                name.emplace_back(s);
                size.emplace_back(1);
                prev_size.emplace_back(1);
                last.emplace_back(vector<string>{s});
                children.emplace_back();
                int u = nodes++;
                undo.emplace(1, u);
                children[v][s] = u;
                size[v]++;
                rebuild();
            } else if (c == "RM") {
                string s;
                cin >> s;

                if (!children[v].count(s)) {
                    cout << "ERR\n";
                    continue;
                }

                cout << "OK\n";
                int u = children[v][s];
                undo.emplace(2, u);
                size[v] -= size[u];
                children[v].erase(s);
                rebuild();
            } else if (c == "CD") {
                string s;
                cin >> s;

                if (s == "..") {
                    if (!~parent[v]) {
                        cout << "ERR\n";
                        continue;
                    }

                    cout << "OK\n";
                    undo.emplace(3, v);
                    int diff = size[v] - prev_size[v];
                    v = parent[v];
                    size[v] += diff;
                    rebuild();
                } else {
                    if (!children[v].count(s)) {
                        cout << "ERR\n";
                        continue;
                    }

                    cout << "OK\n";
                    undo.emplace(-3, v);
                    v = children[v][s];
                    prev_size[v] = size[v];
                }
            } else if (c == "SZ") cout << size[v] << "\n";
            else if (c == "LS") {
                if (children[v].empty()) {
                    cout << "EMPTY\n";
                    continue;
                }

                if (children[v].size() <= 10)
                    for (auto [d, u] : children[v]) cout << d << "\n";
                else {
                    for_each_n(children[v].begin(), 5, [&](const auto &p) { cout << p.first << "\n"; });
                    cout << "...\n";
                    stack<string> temp;
                    for_each_n(children[v].rbegin(), 5, [&](const auto &p) { temp.emplace(p.first); });
                    while (!temp.empty()) {
                        cout << temp.top() << "\n";
                        temp.pop();
                    }
                }
            } else if (c == "TREE") {
                if (children[v].empty()) {
                    cout << "EMPTY\n";
                    continue;
                }

                int k;
                auto dfs = [&](auto &&self, int v) {
                    cout << name[v] << "\n";
                    k--;
                    for (auto [d, u] : children[v]) {
                        if (!k) return;
                        self(self, u);
                    }
                };

                if (size[v] <= 10) {
                    k = size[v];
                    dfs(dfs, v);
                } else {
                    k = 5;
                    dfs(dfs, v);
                    cout << "...\n";
                    for (int i = 4; ~i; i--) cout << last[v][i] << "\n";
                }
            } else {
                if (undo.empty()) {
                    cout << "ERR\n";
                    continue;
                }

                cout << "OK\n";
                auto [cmd, u] = undo.top();
                undo.pop();
                
                if (cmd == 1) {
                    children[v].erase(name[u]);
                    size[v]--;
                    rebuild();
                } else if (cmd == 2) {
                    children[v][name[u]] = u;
                    size[v] += size[u];
                    rebuild();
                } else if (cmd == 3) {
                    v = u;
                    prev_size[v] = size[v];
                } else {
                    int diff = size[v] - prev_size[v];
                    v = u;
                    size[v] += diff;
                    rebuild();
                }
            }
        }
    }
}
