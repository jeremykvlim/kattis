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

vector<pair<int, int>> monotone_jumps(const vector<int> &next, const vector<pair<int, int>> &queries) {
    int n = next.size();
    vector<vector<int>> adj_list(n + 1);
    vector<int> depth(n + 1, 0);
    for (int v = 0; v < n; v++) adj_list[next[v]].emplace_back(v);
    for (int v = n - 1; ~v; v--) depth[v] = depth[next[v]] + 1;

    DisjointSets dsu(n + 1);
    vector<int> rep(n + 1);
    iota(rep.begin(), rep.end(), 0);

    int q = queries.size();
    vector<pair<int, int>> result(q);
    for (int i = 0, v = 0; i < q; i++) {
        auto [a, bound] = queries[i];
        for (; v <= bound; v++)
            for (int u : adj_list[v]) {
                auto [big, small] = dsu.unite(u, v);
                if (small != -1) rep[big] = max(rep[big], rep[small]);
            }

        int b = rep[dsu.find(a)];
        result[i] = {depth[a] - depth[b], b};
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, c1, c2;
    cin >> n >> m >> c1 >> c2;

    int left = c1 / 2, right = c2 / 2, len = right - left, cycle_len = 2 * len + 2;
    vector<vector<vector<int>>> bounds(2, vector<vector<int>>(2, vector<int>(n / 2, INT_MAX)));
    vector<array<int, 3>> pending;
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;

        if (!((a ^ b) & 1)) {
            int x = (a - 1) / 2, y = (b - 1) / 2, side = !(a & 1);
            if (max(x, y) <= left) {
                auto [dl, dr] = minmax({left - x, left - y});
                bounds[0][side][dl + 1] = min(bounds[0][side][dl + 1], dr);
            } else if (min(x, y) >= right) {
                auto [dl, dr] = minmax({x - right, y - right});
                bounds[1][side][dl + 1] = min(bounds[1][side][dl + 1], dr);
            } else pending.push_back({a, b, -1});
        } else {
            int c;
            cin >> c;

            pending.push_back({a, b, c});
        }
    }

    vector<vector<int>> dist(2, vector<int>(2, INT_MAX));
    vector<pair<int, int>> base;
    for (int end = 0; end < 2; end++)
        for (int side = 0; side < 2; side++)
            for (int d = (!end ? left : n / 2 - 1 - right); d; d--)
                if (dist[end][side] > bounds[end][side][d]) {
                    dist[end][side] = d;
                    int h = 2 * (!end ? left - d : right + d - 1) + 1 + side;
                    base.emplace_back(h, h + 2);
                }

    array<int, 2> skip{0, 0};
    for (int end = 0; end < 2; end++)
        for (int side = 0; side < 2; side++)
            if (dist[end][side] == 1) skip[end] |= 1 << side;

    vector<array<int, 4>> intervals;
    for (auto [a, b, c] : pending) {
        int x = (a - 1) / 2, y = (b - 1) / 2, ea = x < left ? 0 : (x > right ? 1 : -1), eb = y < left ? 0 : (y > right ? 1 : -1);
        if ((ea >= 0 && max({0, x - right, left - x}) >= dist[ea][!(a & 1)]) || (eb >= 0 && max({0, y - right, left - y}) >= dist[eb][!(b & 1)])) continue;

        array<int, 2> required{0, 0};
        if (ea >= 0) required[ea] |= 1 << !(a & 1);
        if (eb >= 0) required[eb] |= 1 << !(b & 1);

        int u = clamp(x, left, right) - left, v = clamp(y, left, right) - left;
        if (!(a & 1)) u = cycle_len - 1 - u;
        if (!(b & 1)) v = cycle_len - 1 - v;

        int l, r;
        if (!~c) {
            l = min(u, v);
            r = max(u, v) - 1;
        } else {
            int odd = a & 1 ? u : v, even = a & 1 ? v : u;
            l = c == c2 ? odd : even;
            r = c == c2 ? even - 1 : odd + cycle_len - 1;
        }
        intervals.push_back({l, r, required[0], required[1]});
    }

    vector<pair<int, int>> queries(cycle_len);
    for (int i = 0; i < cycle_len; i++) queries[i] = {i, i + cycle_len - 1};

    int L = INT_MAX, total = 2 * cycle_len;
    vector<pair<int, int>> devices;
    vector<int> cycle(total + 1), next(total);
    for (int ml = 0; ml < 4; ml++)
        for (int mr = 0; mr < 4; mr++)
            if (!(ml & skip[0]) && !(mr & skip[1])) {
                int count = base.size() + popcount((unsigned) ml) + popcount((unsigned) mr);
                if (L > count) {
                    fill(cycle.begin(), cycle.end(), total);
                    bool active = false;
                    for (auto [l, r, rl, rr] : intervals)
                        if (!(ml & rl) && !(mr & rr)) {
                            cycle[l] = min(cycle[l], r);
                            cycle[l + cycle_len] = min(cycle[l + cycle_len], r + cycle_len);
                            active = true;
                        }

                    int extra = 0, j = 0;
                    if (active) {
                        if (L <= count + 1) continue;
                        next[total - 1] = total;
                        for (int i = total - 2; ~i; i--) next[i] = min(next[i + 1], cycle[i + 1]);

                        auto result = monotone_jumps(next, queries);
                        extra = INT_MAX;
                        for (int i = 0; i < cycle_len; i++)
                            if (extra > result[i].first + 1) {
                                extra = result[i].first + 1;
                                j = i;
                            }
                    }

                    if (L > count + extra) {
                        L = count + extra;
                        devices = base;
                        for (int end = 0; end < 2; end++)
                            for (int side = 0; side < 2; side++)
                                if (((!end ? ml : mr) >> side) & 1) {
                                    int h = 2 * (!end ? left - 1 : right) + 1 + side;
                                    devices.emplace_back(h, h + 2);
                                }

                        for (int _ = 0; _ < extra; _++, j = next[j]) {
                            int i = j % cycle_len;
                            if (i == len || i == cycle_len - 1) {
                                int h = i == len ? c2 : c1;
                                devices.emplace_back(h, h + 1);
                            } else {
                                int h = i < len ? c1 + 2 * i : c2 + 1 - 2 * (i - len);
                                devices.emplace_back(h, h + 2);
                            }
                        }
                    }
                }
            }

    cout << L << "\n";
    for (auto [a, b] : devices) cout << a << " " << b << "\n";
}
