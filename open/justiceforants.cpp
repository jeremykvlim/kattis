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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q, k;
    cin >> n >> q >> k;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    vector<vector<int>> adj_list(n + 1);
    for (int i = 2; i <= n; i++) {
        int p;
        cin >> p;

        adj_list[p].emplace_back(i);
    }

    auto lsb = [&](int x) {
        return x & -x;
    };

    vector<pair<int, int>> tour;
    vector<int> child(n + 1, 0), depth(n + 1, 0), inlabel(n + 1), ascendant(n + 1, 0), head(n + 2);
    auto dfs = [&](auto &&self, int v = 1, int prev = 1) -> void {
        tour.emplace_back(v, prev);
        inlabel[v] = tour.size();

        for (int u : adj_list[v])
            if (u != prev) {
                depth[u] = depth[v] + 1;
                self(self, u, v);
                head[inlabel[u]] = v;
                if (lsb(inlabel[v]) < lsb(inlabel[u])) inlabel[v] = inlabel[u];
            }
    };
    dfs(dfs);

    for (auto [v, p] : tour) {
        ascendant[v] = ascendant[p] | lsb(inlabel[v]);
        if (v != 1 && inlabel[v] == inlabel[p]) child[p] = v;
    }

    auto lca = [&](int u, int v) -> int {
        if (unsigned above = inlabel[u] ^ inlabel[v]; above) {
            above = (ascendant[u] & ascendant[v]) & -bit_floor(above);
            if (unsigned below = ascendant[u] ^ above; below) {
                below = bit_floor(below);
                u = head[(inlabel[u] & -below) | below];
            }
            if (unsigned below = ascendant[v] ^ above; below) {
                below = bit_floor(below);
                v = head[(inlabel[v] & -below) | below];
            }
        }

        return depth[u] < depth[v] ? u : v;
    };

    vector<int> base{0}, offset(n + 1), pos(n + 1);
    for (auto [v, p] : tour)
        if (v == 1 || inlabel[v] != inlabel[p]) {
            offset[inlabel[v]] = base.size();
            for (int u = v; u; u = child[u]) {
                base.emplace_back(u);
                pos[u] = base.size() - 1;
            }
        }

    auto path_up = [&](int v, int a) {
        vector<array<int, 2>> path;
        for (; inlabel[v] != inlabel[a]; v = head[inlabel[v]]) path.push_back({pos[v], offset[inlabel[v]]});
        path.push_back({pos[v], pos[a]});
        return path;
    };

    auto path_down = [&](int v, int a) {
        vector<array<int, 2>> path;
        for (; inlabel[v] != inlabel[a]; v = head[inlabel[v]]) path.push_back({offset[inlabel[v]], pos[v]});
        if (v != a) path.push_back({pos[a] + 1, pos[v]});
        reverse(path.begin(), path.end());
        return path;
    };

    auto decompose = [&](int u, int v) {
        int a = lca(u, v);
        auto path = path_up(u, a);
        for (auto p : path_down(v, a)) path.emplace_back(p);
        return path;
    };

    int lg = __lg(n);
    vector<DisjointSets> dsus(lg + 1, DisjointSets(2 * (n + 1)));
    vector<vector<int>> blocks(lg + 1);

    auto encode = [&](int dir, int p) {
        return dir * (n + 1) + p;
    };

    auto add = [&](int i, int dir1, int p1, int dir2, int p2) {
        int u = encode(dir1, p1), v = encode(dir2, p2);
        auto [big, small] = dsus[i].unite(u, v);
        if (small != -1 && i) blocks[i].emplace_back(small);
    };

    while (q--) {
        int u, v, x, y;
        cin >> u >> v >> x >> y;

        auto path1 = decompose(u, v), path2 = decompose(x, y);
        auto [l1, r1] = path1[0];
        auto [l2, r2] = path2[0];
        int sgn1 = l1 <= r1 ? 1 : -1, sgn2 = l2 <= r2 ? 1 : -1, len1 = abs(l1 - r1) + 1, len2 = abs(l2 - r2) + 1;
        for (int p1 = 0, p2 = 0; p1 < path1.size() && p2 < path2.size();) {
            int len = min(len1, len2), i = __lg(len), half = 1 << i,
                r3 = l1 + (len - 1) * sgn1, r4 = l2 + (len - 1) * sgn2, sgn3 = l1 <= r3 ? 1 : -1, sgn4 = l2 <= r4 ? 1 : -1;
            add(i, sgn3 < 0, l1, sgn4 < 0, l2);
            if (len != half) add(i, sgn3 < 0, l1 + (len - half) * sgn3, sgn4 < 0, l2 + (len - half) * sgn4);
            len1 -= len;
            len2 -= len;

            if (!len1) {
                if (++p1 < path1.size()) {
                    l1 = path1[p1][0];
                    r1 = path1[p1][1];
                    sgn1 = l1 <= r1 ? 1 : -1;
                    len1 = abs(l1 - r1) + 1;
                }
            } else l1 += len * sgn1;

            if (!len2) {
                if (++p2 < path2.size()) {
                    l2 = path2[p2][0];
                    r2 = path2[p2][1];
                    sgn2 = l2 <= r2 ? 1 : -1;
                    len2 = abs(l2 - r2) + 1;
                }
            } else l2 += len * sgn2;
        }
    }

    for (int i = lg; i; i--) {
        int half = 1 << (i - 1);
        for (int v : blocks[i]) {
            int v_set = dsus[i].find(v);
            if (v_set != v) {
                int dir1 = v >= n + 1, p1 = v - dir1 * (n + 1), dir2 = v_set >= n + 1, p2 = v_set - dir2 * (n + 1);
                add(i - 1, dir1, p1, dir2, p2);
                add(i - 1, dir1, dir1 ? p1 - half : p1 + half, dir2, dir2 ? p2 - half : p2 + half);
            }
        }
    }
    for (int i = 1; i <= n; i++) add(0, 0, i, 1, i);

    vector<int> sets, count(2 * (n + 1), 0);
    for (int i = 1; i <= n; i++) {
        int s = dsus[0].find(encode(0, i));
        if (!count[s]++) sets.emplace_back(s);
    }

    offset.resize(2 * (n + 1));
    for (int total = 0; int s : sets) {
        offset[s] = total;
        total += count[s];
    }

    vector<int> temp(n);
    for (int i = 1; i <= n; i++) temp[offset[dsus[0].find(encode(0, i))]++] = a[base[i]];
    a = temp;

    auto normalize = [&](int x) {
        return (x + k) % k;
    };

    auto time = 0LL;
    if (!k) {
        for (int s : sets) {
            int c = count[s];
            if (c < 2) continue;

            int o = offset[s];
            auto begin = a.begin() + o - c, end = a.begin() + o;
            if (c == 2) {
                time += abs(a[o - c] - a[o - c + 1]);
                continue;
            }

            auto mid = a.begin() + o - c + c / 2;
            nth_element(begin, mid, end);
            for (int i = o - c; i < o; i++) time += abs(a[i] - *mid);
        }
    } else if (k == 1) {
        for (int s : sets) {
            int c = count[s];
            if (c < 2) continue;

            int o = offset[s];
            auto begin = a.begin() + o - c, end = a.begin() + o;
            if (c == 2) {
                time += a[o - c] != a[o - c + 1];
                continue;
            }

            sort(begin, end);
            int longest = 0;
            for (int l = o - c, r = o - c + 1; l < o; l = r++) {
                for (; r < o && a[l] == a[r]; r++);
                longest = max(longest, r - l);
            }
            time += c - longest;
        }
    } else {
        vector<int> freq(k, 0), len(k, 0), cycle(2 * n, 0), plus, minus, distinct;
        vector<long long> pref1(2 * n + 1, 0), pref2(2 * n + 1, 0);
        for (int s : sets) {
            int c = count[s];
            if (c < 2) continue;

            int o = offset[s];
            auto begin = a.begin() + o - c, end = a.begin() + o;
            if (c == 2) {
                int al = a[o - c], ar = a[o - c + 1];
                if (al == ar) continue;

                al = normalize(al);
                ar = normalize(ar);
                if (al == ar) time += 2;
                else {
                    int d = abs(al - ar);
                    time += min(d, k - d);
                }
                continue;
            }

            sort(begin, end);
            distinct.clear();
            for (int l = o - c, r = o - c + 1; l < o; l = r++) {
                for (; r < o && a[l] == a[r]; r++);

                int rem = normalize(a[l]);
                if (!freq[rem]) distinct.emplace_back(rem);
                freq[rem] += r - l;
                len[rem] = max(len[rem], r - l);
            }

            if (distinct.size() == 1) {
                int rem = distinct[0];
                time += min(2 * (c - len[rem]), c);
                freq[rem] = len[rem] = 0;
                continue;
            }

            sort(distinct.begin(), distinct.end());
            int s1 = distinct.size(), s2 = 2 * s1;
            for (int i = 0; i < s1; i++) {
                cycle[i] = distinct[i];
                cycle[i + s1] = distinct[i] + k;
            }

            pref1[0] = pref2[0] = 0;
            for (int i = 0; i < s2; i++) {
                int f = freq[normalize(cycle[i])];
                pref1[i + 1] = pref1[i] + f;
                pref2[i + 1] = pref2[i] + (long long) f * cycle[i];
            }

            plus.clear();
            minus.clear();
            if (distinct.back() == k - 1 && !freq[0]) plus.emplace_back(0);
            for (int rem : distinct) {
                if (rem != k - 1 && !freq[rem + 1]) plus.emplace_back(rem + 1);
                if (rem && !freq[rem - 1]) minus.emplace_back(rem - 1);
            }
            if (!distinct[0] && !freq[k - 1]) minus.emplace_back(k - 1);

            auto t = LLONG_MAX;
            for (int x = 0, y = 0, z = 0, l = 0, m = 0, prev = -1, pivot = k / 2; x < s1 || y < plus.size() || z < minus.size();) {
                int rem = INT_MAX;
                if (x < s1) rem = min(rem, distinct[x]);
                if (y < plus.size()) rem = min(rem, plus[y]);
                if (z < minus.size()) rem = min(rem, minus[z]);

                for (; x < s1 && distinct[x] == rem; x++);
                for (; y < plus.size() && plus[y] == rem; y++);
                for (; z < minus.size() && minus[z] == rem; z++);

                if (prev == rem) continue;
                prev = rem;

                for (; cycle[l] < rem; l++);
                int r = l + s1;
                for (m = max(m, l); m < r && cycle[m] <= rem + pivot; m++);
                auto f1 = pref1[m] - pref1[l], sum1 = pref2[m] - pref2[l],
                     f2 = pref1[r] - pref1[m], sum2 = pref2[r] - pref2[m],
                     total = sum1 - sum2 - f1 * rem + f2 * (rem + k);

                if (freq[rem]) total += 2 * (freq[rem] - len[rem]);
                t = min(t, total);
            }

            time += t;
            for (int rem : distinct) freq[rem] = len[rem] = 0;
        }
    }

    cout << time;
}
