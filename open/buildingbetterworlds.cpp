#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x, y;
    cin >> n >> x >> y;

    vector<vector<int>> adj_list(n);
    while (x--) {
        int a, b;
        cin >> a >> b;

        adj_list[a - 1].emplace_back(b - 1);
        adj_list[b - 1].emplace_back(a - 1);
    }

    vector<array<int, 3>> talks(y);
    for (auto &[a, b, c] : talks) {
        cin >> a >> b >> c;
        a--;
        b--;
        c--;
    }

    int components = 0;
    vector<int> component(n, -1), color(n, 0);
    for (int s = 0; s < n; s++)
        if (!~component[s]) {
            component[s] = components;
            queue<int> q;
            q.emplace(s);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                for (int u : adj_list[v])
                    if (!~component[u]) {
                        component[u] = components;
                        color[u] = color[v] ^ 1;
                        q.emplace(u);
                    } else if (color[u] == color[v]) {
                        cout << "Ómögulegt";
                        exit(0);
                    }
            }

            components++;
        }

    array<int, 3> chosen{-1, -1, -1};
    for (auto [a, b, c] : talks)
        if ((component[a] != component[b] || color[a] == color[b]) &&
            (component[a] != component[c] || color[a] == color[c]) &&
            (component[b] != component[c] || color[b] == color[c])) {
            chosen = {a, b, c};
            break;
        }

    if (chosen == array{-1, -1, -1}) {
        cout << "Ómögulegt";
        exit(0);
    }

    auto [a, b, c] = chosen;
    for (int i = 0; i < n; i++)
        if (component[i] == component[a]) cout << (color[i] == color[a] ? 'F' : 'M');
        else if (component[i] == component[b]) cout << (color[i] == color[b] ? 'F' : 'M');
        else if (component[i] == component[c]) cout << (color[i] == color[c] ? 'F' : 'M');
        else cout << (color[i] ? 'F' : 'M');
}