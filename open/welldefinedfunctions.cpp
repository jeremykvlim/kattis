#include <bits/stdc++.h>
using namespace std;

tuple<int, vector<int>, vector<vector<int>>> tarjan(int n, vector<vector<pair<int, int>>> &adj_list) {
    vector<int> order(n, 0), low(n, 0), component(n, 0);
    vector<vector<int>> members(1);
    vector<bool> stacked(n, false);
    stack<int> st;
    int count = 0, sccs = 0;

    auto dfs = [&](auto &&self, int v) -> void {
        order[v] = low[v] = ++count;
        st.emplace(v);
        stacked[v] = true;

        for (auto [u, w] : adj_list[v])
            if (!order[u]) {
                self(self, u);
                low[v] = min(low[v], low[u]);
            } else if (stacked[u]) low[v] = min(low[v], order[u]);

        if (order[v] == low[v]) {
            sccs++;
            members.emplace_back();
            int u;
            do {
                u = st.top();
                st.pop();
                stacked[u] = false;
                component[u] = sccs;
                members[sccs].emplace_back(u);
            } while (u != v);
        }
    };

    for (int v = 0; v < n; v++)
        if (!order[v]) dfs(dfs, v);

    return {sccs, component, members};
}

template <typename T>
auto dasdan_gupta(int n, const vector<vector<pair<int, T>>> &adj_list) {
    vector<int> first(n, 0), last(n, 0);
    for (int _ = 0; _ < n; _++) {
        for (int v = 0; v < n; v++) {
            int degree = adj_list[v].size();
            for (auto [u, w] : adj_list[v]) degree += first[u];
            last[v] = degree;
        }
        first.swap(last);
    }
    int s = 0;
    for (int v = 1; v < n; v++)
        if (first[s] > first[v]) s = v;

    T inf = numeric_limits<T>::max() / 4;
    vector<vector<T>> dp(n + 1, vector<T>(n, -inf));
    dp[0][s] = 0;
    vector<vector<int>> valid(n + 1, vector<int>(n, -1));
    vector<int> last_level(n, -1);
    last_level[s] = 0;
    queue<pair<int, int>> q;
    q.emplace(0, s);
    while (!q.empty()) {
        auto [k, v] = q.front();
        q.pop();

        if (k == n || dp[k][v] == -inf) continue;

        for (auto [u, w] : adj_list[v]) {
            if (last_level[u] < k + 1) {
                q.emplace(k + 1, u);
                valid[k + 1][u] = last_level[u];
                last_level[u] = k + 1;
                dp[k + 1][u] = -inf;
            }

            dp[k + 1][u] = max(dp[k + 1][u], dp[k][v] + w);
        }
    }

    using U = typename conditional<is_same<T, int>::value, double, typename conditional<is_same<T, long long>::value, long double, void>::type>::type;
    U lambda = numeric_limits<U>::lowest();
    for (int v = 0; v < n; v++)
        if (last_level[v] == n && dp[n][v] != -inf) {
            U m = numeric_limits<U>::max();
            int k = valid[n][v];
            while (k > -1) {
                if (dp[k][v] != -inf) m = min(m, (U) (dp[n][v] - dp[k][v]) / (n - k));
                k = valid[k][v];
            }
            lambda = max(lambda, m);
        }
    return lambda;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    while (cin >> m) {
        cin.ignore();

        vector<string> functions(m), names(m);
        unordered_map<string, int> indices;
        for (int i = 0; i < m; i++) {
            getline(cin, functions[i]);

            indices[names[i] = functions[i].substr(0, functions[i].find('=') - 4)] = i;
        }

        __int128 visited = 0;
        vector<vector<pair<int, int>>> adj_list_regular(m), adj_list_transpose(m);
        for (int i = 0; i < m; i++) {
            auto f = functions[i];
            istringstream iss(f.substr(f.find('=') + 2));
            vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};
            for (auto t : tokens) {
                if (t == "+" || t == "-" || t == "n" || all_of(t.begin(), t.end(), [&](char c) { return isdigit(c); })) continue;

                int l = t.find('('), r = t.find(')');
                auto name = t.substr(0, l), arg = t.substr(l + 1, r - l - 1);

                int x = 0;
                if (arg.size() > 1) x = arg[1] == '+' ? stoi(arg.substr(2)) : -stoi(arg.substr(2));
                if (!indices.count(name)) {
                    visited |= (__int128) 1 << i;
                    continue;
                }
                int j = indices[name];
                adj_list_regular[i].emplace_back(j, x);
                adj_list_transpose[j].emplace_back(i, x);
            }
        }

        auto [sccs, component, members] = tarjan(m, adj_list_regular);
        for (int c = 1; c <= sccs; c++) {
            int n = members[c].size();
            if (n == 1) {
                int v = members[c][0];
                for (auto [u, w] : adj_list_regular[v])
                    if (u == v) goto cyclic;
                continue;
            }

            cyclic:;
            vector<int> member_id(m, -1);
            for (int i = 0; i < n; i++) member_id[members[c][i]] = i;

            vector<vector<pair<int, int>>> adj_list_scc(n);
            for (int v : members[c])
                for (auto [u, w] : adj_list_regular[v])
                    if (component[u] == c) adj_list_scc[member_id[v]].emplace_back(member_id[u], w);

            if (dasdan_gupta(n, adj_list_scc) >= 0)
                for (int v : members[c]) visited |= (__int128) 1 << v;
        }

        queue<int> q;
        for (int v = 0; v < m; v++)
            if ((visited >> v) & 1) q.emplace(v);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (auto [u, w] : adj_list_transpose[v])
                if (!((visited >> u) & 1)) {
                    visited |= (__int128) 1 << u;
                    q.emplace(u);
                }
        }

        if (!visited) cout << "well-defined\n";
        else {
            vector<string> nwd;
            for (int v = 0; v < m; v++)
                if ((visited >> v) & 1) nwd.emplace_back(names[v]);
            sort(nwd.begin(), nwd.end());
            cout << "not well-defined ";
            for (auto &name : nwd) cout << name << " ";
            cout << "\n";
        }
    }
}
