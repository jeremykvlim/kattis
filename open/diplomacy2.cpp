#include <bits/stdc++.h>
using namespace std;

struct PURQSegmentTree {
    struct Monoid {
        int count, pos, neg;

        Monoid() : count(0), pos(0), neg(0) {}

        auto & operator=(const pair<int, int> &v) {
            count = v.first;
            pos = (v.first & 1) * v.second;
            neg = -pos;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            count += monoid.count;
            int temp1 = (monoid.count & 1 ? neg : pos) + monoid.pos, temp2 = (monoid.count & 1 ? pos : neg) + monoid.neg;
            pos = temp1;
            neg = temp2;
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            ml += mr;
            return ml;
        }
    };

    int n;
    vector<Monoid> ST;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void point_update(int i, const pair<int, int> &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    Monoid range_query(int l, int r) {
        Monoid ml, mr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ml = ml + ST[l++];
            if (r & 1) mr = ST[--r] + mr;
        }

        return ml + mr;
    }

    auto & operator[](int i) {
        return ST[i];
    }

    PURQSegmentTree(int n) : n(n), ST(2 * n) {}
};

struct AntiMonopolyTree {
    vector<int> parent, size;
    vector<pair<int, int>> weight;

    AntiMonopolyTree(int n) : parent(n, -1), size(n, 1), weight(n, {INT_MAX, -1}) {}

    pair<int, int> path_max(int u, int v) {
        upward_maintain(u);
        upward_maintain(v);

        int max_w = INT_MIN, t = -1;
        while (u != v) {
            if (size[u] > size[v]) swap(u, v);
            if (weight[u].first == INT_MAX) return {INT_MAX, -1};
            if (max_w < weight[u].first) {
                max_w = weight[u].first;
                t = u;
            }
            u = parent[u];
        }
        return {max_w, t};
    }

    void upward_maintain(int v) {
        while (~parent[v]) {
            int p = parent[v];
            if (3 * size[v] <= 2 * size[p]) {
                v = p;
                continue;
            }

            size[p] -= size[v];
            parent[v] = parent[p];
            if (weight[v].first < weight[p].first) {
                size[v] += size[p];
                swap(weight[v], weight[p]);
                parent[p] = v;
            }
        }
    }

    int root(int v) {
        while (~parent[v]) v = parent[v];
        return v;
    }

    void cut(int v) {
        for (int p = parent[v]; ~p; p = parent[p]) size[p] -= size[v];
        parent[v] = -1;
        weight[v] = {INT_MAX, -1};
    }

    int add(int u, int v, pair<int, int> w) {
        if (u == v) return w.second;

        auto [max_w, t] = path_max(u, v);
        int i = -1;
        if (max_w != INT_MAX) {
            if (w.first >= max_w) return w.second;
            i = weight[t].second;
            cut(t);
        }

        int du = 0, dv = 0;
        while (~u && ~v) {
            if (w.first >= weight[u].first) {
                int p = parent[u];
                if (~p) size[p] += du;
                u = p;
            } else if (w.first >= weight[v].first) {
                int p = parent[v];
                if (~p) size[p] += dv;
                v = p;
            } else {
                if (size[u] > size[v]) {
                    swap(u, v);
                    swap(du, dv);
                }

                du -= size[u];
                dv += size[u];
                size[v] += size[u];
                w = exchange(weight[u], w);
                u = exchange(parent[u], v);
                if (~u) size[u] += du;
            }
        }

        if (~v)
            for (v = parent[v]; ~v; v = parent[v]) size[v] += dv;
        return i;
    }

    bool remove(int u, int v, int w) {
        auto [max_w, t] = path_max(u, v);
        if (max_w != w) return false;

        cut(t);
        return true;
    }

    int component_size(int v) {
        upward_maintain(v);
        return size[root(v)];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> delta(n + 1, 0);
    PURQSegmentTree st(bit_ceil((unsigned) n + 1));
    auto update = [&](int size, int d) {
        st.point_update(size, {delta[size] += d, size});
    };
    update(1, n);

    int e = 0;
    vector<array<int, 3>> edges(q, {-1, -1, 0});
    vector<int> state(q + 1, 0);
    priority_queue<pair<int, int>> pq;
    AntiMonopolyTree amt(n + 1);
    while (q--) {
        char c;
        cin >> c;

        if (c == 'a') {
            int u, v, p;
            cin >> u >> v >> p;

            edges[e] = {u, v, p};
            state[e] = 1;

            int su = amt.component_size(u), sv = amt.component_size(v), i = amt.add(u, v, {p, e});
            pq.emplace(p, e);
            if (i == -1) {
                update(su, -1);
                update(sv, -1);
                update(su + sv, 1);
                state[e] = 2;
            } else if (i == e) state[e] = 1;
            else {
                state[i] = 1;
                state[e] = 2;
            }
            e++;
        } else if (c == 'r') {
            while (!pq.empty() && !state[pq.top().second]) pq.pop();
            if (pq.empty()) continue;

            int i = pq.top().second;
            pq.pop();
            if (!state[i]) continue;

            int s = state[i];
            state[i] = 0;
            if (s != 2) continue;

            auto [u, v, p] = edges[i];
            int temp = amt.component_size(u);
            amt.remove(u, v, p);
            int su = amt.component_size(u), sv = amt.component_size(v);
            update(temp, -1);
            update(su, 1);
            update(sv, 1);
        } else cout << st[1].pos << "\n" << flush;
    }
}