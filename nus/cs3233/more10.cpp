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

    int n;
    cin >> n;

    DisjointSets dsu(2 * n);
    unordered_map<string, int> compress;
    vector<pair<int, int>> unequivalent;
    while (n--) {
        string X, s, Y;
        cin >> X >> s >> Y;

        reverse(X.begin(), X.end());
        reverse(Y.begin(), Y.end());

        int x, y;
        if (compress[X]) x = compress[X];
        else x = compress[X] = compress.size();
        if (compress[Y]) y = compress[Y];
        else y = compress[Y] = compress.size();

        if (s == "is") dsu.unite(x, y);
        else unequivalent.emplace_back(x, y);
    }

    vector<pair<string, int>> unique{compress.begin(), compress.end()};
    sort(unique.begin(), unique.end());
    for (int i = 0, j = 1; i < unique.size() && j < unique.size(); i = j, j++)
        while (j < unique.size()) {
            auto [X, x] = unique[i];
            auto [Y, y] = unique[j];

            int len = min({(int) X.size(), (int) Y.size(), 3});

            if (X.substr(0, len) == Y.substr(0, len)) {
                dsu.unite(x, y);
                j++;
            } else break;
        }

    for (auto [x, y] : unequivalent)
        if (dsu.find(x) == dsu.find(y)) {
            cout << "wait what?";
            exit(0);
        }

    cout << "yes";
}
