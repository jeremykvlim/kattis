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
    auto dfs = [&](auto &&self, int v, bool guard) {
        visited[v] = true;
        indegree[a[v]]--;
        if (guard) count++;
        if ((!indegree[a[v]] || guard) && !visited[a[v]]) self(self, a[v], !guard);
    };

    for (int i = 0; i < n; i++)
        if (!indegree[i] && !visited[i]) dfs(dfs, i, true);

    for (int i = 0; i < n; i++)
        if (!visited[i]) dfs(dfs, i, false);

    cout << count;
}
