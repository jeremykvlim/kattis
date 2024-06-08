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

    DisjointSet dsu(2 * n);
    map<string, int> compress;
    vector<pair<int, int>> unequivalent;
    while (n--) {
        string X, s, Y;
        cin >> X >> s >> Y;

        reverse(X.begin(), X.end());
        reverse(Y.begin(), Y.end());

        int x = compress.emplace(X, compress.size()).first->second, y = compress.emplace(Y, compress.size()).first->second;
        if (s == "is") dsu.unite(x, y);
        else unequivalent.emplace_back(x, y);
    }

    for (auto x = compress.begin(), y = x; x != compress.end(); x = y) {
        y = next(x);
        while (y != compress.end()) {
            int len = min({(int) x->first.size(), (int) y->first.size(), 3});

            if (x->first.substr(0, len) == y->first.substr(0, len)) {
                dsu.unite(x->second, y->second);
                y++;
            } else break;
        }
    }

    bool consistent = true;
    for (auto [x, y] : unequivalent)
        if (dsu.find(x) == dsu.find(y)) {
            consistent = false;
            break;
        }

    cout << (consistent ? "yes" : "wait what?");
}
