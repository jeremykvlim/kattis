#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, P, h, F;
    cin >> n >> P >> h >> F;

    vector<int> before_AC(n, 0), before_tries(n, 0);
    vector<vector<int>> before_WA(n, vector<int>(P, 0));
    while (h--) {
        int w, p;
        string r;
        cin >> w >> p >> r;
        w--;
        p--;

        if (r == "AC") {
            before_AC[w]++;
            before_tries[w] += before_WA[w][p];
        } else before_WA[w][p]++;
    }

    vector<pair<int, int>> submissions(F);
    vector<bool> AC(F, false);
    vector<vector<int>> attempts(n, vector<int>(P, 0)), last_attempt(n, vector<int>(P, -1));
    for (int f = 0; f < F; f++) {
        int w, p;
        string r;
        cin >> w >> p >> r;
        w--;
        p--;

        submissions[f] = {w, p};
        AC[f] = r == "AC";
        attempts[w][p]++;
        last_attempt[w][p] = f;
    }

    vector<vector<int>> t(n, vector<int>(P, 0));
    for (int w = 0; w < n; w++)
        for (int p = 0; p < P; p++)
            if (attempts[w][p]) t[w][p] = before_WA[w][p] + attempts[w][p] - 1;

    auto after_AC = before_AC, after_tries = before_tries;
    for (int w = 0; w < n; w++)
        for (int p = 0; p < P; p++)
            if (attempts[w][p])
                if (~last_attempt[w][p] && AC[last_attempt[w][p]]) {
                    after_AC[w]++;
                    after_tries[w] += t[w][p];
                }

    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int w1, int w2) {
        if (after_AC[w1] != after_AC[w2]) return after_AC[w1] > after_AC[w2];
        if (after_tries[w1] != after_tries[w2]) return after_tries[w1] < after_tries[w2];
        return w1 < w2;
    });

    vector<vector<int>> ACs_by_tries(P + 1), ACs_by_order(P + 1);
    for (int i = 0; i < n; i++) {
        int w = order[i];
        ACs_by_tries[after_AC[w]].emplace_back(after_tries[w]);
        ACs_by_order[after_AC[w]].emplace_back(i);
    }

    DisjointSets dsu(n + 1);
    vector<int> reveals(n, -1), AC_l = after_AC, AC_r = after_AC, t_l = after_tries, t_r = after_tries;
    for (int f = F - 1; ~f; f--) {
        auto [w, p] = submissions[f];
        if (last_attempt[w][p] != f) continue;

        if (AC[f]) {
            AC_l[w]--;
            t_l[w] -= t[w][p];
        } else {
            AC_r[w]++;
            t_r[w] += t[w][p];
        }
        if (AC_l[w] >= AC_r[w]) continue;
      
        int l = n, r = -1, a = AC_r[w];
        if (0 <= a && a <= P && !ACs_by_tries[a].empty()) {
            int i = upper_bound(ACs_by_tries[a].begin(), ACs_by_tries[a].end(), t_r[w]) - ACs_by_tries[a].begin();
            if (i < ACs_by_tries[a].size()) {
                l = min(l, ACs_by_order[a][i]);
                r = max(r, ACs_by_order[a].back());
            }
        }

        for (a = AC_l[w] + 1; a <= AC_r[w] - 1; a++)
            if (0 <= a && a <= P && !ACs_by_order[a].empty()) {
                l = min(l, ACs_by_order[a][0]);
                r = max(r, ACs_by_order[a].back());
            }

        a = AC_l[w];
        if (0 <= a && a <= P && !ACs_by_tries[a].empty()) {
            int i = upper_bound(ACs_by_tries[a].begin(), ACs_by_tries[a].end(), t_l[w]) - ACs_by_tries[a].begin();
            if (i) {
                l = min(l, ACs_by_order[a][0]);
                r = max(r, ACs_by_order[a][i - 1]);
            }
        }
        if (r == -1) continue;
        
        for (int i = dsu.find(l); i <= r;) {
            if (order[i] == w) {
                i = dsu.find(i + 1);
                continue;
            }
            reveals[order[i]] = f;
            dsu.unite(i + 1, i);
            i = dsu.find(i);
        }
    }
    for (int f : reveals) cout << f + 1 << " ";
}
