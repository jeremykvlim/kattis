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

    int n;
    cin >> n;

    DisjointSets dsu(2 * n);
    vector<unordered_set<int>> diff(2 * n);
    auto query = [&](int i, int j) {
        int u = dsu.find(i), v = dsu.find(j + n);
        if (u == v) return true;
        if (diff[u].count(v)) return false;

        cout << "? " << i + 1 << " " << j + 1 << "\n" << flush;

        bool b;
        cin >> b;

        if (b) {
            auto [big, small] = dsu.unite(u, v);
            if (small != -1) {
                for (int t : diff[small]) {
                    t = dsu.find(t);
                    diff[t].erase(small);
                    diff[t].emplace(big);
                    diff[big].emplace(t);
                }
                diff[small].clear();
            }
        } else {
            diff[u].emplace(v);
            diff[v].emplace(u);
        }
        return b;
    };

    vector<int> pi(n, 0), pos(n);
    for (int x = 0, j = 0, k = 1; x < n;) {
        int i = (j + x) % n;
        if (query(i, j)) {
            pos[j] = i;
            if (++j == n) {
                cout << "! " << x;
                exit(0);
            }
        } else if (j) {
            for (; k < j; k++) {
                int l = pi[k - 1];
                for (;;) {
                    if (query(pos[k], l)) {
                        pi[k] = l + 1;
                        break;
                    }

                    if (!l) break;
                    l = pi[l - 1];
                }
            }
            x += j - pi[j - 1];
            j = pi[j - 1];
        } else x++;
    }
    cout << "! -1";
}
