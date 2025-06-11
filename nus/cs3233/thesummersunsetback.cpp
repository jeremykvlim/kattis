#include <bits/stdc++.h>
#include <tr2/dynamic_bitset>
using namespace std;
using namespace tr2;

struct LaminarTree {
    int n;
    vector<vector<int>> LT;
    vector<int> cut;

    LaminarTree(int n, const vector<array<int, 3>> &edges) : n(n), LT(2 * n - 1), cut(n - 1) {
        build(edges);
    }

    void build(const vector<array<int, 3>> &edges) {
        vector<vector<pair<int, int>>> adj_list(2 * n - 1);
        for (auto [u, v, w] : edges) {
            adj_list[u].emplace_back(v, w);
            adj_list[v].emplace_back(u, w);
        }

        vector<bool> active(2 * n - 1, true);
        for (int phase = n, p = n; phase > 1 && p < 2 * n - 1; phase--, p++) {
            vector<bool> visited(p, false);
            vector<int> order(phase, -1), dist(p, 0);
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
            for (int v = 0; v < p; v++)
                if (active[v]) {
                    for (auto [u, w] : adj_list[v]) dist[v] += w;
                    pq.emplace(dist[v], v);
                }

            auto temp = dist;
            for (int j = 0; j < phase; j++)
                while (!~order[j]) {
                    auto [d, v] = pq.top();
                    pq.pop();

                    if (visited[v]) continue;
                    visited[v] = true;

                    order[j] = v;
                    for (auto [u, w] : adj_list[v])
                        if (!visited[u]) {
                            dist[u] -= w;
                            pq.emplace(dist[u], u);
                        }
                }

            int u = order[phase - 1], v = order[phase - 2];
            cut[p - n] = temp[u] - dist[u];
            active[u] = active[v] = false;
            adj_list[u].clear();
            adj_list[v].clear();
            LT[p].emplace_back(u);
            LT[p].emplace_back(v);

            for (int s = 0; s < p; s++)
                if (active[s])
                    for (auto &[t, w] : adj_list[s]) {
                        if (t == u || t == v) {
                            t = p;
                            adj_list[p].emplace_back(s, w);
                        }
                    }
        }
    }

    pair<int, vector<bool>> minimum_cut() {
        auto it = min_element(cut.begin(), cut.end());
        int min_cut = *it, i = it - cut.begin();
        vector<bool> side(n, false);
        stack<int> st;
        st.emplace(LT[i + n][0]);
        while (!st.empty()) {
            int v = st.top();
            st.pop();

            if (v < n) side[v] = true;
            else
                for (int u : LT[v]) st.emplace(u);
        }
        return {min_cut, side};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, s;
    cin >> n >> s;

    dynamic_bitset<> p_assigned(s), p_unassigned(s);
    vector<array<int, 3>> unassigned;
    for (int i = 1; i <= n; i++) {
        char ch;
        cin >> ch;

        if (ch == 'A') {
            int c;
            cin >> c;
            c--;

            p_assigned.flip(c);
        } else {
            int ai, bi;
            cin >> ai >> bi;
            ai--;
            bi--;

            unassigned.push_back({ai, bi, i});
            p_unassigned.flip(ai);
        }
    }
    int U = unassigned.size();

    vector<vector<int>> adj_list(s);
    for (int i = 0; i < U; i++) {
        adj_list[unassigned[i][0]].emplace_back(unassigned[i][1]);
        adj_list[unassigned[i][1]].emplace_back(unassigned[i][0]);
    }

    vector<vector<int>> components(s);
    vector<bool> visited(s, false);
    for (int i = 0; i < s; i++) {
        if (!visited[i]) {
            visited[i] = true;
            components[i].emplace_back(i);
            queue<int> q;
            q.emplace(i);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                for (int u : adj_list[v]) {
                    if (!visited[u]) {
                        visited[u] = true;
                        q.emplace(u);
                        components[i].emplace_back(u);
                    }
                }
            }

            int xor_sum = 0;
            for (int v : components[i]) xor_sum ^= (p_assigned[v] + p_unassigned[v] + 1) & 1;

            if (xor_sum) {
                cout << 0;
                exit(0);
            }
        }
    }

    int m = 1e9;
    dynamic_bitset<> events(s);
    for (int i = 0; i < s; i++) {
        int k = components[i].size();
        if (k <= 1) continue;

        vector<int> indices(s, -1);
        for (int j = 0; j < k; j++) indices[components[i][j]] = j;

        vector<array<int, 3>> edges;
        for (int j = 0; j < U; j++) {
            int u = indices[unassigned[j][0]], v = indices[unassigned[j][1]];
            if (~u && ~v) edges.push_back({u, v, 1});
        }
        if (edges.empty()) continue;

        LaminarTree lt(k, edges);
        auto [cut, side] = lt.minimum_cut();
        if (!cut) continue;

        if (m > cut) {
            m = cut;
            dynamic_bitset<> temp(s);
            for (int j = 0; j < k; j++)
                if (side[j]) temp[components[i][j]] = true;
            events = temp;
        }
    }

    if (m == 1e9) {
        cout << "IMPOSSIBLE";
        exit(0);
    }

    dynamic_bitset<> dbs_a(U), dbs_b(U);
    vector<int> cross;
    for (int i = 0; i < U; i++) {
        dbs_a[i] = events[unassigned[i][0]];
        dbs_b[i] = events[unassigned[i][1]];
        if (dbs_a[i] ^ dbs_b[i]) cross.emplace_back(i);
    }

    vector<pair<int, int>> deceive;
    if (!((events.count() ^ (p_assigned & events).count() ^ (dbs_a & dbs_b).count()) & 1) && !cross.empty()) {
        int i = cross[0];
        deceive.emplace_back(unassigned[i][2], (events[unassigned[i][0]] ? unassigned[i][0] : unassigned[i][1]) + 1);
        for (int j : cross)
            if (i != j) deceive.emplace_back(unassigned[j][2], (events[unassigned[j][0]] ? unassigned[j][1] : unassigned[j][0]) + 1);
    } else
        for (int i : cross) deceive.emplace_back(unassigned[i][2], (events[unassigned[i][0]] ? unassigned[i][1] : unassigned[i][0]) + 1);

    cout << m << "\n";
    for (auto [vi, si] : deceive) cout << vi << " " << si << "\n";
}
