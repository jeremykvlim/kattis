#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list(n);
    vector<unordered_set<int>> adj_set(n);
    vector<int> degree(n);
    for (int _ = 0; _ < m; _++) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b);
        adj_list[b].emplace_back(a);
        adj_set[a].emplace(b);
        adj_set[b].emplace(a);
        degree[a]++;
        degree[b]++;
    }

    auto check = [&](const vector<int> &C = {}) {
        int w = C.size();

        vector<bool> seen(n, false);
        for (int v : C) seen[v] = true;

        int count = 0;
        for (int v : C)
            for (int u : adj_list[v])
                if (seen[u]) count++;
        if (count != (long long) w * (w - 1) || n - w < 1) return false;

        int sum = 0;
        for (int v : C) sum += degree[v];
        if (m - sum + count / 2 != n - w - 1) return false;

        int s = -1;
        vector<bool> T(n, false);
        for (int i = 0; i < n; i++)
            if (!seen[i]) {
                T[i] = true;
                s = i;
            }

        vector<bool> visited(n, false);
        visited[s] = true;
        queue<int> q;
        q.emplace(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            int t = 0, c = 0;
            for (int u : adj_list[v])
                if (T[u]) {
                    t++;
                    if (!visited[u]) {
                        visited[u] = true;
                        q.emplace(u);
                    }
                } else c++;

            if (t > 3 || c > 1 || (c && t != 1)) return false;
        }

        for (int i = 0; i < n; i++)
            if (T[i] && !visited[i]) return false;
        return true;
    };

    auto check5 = [&]() {
        vector<int> C;
        for (int i = 0; i < n; i++)
            if (degree[i] >= 4) C.emplace_back(i);

        if (C.size() >= 5) {
            if (check(C)) cout << "yes " << C.size();
            else cout << "no";
            exit(0);
        }
    };
    check5();

    auto check4 = [&]() {
        for (int a = 0; a < n; a++) {
            int s = adj_list[a].size();
            for (int i = 0; i < s; i++) {
                int b = adj_list[a][i];
                for (int j = i + 1; j < s; j++) {
                    int c = adj_list[a][j];
                    if (adj_set[b].count(c))
                        for (int k = j + 1; k < s; k++) {
                            int d = adj_list[a][k];
                            if (adj_set[b].count(d) && adj_set[c].count(d)) return check({a, b, c, d});
                        }
                }
            }
        }
        return false;
    };
    if (check4()) {
        cout << "yes 4";
        exit(0);
    }

    auto check3 = [&]() {
        for (int a = 0; a < n; a++) {
            int s = adj_list[a].size();
            for (int i = 0; i < s; i++) {
                int b = adj_list[a][i];
                for (int j = i + 1; j < s; j++) {
                    int c = adj_list[a][j];
                    if (adj_set[b].count(c)) return check({a, b, c});
                }
            }
        }
        return false;
    };
    if (check3()) {
        cout << "yes 3";
        exit(0);
    }

    auto check2 = [&]() {
        vector<int> count(n, 0);
        for (int a = 0; a < n; a++)
            for (int b : adj_list[a])
                if (degree[b] != 2) count[a]++;

        for (int a = 0; a < n; a++)
            for (int b : adj_list[a]) {
                if (a >= b || degree[a] + degree[b] != m - n + 4 || count[a] > (degree[b] != 2) || count[b] > (degree[a] != 2)) continue;
                if (check({a, b})) return true;
            }
        return false;
    };
    if (check2()) {
        cout << "yes 2";
        exit(0);
    }

    auto check1 = [&]() {
        for (int a = 0; a < n; a++) {
            if (degree[a] != m - n + 2) continue;
            if (check({a})) return true;
        }
        return false;
    };
    if (check1()) {
        cout << "yes 1";
        exit(0);
    }

    if (check()) cout << "yes 0";
    else cout << "no";
}
