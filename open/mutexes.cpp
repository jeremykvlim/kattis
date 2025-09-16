#include <bits/stdc++.h>
#include <tr2/dynamic_bitset>
using namespace std;
using namespace tr2;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    unordered_map<string, int> functions, mutexes;
    vector<pair<string, int *>> calls;
    vector<vector<pair<string, int>>> commands(n);
    for (int i = 0; i < n; i++) {
        int m;
        string f;
        cin >> m >> f;

        functions[f] = i;
        commands[i].resize(m);
        for (auto &[command, v] : commands[i]) {
            string x;
            cin >> command >> x;

            if (command != "call") {
                if (!mutexes.count(x)) v = mutexes[x] = mutexes.size();
                else v = mutexes[x];
            } else calls.emplace_back(x, &v);
        }
    }
    for (auto &[f, v] : calls) *v = functions[f];

    vector<vector<int>> adj_list(n);
    vector<int> degree(n, 0);
    for (int i = 0; i < n; i++) {
        unordered_set<int> distinct;
        for (auto [command, v] : commands[i])
            if (command == "call") distinct.emplace(v);

        for (int v : distinct) {
            adj_list[v].emplace_back(i);
            degree[i]++;
        }
    }

    queue<int> q;
    for (int i = 0; i < n; i++)
        if (!degree[i]) q.emplace(i);

    vector<int> order;
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        order.emplace_back(v);
        for (int u : adj_list[v])
            if (!--degree[u]) q.emplace(u);
    }

    int m = mutexes.size();
    vector<dynamic_bitset<>> visited(n, dynamic_bitset<>(m)), unlocked(n, dynamic_bitset<>(m)), locked(n, dynamic_bitset<>(m)), parity(n, dynamic_bitset<>(m));
    for (int i : order)
        for (auto [command, v] : commands[i])
            if (command == "call") {
                auto mask = visited[v] & ~visited[i];
                visited[i] |= mask;
                unlocked[i] |= mask;
                locked[i] |= mask;

                auto temp1 = parity[i] & visited[v], temp2 = ~parity[i] & visited[v];
                unlocked[i] &= ~(((visited[v] ^ temp1) & ~unlocked[v]) | (temp1 & ~locked[v]));
                locked[i] &= ~(((visited[v] ^ temp2) & ~unlocked[v]) | (temp2 & ~locked[v]));
                parity[i] ^= parity[v] & visited[v];
            } else {
                if (!visited[i][v]) visited[i][v] = unlocked[i][v] = locked[i][v] = true;

                if (command == "acquire") {
                    if (unlocked[i][v] && parity[i][v]) unlocked[i][v] = false;
                    if (locked[i][v] && !parity[i][v]) locked[i][v] = false;
                    parity[i].flip(v);
                } else if (command == "release") {
                    if (unlocked[i][v] && !parity[i][v]) unlocked[i][v] = false;
                    if (locked[i][v] && parity[i][v]) locked[i][v] = false;
                    parity[i].flip(v);
                } else {
                    if (unlocked[i][v] && !parity[i][v]) unlocked[i][v] = false;
                    if (locked[i][v] && parity[i][v]) locked[i][v] = false;
                }
            }

    dynamic_bitset<> held(m);
    auto dfs = [&](auto &&self, int f) -> void {
        for (auto [command, v] : commands[f])
            if (command == "acquire") {
                if (held[v]) {
                    cout << "deadlock";
                    exit(0);
                }
                held[v] = true;
            } else if (command == "release") {
                if (!held[v]) {
                    cout << "error";
                    exit(0);
                }
                held[v] = false;
            } else if (command == "access") {
                if (!held[v]) {
                    cout << "corruption";
                    exit(0);
                }
            } else {
                if ((((~held & visited[v]) & ~unlocked[v]) | ((held & visited[v]) & ~locked[v])).any()) {
                    self(self, v);
                    return;
                }
                held ^= parity[v] & visited[v];
            }
    };
    dfs(dfs, functions["main"]);
    cout << "a-ok";
}
