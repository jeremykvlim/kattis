#include <bits/stdc++.h>
using namespace std;

struct RURQSegmentTree {
    struct Monoid {
        int value, i;

        Monoid(const pair<int, int> &v = {INT_MIN, INT_MAX}) : value(v.first), i(v.second) {}

        auto & operator+=(const int &v) {
            value += v;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            if (monoid.value > value || (monoid.value == value && monoid.i < i)) {
                value = monoid.value;
                i = monoid.i;
            }
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            return ml += mr;
        }
    };

    int n, h;
    vector<Monoid> ST;
    vector<int> lazy;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    void apply(int i, const int &v) {
        ST[i] += v;
        if (i < n) lazy[i] += v;
    }

    void push(int i) {
        for (int k = h; k; k--) {
            int j = i >> k;
            if (lazy[j]) {
                apply(j << 1, lazy[j]);
                apply(j << 1 | 1, lazy[j]);
                lazy[j] = 0;
            }
        }
    }

    void range_update(int l, int r, const int &v) {
        push(l + n);
        push(r + n - 1);
        bool cl = false, cr = false;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (cl) pull(l - 1);
            if (cr) pull(r);
            if (l & 1) {
                cl = true;
                apply(l++, v);
            }
            if (r & 1) {
                cr = true;
                apply(--r, v);
            }
        }

        for (l--; r; l >>= 1, r >>= 1) {
            if (cl) pull(l);
            if (cr && (!cl || l < r)) pull(r);
        }
    }

    Monoid range_query(int l, int r) {
        push(l + n);
        push(r + n - 1);
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

    RURQSegmentTree(int n, const vector<pair<int, int>> &a) : n(n), h(__lg(n)), ST(2 * n), lazy(n, 0) {
        for (int i = 0; i < a.size(); i++) ST[i + n] = a[i];
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n) {
        if (n == 1) {
            int _;
            cin >> _;
            cout << "0\n";
            continue;
        }

        vector<int> p(n), indices(n);
        for (int i = 0; i < n; i++) {
            cin >> p[i];

            indices[p[i]] = i;
        }

        vector<int> count(n + 1, 0);
        vector<vector<int>> intervals(n);
        for (int i = 0; i < n; i++)
            if (p[i] != i) {
                int l = min(p[i], i) + 1, r = max(p[i], i);
                intervals[l].emplace_back(r);
                count[l]++;
                count[r + 1]--;
            }

        vector<pair<int, int>> a(n);
        for (int i = 0, sum = 0; i < n; i++) a[i] = {sum += count[i], i};

        RURQSegmentTree st1(bit_ceil((unsigned) n), a);
        int m = -1, cl = -1, cr = -1;
        for (int l = 0; l < n; l++) {
            for (int r : intervals[l]) st1.range_update(0, r + 1, -2);
            if (l + 1 < n) {
                auto [value, r] = st1.range_query(l + 1, n);
                int rows = a[l].first + value;
                if (m < rows || (rows == m && (cl > l || (cl == l && cr > r)))) {
                    m = rows;
                    cl = l;
                    cr = r;
                }
            }
        }
        m /= 2;

        vector<bool> dir(n, false);
        vector<array<int, 3>> edges(2 * m);
        for (int i = 0; i < n; i++) a[i] = {0, i};
        RURQSegmentTree st2(bit_ceil((unsigned) n), a);

        for (int i = 0, j = 0; i < n; i++)
            if (p[i] != i) {
                int l = min(p[i], i) + 1, r = max(p[i], i);
                auto add = [&](int i) {
                    if (((p[i] < i) ^ dir[i])) st2.range_update(l, r + 1, 1);
                    else {
                        st2.range_update(0, l, 1);
                        if (r + 1 < n) st2.range_update(r + 1, n, 1);
                    }
                };
                if (cr < l || r < cl || (cl < l && r < cr)) {
                    dir[i] = p[i] > i;
                    add(i);
                } else if (l <= cl && cr <= r) {
                    dir[i] = p[i] < i;
                    add(i);
                } else if (cl <= i && i < cr) edges[j++] = {i, p[i], i};
                else edges[j++] = {p[i], i, i};
            }
        sort(edges.begin(), edges.end());

        vector<bool> invert(2 * m, false);
        auto adjust = [&](int i, int value) {
            auto [u, v, _] = edges[i];

            int l = min(u, v) + 1, r = max(u, v);
            if (((u > v) ^ invert[i])) st2.range_update(l, r + 1, value);
            else {
                st2.range_update(0, l, value);
                if (r + 1 < n) st2.range_update(r + 1, n, value);
            }
        };
        for (int i = 0; i < 2 * m; i++) {
            invert[i] = i + 1 > m;
            adjust(i, 1);
        }

        for (;;) {
            auto check = [&](int i, int j) {
                auto [u, v, _] = edges[i];

                int l = min(u, v) + 1, r = max(u, v);
                if (((u > v) ^ invert[i])) return l <= j && j <= r;
                else return j < l || r < j;
            };
            auto [value, i] = st2.range_query(0, n);
            if (value <= m) break;

            int l = 2 * m - 1, r = 0;
            while (invert[l] || !check(l, i)) l--;
            while (!invert[r] || !check(r, i)) r++;

            adjust(l, -1);
            invert[l] = true;
            adjust(l, 1);

            adjust(r, -1);
            invert[r] = false;
            adjust(r, 1);
        }

        for (int i = 0; i < 2 * m; i++) {
            auto [u, v, j] = edges[i];
            dir[j] = (v == j) ^ invert[i];
        }

        vector<array<int, 2>> lazy(n + 1, {0, 0});
        vector<int> q(n);
        iota(q.begin(), q.end(), 0);
        auto update = [&](int i, int value) {
            if (p[i] == q[i]) return;

            int l = min(p[i], q[i]) + 1, r = max(p[i], q[i]);
            if ((p[i] < q[i]) ^ dir[i]) {
                lazy[l][dir[i]] += value;
                lazy[r + 1][dir[i]] -= value;
            } else {
                lazy[0][dir[i]] += value;
                lazy[l][dir[i]] -= value;
                lazy[r + 1][dir[i]] += value;
            }
        };
        for (int i = 0; i < n; i++) update(i, 1);

        vector<array<int, 2>> pref(n + 1, {0, 0});
        cout << m << "\n";
        for (; m; m--) {
            for (int i = 0; i < n; i++) {
                pref[i][0] = (!i ? 0 : pref[i - 1][0]) + lazy[i][0];
                pref[i][1] = (!i ? 0 : pref[i - 1][1]) + lazy[i][1];
            }

            bool back = false;
            for (int i = 0; i < n; i++)
                if (pref[i][0] + pref[i][1] == m) {
                    back = pref[i][1] == m;
                    break;
                }

            auto prev = [&](int i) { return (i - 1 + n) % n; };
            auto next = [&](int i) { return (i + 1) % n; };

            vector<int> child(n);
            iota(child.begin(), child.end(), 0);

            if (back) {
                for (int i = 0; i < n; i++)
                    if (dir[i] && p[i] != q[i]) child[next(q[i])] = max(child[next(q[i])], p[i] + (next(q[i]) > p[i] ? n : 0));

                for (int i = 1; i < n; i++) child[i] = max(child[i], child[i - 1]);
                for (int i = 0; i < n; i++) child[i] = (max(child[i], child[prev(i)] - (!i ? n : 0))) % n;
            } else {
                for (int i = 0; i < n; i++)
                    if (!dir[i] && p[i] != q[i]) child[q[i]] = min(child[q[i]], next(p[i]) - (q[i] < next(p[i]) ? n : 0));

                for (int i = n - 2; ~i; i--) child[i] = min(child[i], child[i + 1]);
                for (int i = n - 1; ~i; i--) child[i] = (min(child[i], child[next(i)] + (i == n - 1 ? n : 0)) + n) % n;
            }

            vector<bool> used(n, false);
            for (int i = 0; i < n; i++)
                if (pref[i][0] + pref[i][1] == m) used[back ? child[i] : prev(child[i])] = true;

            vector<int> cols;
            for (int i = back ? 0 : n - 1; back ? i < n : ~i; back ? i++ : i--)
                if (used[i]) cols.emplace_back(indices[i]);

            string row(n, '.');
            for (int i = 0, j = q[cols[0]]; i < cols.size(); i++) {
                update(cols[i], -1);

                int temp = q[cols[i]];
                q[cols[i]] = i == cols.size() - 1 ? j : q[cols[i + 1]];

                auto [l, r] = minmax(temp, q[cols[i]]);
                if ((back ? (q[cols[i]] < temp) : (q[cols[i]] > temp)) ^ (l <= p[cols[i]] && p[cols[i]] <= r)) dir[cols[i]] = !dir[cols[i]];

                update(cols[i], 1);
                row[q[cols[i]]] = back ? '\\' : '/';
            }

            cout << row << "\n";
        }
    }
}
