#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> g(n), degree(n, 0);
    for (int &gi : g) {
        cin >> gi;
        gi--;

        degree[gi]++;
    }

    int count = 0;
    queue<int> q;
    for (int i = 0; i < n; i++)
        if (!degree[i]) {
            count++;
            q.emplace(i);
        }

    auto temp = degree;
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        int u = g[v];
        if (!--temp[u]) q.emplace(u);
    }

    int cycles = 0;
    vector<bool> visited(n, false);
    for (int v = 0; v < n; v++)
        if (temp[v] > 0 && !visited[v]) {
            bool one = true;
            for (int u = v; !visited[u]; u = g[u]) {
                visited[u] = true;
                if (degree[u] != 1) one = false;
            }
            cycles += one;
        }

    if (!count && cycles == 1) cout << 0;
    else cout << count + cycles;
}
