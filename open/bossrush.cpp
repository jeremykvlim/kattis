#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct AntiMonopolyTree {
    vector<int> parent, size;
    vector<T> weight;

    AntiMonopolyTree(int n) : parent(n, -1), size(n, 1), weight(n, numeric_limits<T>::max()) {}

    pair<T, int> path_max(int u, int v) {
        upward_maintain(u);
        upward_maintain(v);

        T max_w = numeric_limits<T>::min();
        int t = -1;
        while (u != v) {
            if (size[u] > size[v]) swap(u, v);
            if (weight[u] == numeric_limits<T>::max()) return {numeric_limits<T>::max(), -1};
            if (max_w < weight[u]) {
                max_w = weight[u];
                t = u;
            }
            u = parent[u];
        }
        return {max_w, t};
    }

    bool connected(int u, int v) {
        return u == v || path_max(u, v).second != -1;
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
            if (weight[v] < weight[p]) {
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
        weight[v] = numeric_limits<T>::max();
    }

    bool add(int u, int v, T w) {
        if (u == v) return false;

        auto [max_w, t] = path_max(u, v);
        bool merged = max_w == numeric_limits<T>::max();
        if (!merged) {
            if (w >= max_w) return false;
            cut(t);
        }

        int du = 0, dv = 0;
        while (~u && ~v) {
            if (w >= weight[u]) {
                int p = parent[u];
                if (~p) size[p] += du;
                u = p;
            } else if (w >= weight[v]) {
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
        return merged;
    }

    bool remove(int u, int v, T w) {
        auto [max_w, t] = path_max(u, v);
        if (max_w != w) return false;

        cut(t);
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, w, d;
    cin >> n >> w >> d;

    vector<int> f(n);
    for (int &fi : f) cin >> fi;

    int r = w % d;
    vector<int> deduped{0};
    for (int fi : f) {
        deduped.emplace_back(fi);
        deduped.emplace_back((fi + r) % d);
    }
    sort(deduped.begin(), deduped.end());
    deduped.erase(unique(deduped.begin(), deduped.end()), deduped.end());

    int m = deduped.size();
    vector<int> pos_l(n), pos_r(n);
    for (int i = 0; i < n; i++) {
        pos_l[i] = lower_bound(deduped.begin(), deduped.end(), f[i]) - deduped.begin();
        pos_r[i] = lower_bound(deduped.begin(), deduped.end(), (f[i] + r) % d) - deduped.begin();
    }

    vector<int> diff(m + 1, 0);
    auto add = [&](int l, int r) {
        if (l >= r) return;
        diff[l]++;
        diff[r]--;
    };

    if (r)
        for (int i = 0; i < n; i++) {
            if (pos_l[i] < pos_r[i]) add(pos_l[i], pos_r[i]);
            else {
                add(pos_l[i], m);
                add(0, pos_r[i]);
            }
        }

    vector<int> pref(m);
    pref[0] = diff[0];
    for (int i = 1; i < m; i++) pref[i] = pref[i - 1] + diff[i];

    AntiMonopolyTree<long long> amt(m);
    for (int i = 0; i < n; i++) amt.add(pos_l[i], pos_r[i], -1);

    auto base = 0LL;
    for (int i = 0; i < m; i++) {
        int t = i + 1 < m ? deduped[i + 1] : d;
        base = max(base, (long long) d * (pref[i] - 1) + t);
        amt.add(i, (i + 1) % m, (long long) d * pref[i] + t);
    }

    auto bottleneck = 0LL;
    for (int v = 0; v < m; v++)
        if (~amt.parent[v]) bottleneck = max(bottleneck, amt.weight[v]);
    cout << max(base, bottleneck) + (long long) n * (w - r) - w;
}