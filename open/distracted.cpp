#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, l;
    cin >> n >> l;

    vector<string> names(n);
    unordered_map<string, int> indices;
    vector<int> status(n, -1);
    for (int i = 0; i < n; i++) {
        string s;
        char c;
        cin >> s >> c;

        names[i] = s;
        indices[s] = i;
        if (c == 'm') status[i] = 1;
        if (c == 'u') status[i] = 0;
    }

    vector<pair<int, int>> edges;
    vector<vector<int>> adj_list(n);
    while (l--) {
        string U, _, V;
        cin >> U >> _ >> V;

        int u = indices[U], v = indices[V];
        if (status[u] == 1 && !status[v]) { 
            cout << 1;
            exit(0);
        }
        adj_list[u].emplace_back(v);
        edges.emplace_back(u, v);
    }
    
    queue<int> q;
    vector<bool> visited(n, false);
    for (int i = 0; i < n; i++) 
        if (status[i] == 1 && !visited[i]) {
            visited[i] = true;
            q.emplace(i);
            while (!q.empty()) {
                int v = q.front();
                q.pop();
                
                for (int u : adj_list[v]) 
                    if (!visited[u]) {
                        if (!status[u]) {
                            cout << 1;
                            exit(0);
                        }
                        visited[u] = true;
                        q.emplace(u);
                    }
            }
        }
    
    for (auto [u, v] : edges) 
        if (status[u] != 0 && status[v] != 1) {
            cout << "?";
            exit(0);
        }
    
    cout << 0;
}
