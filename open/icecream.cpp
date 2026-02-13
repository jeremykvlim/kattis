#include <bits/stdc++.h>
using namespace std;

tuple<int, vector<int>, vector<vector<int>>> tarjan(int n, vector<vector<int>> &adj_list) {
    vector<int> order(n, 0), low(n, 0), component(n, 0);
    vector<vector<int>> members(1);
    vector<bool> stacked(n, false);
    stack<int> st;
    int count = 0, sccs = 0;

    auto dfs = [&](auto &&self, int v) -> void {
        order[v] = low[v] = ++count;
        st.emplace(v);
        stacked[v] = true;

        for (int u : adj_list[v])
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

    using U = typename conditional<is_same<T, int>::value, double, typename conditional<is_same<T, long long>::value, double, void>::type>::type;
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

    int n, k, a, b;
    cin >> n >> k >> a >> b;

    vector<int> t(k);
    for (int &ti : t) cin >> ti;

    vector<vector<int>> w(k, vector<int>(k));
    vector<vector<pair<int, int>>> adj_list1(k);
    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++) {
            int u;
            cin >> u;

            adj_list1[j].emplace_back(i, w[i][j] = t[i] + u);
        }
    auto lambda = dasdan_gupta(k, adj_list1);

    vector<double> potential(k, 0);
    for (int _ = 0; _ < k; _++) {
        auto temp = potential;
        for (int u = 0; u < k; u++)
            for (int v = 0; v < k; v++) temp[u] = max(temp[u], potential[v] + w[u][v] - lambda);
        potential = temp;
    }

    vector<vector<int>> adj_list2(k);
    for (int u = 0; u < k; u++)
        for (int v = 0; v < k; v++)
            if (potential[v] + w[u][v] - lambda > potential[u] - 1e-10) adj_list2[v].emplace_back(u);
    auto [sccs, component, members] = tarjan(k, adj_list2);

    vector<bool> cyclic(sccs + 1, false);
    for (int c = 1; c <= sccs; c++)
        if (members[c].size() > 1) cyclic[c] = true;
        else {
            int v = members[c][0];
            for (int u : adj_list2[v])
                if (u == v) {
                    cyclic[c] = true;
                    break;
                }
        }

    int period = 0;
    for (int c = 1; c <= sccs; c++)
        if (cyclic[c]) {
            vector<int> dist(k, -1);
            dist[members[c][0]] = 0;
            queue<int> q;
            q.emplace(members[c][0]);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                for (int u : adj_list2[v])
                    if (!~dist[u] && component[u] == c) {
                        dist[u] = dist[v] + 1;
                        q.emplace(u);
                    }
            }

            int g = 0;
            for (int v : members[c])
                for (int u : adj_list2[v])
                    if (component[u] == c) g = __gcd(g, abs(dist[v] - dist[u] + 1));

            period = __gcd(period, g);
        }

    vector<int> dp{0, *max_element(t.begin(), t.end())};
    int start, offset = 0;
    for (int l = 2;; l++) {
        auto temp = t;
        for (int i = 0; i < k; i++)
            for (int j = 0; j < k; j++) temp[i] = max(temp[i], t[j] + w[i][j]);
        t = temp;
        dp.emplace_back(*max_element(t.begin(), t.end()));

        if (l > period) {
            offset = dp[l] - dp[l - period];
            for (int i = l - 2 * period + 1; i <= l; i++)
                if (dp[i] - dp[i - period] != offset) goto next;

            int i = l;
            for (; i >= period; i--)
                if (dp[i] - dp[i - period] != offset) break;
            start = i - period + 1;
            break;
        }
        next:;
    }

    auto quo = 0.;
    for (int l = start; l < min({start + period, (int) dp.size(), n + 1}); l++) {
        int q = (n - l) / period;
        quo = max({quo, dp[l] / (double) (a * l + b), (dp[l] + (long long) q * offset) / (double) (a * (l + (long long) q * period) + b)});
    }
    cout << fixed << setprecision(6) << quo;
}
