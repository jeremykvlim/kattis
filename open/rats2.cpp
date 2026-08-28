#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    auto bit = [&](int b) {
        return ((__int128) 1) << b;
    };

    auto set = [&](__int128 &mask, int i) {
        mask |= bit(i);
    };

    auto reset = [&](__int128 &mask, int i) {
        mask &= ~bit(i);
    };

    auto test = [&](__int128 mask, int i) {
        return (mask >> i) & 1;
    };

    __int128 rats = 0, katts = 0;
    for (int _ = 0; _ < k; _++) {
        int r;
        cin >> r;

        set(rats, r - 1);
    }
    for (int _ = 0; _ < k; _++) {
        int t;
        cin >> t;

        set(katts, t - 1);
    }

    auto place = [&](__int128 mask) {
        if (!mask) return;

        cout << popcount((unsigned __int128) mask) << " ";
        for (int s = 0; s < n; s++)
            if (test(mask, s)) cout << s + 1 << " ";
        cout << "\n" << flush;

        rats = 0;
        for (int _ = 0; _ < k; _++) {
            int r;
            cin >> r;

            set(rats, r - 1);
        }

        if (rats == katts) {
            cout << "activate!";
            exit(0);
        }
    };

    __int128 used = 0, all = (((__int128) 1) << n) - 1;
    vector<__int128> adj_masks(n, 0);
    while (used != all) {
        auto node = [&]() {
            for (int v = 0; v < n; v++)
                if (test(rats, v) && !test(used, v)) return v;
            return -1;
        };
        int v = node();
        if (!~v) place(used);

        __int128 visited = 0;
        stack<__int128> st;
        st.emplace(rats);
        while (!~v) {
            visited |= rats;
            place(visited);

            if (rats == st.top()) {
                if (st.size() == 1) place(rats);
                else {
                    st.pop();
                    place(all & ~st.top());
                }
            } else st.emplace(rats);

            v = node();
        }

        for (;;) {
            while (test(rats, v)) {
                auto temp = rats;
                place(bit(v) | (adj_masks[v] & ~rats));
                if (rats == temp)
                    while (test(rats, v)) place(bit(v));
            }

            auto temp = rats;
            place(all & ~(bit(v) | (adj_masks[v] & rats)));
            temp &= ~rats;
            int u = 0;
            for (; u < n && !test(temp, u); u++);
            if (u == n || test(adj_masks[v], u)) break;
            set(adj_masks[v], u);
        }
        set(used, v);
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (!test(adj_masks[j], i)) reset(adj_masks[i], j);

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (test(adj_masks[i], j) && (adj_masks[i] & adj_masks[j] & (bit(i) - 1))) {
                reset(adj_masks[i], j);
                reset(adj_masks[j], i);
            }

    vector<int> dist(n);
    auto dfs = [&](auto &&self, __int128 component, __int128 rats, __int128 known = 0) -> vector<__int128> {
        auto component_mask = [&](__int128 allowed, int src) {
            auto c = bit(src);
            queue<int> q;
            q.emplace(src);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                auto mask = adj_masks[v] & allowed & ~c;

                for (int u = 0; u < n; u++)
                    if (test(mask, u)) {
                        set(c, u);
                        q.emplace(u);
                    }
            }
            return c;
        };

        int centroid = -1;
        for (int v = 0, size = n + 1; v < n; v++) {
            if (!test(component, v) || test(katts, v)) continue;

            int s = 0;
            for (int u = 0; u < n; u++)
                if (test(component, u) && test(adj_masks[v], u)) s = max(s, popcount((unsigned __int128) component_mask(component & ~bit(v), u)));

            if (size > s) {
                size = s;
                centroid = v;
            }
        }
        if (!~centroid) return {};

        vector<int> starts;
        vector<__int128> components;
        for (int v = 0; v < n; v++)
            if (test(component, v) && test(adj_masks[centroid], v)) {
                starts.emplace_back(v);
                components.emplace_back(component_mask(component & ~bit(centroid), v));
            }

        int m = starts.size();
        vector<int> delta(m);
        vector<__int128> owls;
        for (;;) {
            for (int i = 0; i < m; i++) delta[i] = popcount((unsigned __int128) (components[i] & rats)) - popcount((unsigned __int128) (components[i] & katts));

            auto predict = [&](__int128 query) {
                fill(dist.begin(), dist.end(), -1);
                queue<int> q;
                for (int i = 0; i < n; i++)
                    if (test(query, i)) {
                        dist[i] = 0;
                        q.emplace(i);
                    }

                while (!q.empty()) {
                    int v = q.front();
                    q.pop();

                    for (int u = 0; u < n; u++)
                        if (test(adj_masks[v], u) && test(component, u) && !~dist[u]) {
                            dist[u] = dist[v] + 1;
                            q.emplace(u);
                        }
                }

                auto r = rats;
                for (int v = 0; v < n; v++)
                    if (test(rats, v)) {
                        int t = v;
                        for (int u = 0; u < n; u++) {
                            if (!test(adj_masks[v], u) || !test(component, u) || test(r, u)) continue;
                            if (dist[u] > dist[t] || (dist[u] == dist[t] && t != v && t > u)) t = u;
                        }

                        reset(r, v);
                        set(r, t);
                    }
                return r;
            };

            if (test(rats, centroid)) {
                int s = -1;
                for (int i = 0; i < m; i++)
                    if (delta[i] < 0) s = starts[i];
                if (!~s) return owls;

                auto c = component;
                if (!test(rats, s)) {
                    reset(c, s);
                    for (int u = 0; u < n; u++)
                        if (u != centroid && test(component, u) && test(adj_masks[s], u) && test(rats, u)) reset(c, u);
                } else {
                    __int128 mask = 0;
                    queue<pair<int, int>> q;
                    q.emplace(s, centroid);
                    while (!q.empty()) {
                        auto [v, p] = q.front();
                        q.pop();

                        if (test(rats, v))
                            for (int u = 0; u < n; u++)
                                if (u != p && test(component, u) && test(adj_masks[v], u) && !test(rats, u) && !test(mask, u)) {
                                    set(mask, u);
                                    reset(c, u);
                                    break;
                                }

                        if (test(mask, v))
                            for (int u = 0; u < n; u++)
                                if (u != p && test(component, u) && test(adj_masks[v], u) && test(rats, u)) reset(c, u);

                        for (int u = 0; u < n; u++)
                            if (u != p && test(component, u) && test(adj_masks[v], u)) q.emplace(u, v);
                    }
                }

                owls.emplace_back(c);
                rats = predict(c | known);
                continue;
            }

            int k = -1;
            for (int i = 0; i < m; i++)
                if (delta[i] > 0) {
                    k = i;
                    break;
                }

            if (~k) {
                int s = starts[k];
                auto c = component;
                if (test(rats, s)) {
                    reset(c, centroid);
                    for (int u = 0; u < n; u++)
                        if (u != s && test(component, u) && test(adj_masks[centroid], u) && test(rats, u)) reset(c, u);
                } else {
                    __int128 visited = bit(s), holes = 0, moved = 0;
                    queue<int> q;
                    q.emplace(s);
                    while (!q.empty()) {
                        int v = q.front();
                        q.pop();

                        bool active = adj_masks[v] & (holes | moved);
                        for (int u = 0; u < n; u++)
                            if (test(components[k], u) && test(adj_masks[v], u) && !test(visited, u)) {
                                set(visited, u);
                                if (!active && test(rats, u) && !test(rats, v) && !test(moved, u)) {
                                    active = true;
                                    set(holes, v);
                                    set(moved, u);
                                }
                                q.emplace(u);
                            }
                    }

                    c &= ~holes;
                    for (int u = 0; u < n; u++)
                        if (test(components[k], u) && test(rats, u) && !test(moved, u)) reset(c, u);
                }

                owls.emplace_back(c);
                rats = predict(c | known);
                continue;
            }
            set(known, centroid);

            auto temp = known;
            for (int i = owls.size(), len = 0; auto c : components) {
                auto o = self(self, c, rats & c, known);

                if (len < o.size()) {
                    len = o.size();
                    owls.resize(i + len, temp);
                }
                for (int j = 0; j < len; j++) owls[i + j] |= j < o.size() ? o[j] : c;
                temp |= c;
            }
            return owls;
        }
    };
    for (auto c : dfs(dfs, all, rats)) place(c);
    cout << "activate!";
}
