#include <bits/stdc++.h>
using namespace std;

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

        int m = -1, cl = -1, cr = -1;
        for (int L = 0; L < n; L++) {
            vector<int> rows(n + 1, 0);
            for (int i = 0; i < n; i++)
                if (p[i] != i) {
                    int l = min(p[i], i) + 1, r = max(p[i], i);

                    if (l <= L && L <= r) {
                        rows[0]++;
                        rows[l]--;
                        rows[r + 1]++;
                    } else {
                        rows[l]++;
                        rows[r + 1]--;
                    }
                }

            partial_sum(rows.begin(), rows.end(), rows.begin());
            for (int R = L + 1; R < n; R++)
                if (m < rows[R] || m == rows[R] && (cl > L || (cl == L && cr > R))) {
                    m = rows[R];
                    cl = L;
                    cr = R;
                }
        }
        m /= 2;

        vector<bool> dir(n, false);
        vector<int> count(n, 0);
        vector<array<int, 3>> edges(m + m);
        for (int i = 0, j = 0; i < n; i++)
            if (p[i] != i) {
                int l = min(p[i], i) + 1, r = max(p[i], i);

                auto add = [&](int i) {
                    int l = min(p[i], i), r = max(p[i], i);

                    if ((p[i] < i) ^ dir[i])
                        for (int j = l + 1; j <= r; j++) count[j]++;
                    else {
                        for (int j = 0; j <= l; j++) count[j]++;
                        for (int j = r + 1; j < n; j++) count[j]++;
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

        vector<bool> invert(m + m, false);
        auto adjust = [&](int i, int value) {
            auto [u, v, _] = edges[i];

            int l = min(u, v), r = max(u, v);

            if ((u > v) ^ invert[i])
                for (int j = l + 1; j <= r; j++) count[j] += value;
            else {
                for (int j = 0; j <= l; j++) count[j] += value;
                for (int j = r + 1; j < n; j++) count[j] += value;
            }
        };
        for (int i = 0; i < m + m; i++) {
            invert[i] = i + 1 > m;
            adjust(i, 1);
        }

        for (int i = max_element(count.begin(), count.end()) - count.begin(); count[i] > m; i = max_element(count.begin(), count.end()) - count.begin()) {
            auto check = [&](int i, int j) {
                auto [u, v, _] = edges[i];

                int l = min(u, v), r = max(u, v);

                if ((u > v) ^ invert[i]) return l < j && j <= r;
                else return j <= l || r < j;
            };

            int l = m + m - 1, r = 0;
            while (invert[l] || !check(l, i)) l--;
            while (!invert[r] || !check(r, i)) r++;
            adjust(l, -1);
            invert[l] = true;
            adjust(l, 1);
            adjust(r, -1);
            invert[r] = false;
            adjust(r, 1);
        }

        for (int i = 0; i < m + m; i++) {
            auto [u, v, j] = edges[i];

            dir[j] = (v == j) ^ invert[i];
        }

        vector<array<int, 2>> lazy(n + 1, {0, 0});
        vector<int> q(n);
        iota(q.begin(), q.end(), 0);
        auto update = [&](int i, int value) {
            if (p[i] == q[i]) return;

            int l = min(p[i], q[i]), r = max(p[i], q[i]);

            if ((p[i] < q[i]) ^ dir[i]) {
                lazy[l + 1][dir[i]] += value;
                lazy[r + 1][dir[i]] -= value;
            } else {
                lazy[0][dir[i]] += value;
                lazy[l + 1][dir[i]] -= value;
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

            auto prev = [&](int i) {return (i - 1 + n) % n;};
            auto next = [&](int i) {return (i + 1) % n;};

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

                int l = min(temp, q[cols[i]]), r = max(temp, q[cols[i]]);
                if ((back ? (q[cols[i]] < temp) : (q[cols[i]] > temp)) ^ (l <= p[cols[i]] && p[cols[i]] <= r)) dir[cols[i]] = !dir[cols[i]];

                update(cols[i], 1);
                row[q[cols[i]]] = back ? '\\' : '/';
            }

            cout << row << "\n";
        }
    }
}
