#include <bits/stdc++.h>
using namespace std;

struct DisjointSet {
    vector<int> sets;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            return true;
        }
        return false;
    }

    DisjointSet(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> P1(n), P2(n), color(n);
    for (int i = 0; i < n; i++) {
        char c;
        cin >> P1[i] >> P2[i] >> c;

        P1[i]--;
        P2[i]--;
        color[i] = c - 'a';
    }

    if (n <= 2) {
        for (int i = 0; i < n; i++) cout << (char) (color[i] + 'a') << (char) (color[i] + 'a') << "\n";
        exit(0);
    }

    string second(2 * n, 'z');
    for (int mask = 0; mask < 1 << (n - 2); mask++) {
        DisjointSet dsu(n);
        vector<int> fixed(n, -1), temp = color;
        string s;
        for (int i = 1, j = n - 1; i <= n - 2; i++, j--) {
            if (!~P1[j]) continue;

            int p1 = P1[j], p2 = P2[j];
            if (mask & (1 << (i - 1))) swap(p1, p2);

            auto check = [&](int c, int p) -> bool {
                if (c < color[p]) return false;
                if (c > color[p]) {
                    int p_set = dsu.find(p);
                    if (temp[p_set] > c || ~fixed[p_set] && fixed[p_set] != c) return false;
                    fixed[p_set] = c;
                }
                return true;
            };
            if (!check(color[j], p1)) goto next;

            int c_set = dsu.find(j);
            if (~fixed[c_set]) {
                if (!check(fixed[c_set], p2)) goto next;
            } else {
                if (temp[c_set] <= color[p2]) fixed[c_set] = color[p2];
                else {
                    int p2_set = dsu.find(p2);
                    if (~fixed[p2_set] && fixed[p2_set] < temp[c_set]) goto next;
                    if (dsu.unite(c_set, p2_set)) {
                        temp[c_set] = max(temp[c_set], temp[p2_set]);
                        fixed[c_set] = max(fixed[c_set], fixed[p2_set]);
                    }
                }
            }
        }

        for (int i = 0; i < n; i++) {
            int i_set = dsu.find(i);

            s += (char) ((~fixed[i_set] ? fixed[i_set] : temp[i_set]) + 'a');
        }
        second = min(second, s);
        next:;
    }

    if (second.size() <= n)
        for (int i = 0; i < n; i++) cout << (char) (color[i] + 'a') << second[i] << "\n";
    else cout << -1;
}
