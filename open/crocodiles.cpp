#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        while (sets[v] >= 0) {
            int p = sets[v];
            if (sets[p] >= 0) sets[v] = sets[p];
            v = p;
        }
        return v;
    }

    pair<int, int> unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return {u_set, -1};

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return {u_set, v_set};
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1) {}
};

struct SplayTree {
    struct SplayNode {
        array<int, 3> family{};
        int count, base, aggregate;

        SplayNode() : family{0, 0, 0}, count(0), base(0), aggregate(0) {}
    };

    vector<SplayNode> ST;

    SplayTree(int n) : ST(n + 1) {}

    auto & operator[](int i) {
        return ST[i];
    }

    void pull(int i) {
        if (!i) return;
        auto [l, r, p] = ST[i].family;
        ST[i].count = !!ST[i].base + ST[l].count + ST[r].count;
        ST[i].aggregate = max({ST[i].base, ST[l].aggregate, ST[r].aggregate});
    }

    void splay(int i) {
        auto root = [&](int i) {
            auto [l, r, p] = ST[ST[i].family[2]].family;
            return !i || l != i && r != i;
        };

        auto child = [&](int i, int parent) { return ST[parent].family[1] == i; };

        auto rotate = [&](int i) {
            int j = ST[i].family[2], k = ST[j].family[2];
            if (!root(j)) ST[k].family[child(j, k)] = i;

            int c = child(i, j), s = ST[j].family[c] = ST[i].family[c ^ 1];
            if (s) ST[s].family[2] = j;

            ST[i].family[c ^ 1] = j;
            ST[i].family[2] = k;
            ST[j].family[2] = i;
            pull(j);
        };

        while (!root(i)) {
            int j = ST[i].family[2], k = ST[j].family[2];
            if (!root(j)) rotate(child(i, j) != child(j, k) ? i : j);
            rotate(i);
        }
        pull(i);
    }

    int subtree_min(int i) {
        while (ST[i].family[0]) i = ST[i].family[0];
        return i;
    }
};

struct RootedLinkCutTree : SplayTree {
    RootedLinkCutTree(int n) : SplayTree(n) {}

    void access(int i) {
        for (int u = 0, v = i; v; u = v, v = ST[v].family[2]) {
            splay(v);
            ST[v].family[1] = u;
            pull(v);
        }
        splay(i);
    }

    int find(int i) {
        access(i);
        i = subtree_min(i);
        splay(i);
        return i;
    }

    void link(int i, int j) {
        access(i);
        ST[i].family[2] = j;
    }

    void cut(int i) {
        access(i);
        auto &[l, r, p] = ST[i].family;
        l = ST[l].family[2] = 0;
        pull(i);
    }

    void update(int i, int j, int base) {
        link(i, j);
        ST[i].base = base;
        pull(i);
    }

    pair<int, int> query(int i, int v, int k) {
        access(i);
        k = min(k, ST[i].count);
        if (!k) return {i - 1, 0};

        int j = 0, used = 0;
        while (i) {
            int r = ST[i].family[1], c = ST[r].count;
            if (k <= c) {
                i = r;
                continue;
            }

            if (ST[r].aggregate >= v) {
                k = c;
                i = r;
                continue;
            }

            used += c;
            k -= c;
            if (ST[i].base) {
                used++;
                if (ST[i].base >= v || k == 1) {
                    j = i;
                    break;
                }
                k--;
            }
            i = ST[i].family[0];
        }
        splay(j);
        return {ST[j].base, used};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<long long> pref(n + 1, 0);
    for (int i = 0; i < n; i++) {
        int a;
        cin >> a;

        pref[i + 1] = pref[i] + a;
    }

    vector<array<int, 2>> links(2 * n + 1, {0, 0});
    for (int v = n; v <= 2 * n; v++) links[v] = {v, v};

    set<int> active;
    auto lb = [&](int x) {
        auto it = active.lower_bound(x);
        return it == active.end() ? -1 : *it;
    };

    auto ub = [&](int x) {
        auto it = active.upper_bound(x);
        return it == active.begin() ? -1 : *prev(it);
    };

    RootedLinkCutTree lct(n + 1);
    vector<int> reach(n);
    map<int, int> reach_inv;
    auto update = [&](int v, int p, bool head) {
        lct.update(v + 1, p + 1, head && p != n ? reach[p] : 0);
    };

    auto remove = [&](int v) {
        int p = ub(reach[v]);
        if (p == v) p = n;
        int prev = links[v][0], next = links[v][1], s = p + n;
        lct.cut(v + 1);
        if (next != s) lct.cut(next + 1);
        links[prev][1] = next;
        links[next][0] = prev;
        if (next != s) update(next, prev == s ? p : prev, prev == s);
    };

    auto get = [&](int p, int head) -> pair<int, int> {
        int prev = links[head][0], s = p + n, tail = links[s][0];
        lct.cut(head + 1);
        if (prev == s) links[s] = {s, s};
        else {
            links[prev][1] = s;
            links[s][0] = prev;
        }
        return {head, tail};
    };

    DisjointSets dsu(n);
    vector<int> rep(n, -1);
    auto activate = [&](int v) {
        rep[v] = v;
        auto merge = [&](int u, int v) {
            auto [big, small] = dsu.unite(u, v);
            rep[big] = min(rep[big], rep[small]);
        };
        if (~rep[v - 1]) merge(v, v - 1);
        if (v + 1 < n && ~rep[v + 1]) merge(v, v + 1);

        int u = ub(v);
        pair<int, int> chain{-1, -1};
        auto extend = [&](const pair<int, int> &c) {
            if (chain == make_pair(-1, -1)) chain = c;
            else {
                links[chain.second][1] = c.first;
                links[c.first][0] = chain.second;
                update(c.first, chain.second, false);
                chain.second = c.second;
            }
        };
        if (u >= 0) {
            if (reach[u] >= reach[v]) return;
            int t = lb(u + 1);
            auto it = reach_inv.lower_bound(v);
            if (it != reach_inv.end() && (t < 0 || it->first < t) && it->second != u) extend(get(u, it->second));
            if (reach[u] >= v && (t < 0 || reach[u] < t)) {
                remove(u);
                extend({u, u});
            }
        }

        auto it_l = active.lower_bound(v), it_r = it_l;
        for (; it_r != active.end() && reach[*it_r] <= reach[v]; it_r++);

        for (auto it = it_l; it != it_r; it++) {
            int t = *it, s = t + n, head = links[s][1];
            if (head != s) extend(get(t, head));
            remove(t);
            reach_inv.erase(reach[t]);
        }
        active.erase(it_l, it_r);
        active.emplace(v);
        reach_inv[reach[v]] = v;

        if (chain.first >= 0) {
            int s = v + n;
            links[s] = {chain.second, chain.first};
            links[chain.first][0] = links[chain.second][1] = s;
            update(chain.first, v, true);
        }

        int p = ub(reach[v]);
        if (p == v) p = n;
        else {
            int t = lb(p + 1);
            auto it = reach_inv.upper_bound(reach[v]);
            if (it != reach_inv.end() && (t < 0 || it->first < t) && it->second != p) {
                int s = p + n, next = it->second, prev = links[next][0];
                lct.cut(next + 1);
                links[v] = {prev, next};
                links[prev][1] = links[next][0] = v;
                update(v, prev == s ? p : prev, prev == s);
                update(next, v, false);
                return;
            }
        }
        int s = p + n, prev = links[s][0];
        links[v] = {prev, s};
        links[prev][1] = links[s][0] = v;
        update(v, prev == s ? p : prev, prev == s);
    };

    vector<vector<int>> sweep(n);
    for (int i = n - 1, temp = k; ~i; i--, k = temp) {
        for (int v : sweep[i]) activate(v);

        int v = i + 1, w = i + 1;
        while (k && v < n) {
            if (~rep[w]) {
                int w_set = dsu.find(w);
                w = rep[w_set] + dsu.size(w_set);
            }

            int u = ub(v);
            if (v < w) {
                if (u < 0 || reach[u] <= v) break;

                v = reach[u];
                if (--k && v < w) {
                    auto [t, used] = lct.query(u + 1, w, k);
                    if (used) v = t;
                    k -= used;
                }
                if (v < w) break;
            } else {
                int t = u < 0 ? v : max(v, reach[u]);
                auto d = 2 * pref[!~rep[v] ? v : rep[dsu.find(v)] - 1] - pref[i];
                if (t < n && d > pref[t + 1]) t = lower_bound(pref.begin(), pref.end(), d) - pref.begin() - 1;
                if (t == v) break;

                w = v + 1;
                v = t;
                k--;
            }
        }

        reach[i] = v;
        auto d = 2 * pref[i] - pref[v];
        if (d > 0) sweep[lower_bound(pref.begin(), pref.begin() + i, d) - pref.begin() - 1].emplace_back(i);
    }

    int count = 0;
    for (int i = 0, v = 0, u = 0; i < n; i++) {
        v = max(v, reach[i]);
        if (u == i) {
            u = v;
            count++;
        }
    }
    cout << count;
}
