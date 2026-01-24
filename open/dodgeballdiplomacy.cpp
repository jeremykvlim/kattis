#include <bits/stdc++.h>
using namespace std;

struct WeightedDisjointSets {
    vector<int> sets, prio, size;
    vector<pair<int, int>> weight;

    WeightedDisjointSets(int n) : sets(n), prio(n), size(n, 1), weight(n, {INT_MAX, 0}) {
        iota(sets.begin(), sets.end(), 0);
        iota(prio.begin(), prio.end(), 0);
        shuffle(prio.begin(), prio.end(), mt19937_64(random_device{}()));
    }

    int & compress(int v) {
        if (sets[v] == v) return sets[v];
        while (weight[sets[v]].first <= weight[v].first) {
            size[sets[v]] -= size[v];
            sets[v] = sets[sets[v]];
        }
        return sets[v];
    }

    int find(int v, int w = INT_MAX - 1) {
        while (weight[v].first <= w) v = compress(v);
        return v;
    }

    void detach(int v) {
        if (sets[v] == v) return;
        detach(sets[v]);
        size[sets[v]] -= size[v];
    }

    int attach(int v, int w = INT_MAX - 1) {
        while (weight[v].first <= w) {
            size[sets[v]] += size[v];
            v = sets[v];
        }
        return v;
    }

    void link(int u, int v, pair<int, int> w) {
        detach(u);
        detach(v);
        while (u != v) {
            u = attach(u, w.first);
            v = attach(v, w.first);
            if (prio[u] < prio[v]) swap(u, v);
            swap(sets[v], u);
            swap(weight[v], w);
        }
        attach(u);
    }

    void cut(int v, int w) {
        while (sets[v] != v) {
            if (weight[v].first == w) {
                for (int u = v; u != sets[u]; size[u = sets[u]] -= size[v]);
                sets[v] = v;
                weight[v] = {INT_MAX, 0};
                return;
            }
            v = compress(v);
        }
    }

    void cut(int u, int v, int w) {
        cut(u, w);
        cut(v, w);
    }

    int path_max(int u, int v) {
        if (find(u) != find(v)) return -1;

        for (;;) {
            if (weight[u].first > weight[v].first) swap(u, v);
            if (sets[u] == v) return u;
            u = sets[u];
        }
    }

    int unite(int u, int v, pair<int, int> w) {
        if (u != v) {
            int t = path_max(u, v);
            if (t == -1) {
                link(u, v, w);
                return -1;
            } else if (weight[t].first > w.first) {
                int i = weight[t].second;
                cut(t, weight[t].first);
                link(u, v, w);
                return i;
            }
        }
        return w.second;
    }

    int component_size(int v) {
        return size[find(v)];
    }
};

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
            return ml += mr;
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
    WeightedDisjointSets wdsu(n + 1);
    while (q--) {
        char c;
        cin >> c;

        if (c == 'a') {
            int u, v, p;
            cin >> u >> v >> p;

            edges[e] = {u, v, p};
            state[e] = 1;

            int su = wdsu.component_size(u), sv = wdsu.component_size(v), i = wdsu.unite(u, v, {p, e});
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
            int temp = wdsu.component_size(u);
            wdsu.cut(u, v, p);
            int su = wdsu.component_size(u), sv = wdsu.component_size(v);
            update(temp, -1);
            update(su, 1);
            update(sv, 1);
        } else cout << st[1].pos << "\n" << flush;
    }
}
