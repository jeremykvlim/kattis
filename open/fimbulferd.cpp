#include <bits/stdc++.h>
using namespace std;

tuple<int, vector<int>, vector<int>, vector<vector<int>>>  tarjan(int n, vector<vector<int>> &adj_list) {
    vector<int> order(n, 0), low(n, 0), component(n, 0), size(1);
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
            size.emplace_back();
            members.emplace_back();
            int u;
            do {
                u = st.top();
                st.pop();
                stacked[u] = false;
                component[u] = sccs;
                size[sccs]++;
                members[sccs].emplace_back(u);
            } while (u != v);
        }
    };

    for (int v = 0; v < n; v++)
        if (!order[v]) dfs(dfs, v);

    return {sccs, component, size, members};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int V, E, Q;
    cin >> V >> E >> Q;

    vector<array<int, 3>> edges(E);
    vector<vector<int>> adj_list1(V);
    for (auto &[a, b, t] : edges) {
        cin >> a >> b >> t;
        a--;
        b--;

        adj_list1[a].emplace_back(b);
    }

    auto [sccs, component, size, members] = tarjan(V, adj_list1);
    vector<vector<int>> dag(sccs + 1);
    vector<vector<pair<int, int>>> adj_list2(V);
    for (auto [u, v, w] : edges)
        if (component[u] != component[v]) dag[component[u]].emplace_back(component[v]);
        else adj_list2[u].emplace_back(v, w);

    vector<long long> dist(V);
    vector<int> len(V, 0);
    vector<bool> queued(V, false), neg_cycle(sccs + 1, false);
    deque<int> dq;
    for (int c = 1; c <= sccs; c++) {
        dq.clear();
        for (int v : members[c]) {
            dist[v] = len[v] = 0;
            queued[v] = true;
            dq.emplace_back(v);
        }

        while (!dq.empty() && !neg_cycle[c]) {
            int v = dq.front();
            dq.pop_front();

            queued[v] = false;

            for (auto [u, w] : adj_list2[v])
                if (dist[u] > dist[v] + w) {
                    dist[u] = dist[v] + w;
                    len[u] = len[v] + 1;

                    if (len[u] >= size[c]) {
                        neg_cycle[c] = true;
                        break;
                    }

                    if (!queued[u]) {
                        queued[u] = true;
                        if (dq.empty() || dist[u] < dist[dq.front()]) dq.emplace_front(u);
                        else dq.emplace_back(u);
                    }
                }
        }
    }

    vector<bitset<(int) 5e3 + 1>> reachable(sccs + 1), reachable_neg_cycle(sccs + 1);
    for (int v = 1; v <= sccs; v++) {
        reachable[v][v] = true;
        for (int u : dag[v]) {
            reachable[v] |= reachable[u];
            reachable_neg_cycle[v] |= reachable_neg_cycle[u];
        }
        if (neg_cycle[v]) reachable_neg_cycle[v] |= reachable[v];
    }

    vector<long long> temperature(Q);
    vector<pair<int, int>> queries(Q);
    vector<vector<pair<int, int>>> sources(V);
    for (int q = 0; q < Q; q++) {
        auto &[a, b] = queries[q];
        cin >> a >> b;
        a--;
        b--;

        if (!reachable[component[a]][component[b]]) temperature[q] = LLONG_MAX;
        else if (reachable_neg_cycle[component[a]][component[b]]) temperature[q] = LLONG_MIN;
        else sources[a].emplace_back(b, q);
    }

    vector<array<int, 3>> temp;
    vector<vector<pair<int, int>>> adj_list3(V);
    for (auto [u, v, w] : edges)
        if (!neg_cycle[component[u]] && !neg_cycle[component[v]]) {
            temp.push_back({u, v, w});
            adj_list3[u].emplace_back(v, w);
        }
    edges = temp;

    vector<long long> potential(V, 0);
    fill(queued.begin(), queued.end(), true);
    dq.clear();
    for (int v = 0; v < V; v++) dq.emplace_back(v);

    while (!dq.empty()) {
        int v = dq.front();
        dq.pop_front();

        queued[v] = false;

        for (auto [u, w] : adj_list3[v])
            if (potential[u] > potential[v] + w) {
                potential[u] = potential[v] + w;

                if (!queued[u]) {
                    queued[u] = true;
                    if (dq.empty() || potential[u] < potential[dq.front()]) dq.emplace_front(u);
                    else dq.emplace_back(u);
                }
            }
    }

    vector<vector<pair<int, long long>>> adj_list4(V);
    for (auto [u, v, w] : edges) adj_list4[u].emplace_back(v, w + potential[u] - potential[v]);

    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    for (int s = 0; s < V; s++)
        if (!sources[s].empty()) {
            fill(dist.begin(), dist.end(), 1e18);
            dist[s] = 0;
            pq.emplace(0, s);
            while (!pq.empty()) {
                auto [d, v] = pq.top();
                pq.pop();

                if (d != dist[v]) continue;

                for (auto [u, w] : adj_list4[v])
                    if (dist[u] > d + w) {
                        dist[u] = d + w;
                        pq.emplace(d + w, u);
                    }
            }

            for (auto [t, q] : sources[s]) {
                auto [a, b] = queries[q];
                temperature[q] = dist[t] - potential[a] + potential[b];
            }
        }

    for (auto t : temperature)
        if (t == LLONG_MAX) cout << "engin leid\n";
        else if (t == LLONG_MIN) cout << "nogu hlytt\n";
        else cout << t << "\n";

}
