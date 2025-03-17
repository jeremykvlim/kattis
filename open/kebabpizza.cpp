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

    int n, k;
    cin >> n >> k;

    vector<pair<int, int>> toppings(n);
    for (int i = 0; i < n; i++) {
        int a, b;
        cin >> a >> b;

        toppings[i] = minmax(a - 1, b - 1);
    }
    sort(toppings.begin(), toppings.end());
    toppings.erase(unique(toppings.begin(), toppings.end()), toppings.end());

    vector<int> count(k);
    for (auto [a, b] : toppings) {
        count[a]++;
        count[b]++;
    }

    vector<bool> seen(k);
    for (auto &[a, b] : toppings) {
        if (count[a] == 1) a = b;
        else if (count[b] == 1) b = a;

        seen[a] = seen[b] = true;
    }
    fill(count.begin(), count.end(), 0);

    for (auto [a, b] : toppings)
        if (a != b) {
            count[a]++;
            count[b]++;
        }

    int mask = 1;
    for (int i = 0; i < k; i++) {
        if (!seen[i]) continue;

        if (count[i] > 2) {
            cout << "impossible";
            exit(0);
        }

        mask &= count[i] == 2;
    }

    DisjointSets dsu(k);
    for (auto [a, b] : toppings) {
        if (a == b) continue;

        if (!mask && dsu.find(a) == dsu.find(b)) {
            cout << "impossible";
            exit(0);
        }
        dsu.unite(a, b);
    }

    if (mask) {
        int c = find(seen.begin(), seen.end(), true) - seen.begin();
        for (int i = 0; i < k; i++)
            if (i != c && seen[i] && dsu.find(i) != dsu.find(c)) {
                cout << "impossible";
                exit(0);
            }
    }
    cout << "possible";
}
