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

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return false;

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return true;
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    while (cin >> s && s != "END") {
        int n = s.size();

        bool same = false;
        DisjointSets dsu(n);
        vector<int> first(26, -1);
        for (int i = 0; i < n; i++) {
            int c = tolower(s[i]) - 'a';
            if (!~first[c]) {
                first[c] = i;
                continue;
            }

            int j = first[c];
            if (s[i] == s[j]) {
                same = true;
                dsu.unite(i, j);
                dsu.unite((i + 1) % n, (j + 1) % n);
            } else {
                dsu.unite(i, (j + 1) % n);
                dsu.unite((i + 1) % n, j);
            }
        }

        int m = n / 2 + 1;
        for (int i = 0; i < n; i++)
            if (dsu.find(i) == i) m--;

        if (!m) cout << "none\n";
        else if (!same) {
            m /= 2;
            cout << m << " " << (m == 1 ? "leg\n" : "legs\n");
        } else cout << m << " " << (m == 1 ? "arm\n" : "arms\n");
    }
}
