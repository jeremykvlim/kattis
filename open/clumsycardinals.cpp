#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
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

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    DisjointSets dsu(n);
    unordered_map<int, int> NE_SW, NW_SE;
    int count = n;
    for (int i = 0; i < n; i++) {
        int r, c;
        cin >> r >> c;

        for (int j = 0; j <= 2; j++) {
            if (NE_SW.count(r + c + j - 1))
                if (dsu.unite(i, NE_SW[r + c + j - 1])) count--;

            if (NW_SE.count(r - c + j - 1))
                if (dsu.unite(i, NW_SE[r - c + j - 1])) count--;
        }

        NE_SW[r + c] = NW_SE[r - c] = i;
    }

    cout << count;
}
