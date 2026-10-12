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
    T inf = numeric_limits<T>::min();
    vector<vector<T>> dp(n + 1, vector<T>(n, inf));
    dp[0][0] = 0;
    vector<vector<int>> valid(n + 1, vector<int>(n, -1));
    vector<int> last_level(n, -1);
    last_level[0] = 0;
    queue<pair<int, int>> q;
    q.emplace(0, 0);
    while (!q.empty()) {
        auto [k, v] = q.front();
        q.pop();

        if (k == n || dp[k][v] == inf) continue;

        for (auto [u, w] : adj_list[v]) {
            if (last_level[u] < k + 1) {
                q.emplace(k + 1, u);
                valid[k + 1][u] = last_level[u];
                last_level[u] = k + 1;
                dp[k + 1][u] = inf;
            }

            dp[k + 1][u] = max(dp[k + 1][u], dp[k][v] + w);
        }
    }

    using U = conditional_t<sizeof(T) <= sizeof(int), double, long double>;
    U lambda = numeric_limits<U>::lowest();
    for (int v = 0; v < n; v++)
        if (last_level[v] == n && dp[n][v] != inf) {
            U m = numeric_limits<U>::max();
            int k = valid[n][v];
            while (k > -1) {
                if (dp[k][v] != inf) m = min(m, ((U) dp[n][v] - dp[k][v]) / (n - k));
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

    int t_max = -201;
    vector<int> t(k);
    for (int &ti : t) {
        cin >> ti;

        t_max = max(t_max, ti);
    }

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

    int period = 1;
    for (int c = 1; c <= sccs; c++) {
        if (members[c].size() == 1) {
            int v = members[c][0];
            if (find(adj_list2[v].begin(), adj_list2[v].end(), v) == adj_list2[v].end()) continue;
        }

        vector<int> dist(k, -1);
        dist[members[c][0]] = 0;
        queue<int> q;
        q.emplace(members[c][0]);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int u : adj_list2[v])
                if (component[u] == c && !~dist[u]) {
                    dist[u] = dist[v] + 1;
                    q.emplace(u);
                }
        }

        int g = 0;
        for (int v : members[c])
            for (int u : adj_list2[v])
                if (component[u] == c) g = gcd(g, abs(dist[v] + 1 - dist[u]));
        period = min((long long) n + 1, (long long) period / gcd(period, g) * g);
    }

    vector<long long> dp{0, t_max}, state(t.begin(), t.end()), delayed(t.begin(), t.end()), temp(k);
    int start = -1;
    auto offset = 0LL;
    for (int l = 2; l <= n; l++) {
        fill(temp.begin(), temp.end(), -1e18);
        for (int i = 0; i < k; i++)
            for (int j = 0; j < k; j++) temp[i] = max(temp[i], state[j] + w[i][j]);
        state = temp;
        dp.emplace_back(*max_element(state.begin(), state.end()));

        if (l > period) {
            offset = dp[l] - dp[l - period];
            for (int i = 0; i < k; i++)
                if (state[i] != delayed[i] + offset) goto next;
            start = l - period;
            break;

            next:;
            fill(temp.begin(), temp.end(), -1e18);
            for (int i = 0; i < k; i++)
                for (int j = 0; j < k; j++) temp[i] = max(temp[i], delayed[j] + w[i][j]);
            delayed = temp;
        }
    }

    auto quo = 0.;
    for (int l = 1; l < dp.size(); l++) quo = max(quo, dp[l] / (double) ((long long) a * l + b));

    if (~start)
        for (int l = start; l < min({start + period, (int) dp.size(), n + 1}); l++) {
            int q = (n - l) / period;
            quo = max(quo, (dp[l] + (long long) q * offset) / (double) (a * (l + (long long) q * period) + b));
        }
    cout << fixed << setprecision(6) << quo;
}
