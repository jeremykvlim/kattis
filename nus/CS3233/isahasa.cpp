#include <bits/stdc++.h>
using namespace std;

unordered_set<int> bfs(deque<int> q, vector<vector<int>> &adj_list) {
    unordered_set<int> visited(q.begin(), q.end());
    while (!q.empty()) {
        int v = q.front();
        q.pop_front();
        
        for (int u : adj_list[v]) 
            if (visited.emplace(u).second) q.emplace_back(u);
    }
    return visited;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    unordered_map<string, int> compress;
    vector<pair<int, int>> is, has;
    for (int i = 0; i < n; i++) {
        string c1, r, c2;
        cin >> c1 >> r >> c2;
        
        int a = compress.emplace(c1, compress.size()).first->second, b = compress.emplace(c2, compress.size()).first->second;
        (r == "is-a" ? is : has).emplace_back(a, b);
    }

    int size = compress.size();
    vector<vector<int>> is_adj_list(size), both_adj_list(size);
    vector<unordered_set<int>> direct(size), transitive(size), superclasses(size), subclasses(size);

    for (auto [a, b] : is) {
        is_adj_list[a].emplace_back(b);
        both_adj_list[a].emplace_back(b);
    }

    for (auto [a, b] : has) both_adj_list[a].emplace_back(b);

    for (int i = 0; i < size; i++) {
        superclasses[i] = bfs({i}, is_adj_list);
        for (int c : superclasses[i]) subclasses[c].emplace(i);
    }

    for (auto [a, b] : has)
        for (int c : subclasses[a]) direct[c].emplace(b);

    for (int i = 0; i < size; i++) transitive[i] = bfs({direct[i].begin(), direct[i].end()}, both_adj_list);

    for (int i = 1; i <= m; i++) {
        string c1, r, c2;
        cin >> c1 >> r >> c2;
        
        cout << "Query " << i << ": " << ((r == "has-a" ? transitive : superclasses)[compress[c1]].count(compress[c2]) ? "true" : "false") << "\n";
    }
}
