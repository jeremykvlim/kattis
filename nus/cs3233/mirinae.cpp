#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n), indegree(n, 0);
    for (int &ai : a) {
        cin >> ai;

        indegree[--ai]++;
    }

    vector<bool> visited(n, false);
    int count = 0;
    auto dfs = [&](auto &&self, int v, bool c) {
        if (visited[v]) return;

        visited[v] = true;
        indegree[a[v]]--;
        count += c;
        if (!indegree[a[v]] || c) self(self, a[v], !c);
    };

    for (int i = 0; i < n; i++)
        if (!indegree[i]) dfs(dfs, i, true);

    for (int i = 0; i < n; i++)
        if (!visited[i]) dfs(dfs, i, false);

    cout << count;
}
