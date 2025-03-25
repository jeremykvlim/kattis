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

    int b;
    cin >> b;

    while (b--) {
        int n;
        cin >> n;

        list<int> all(n);
        iota(all.begin(), all.end(), 1);
        vector<list<int>::iterator> its(n + 1);
        for (auto it = all.begin(); it != all.end(); it++) its[*it] = it;

        vector<int> x(n + 1);
        auto test = [&]() {
            cout << "test ";
            for (int i = 1; i <= n; i++) cout << x[i] << " ";
            cout << "\n" << flush;

            string s;
            cin >> s;

            return s;
        };

        DisjointSets dsu(n + 1);
        vector<int> maybe_good, maybe_bad;
        while (!all.empty()) {
            if (all.size() == 1) {
                maybe_good.emplace_back(*all.begin());
                all.clear();
            } else {
                fill(x.begin(), x.end(), 0);
                vector<pair<int, int>> m;
                int m1 = 0;
                for (int m2 : all)
                    if (m1) {
                        m.emplace_back(m1, m2);
                        x[m1] = m2;
                        x[m2] = m1;
                        m1 = 0;
                    } else m1 = m2;

                if (m1) {
                    maybe_good.emplace_back(m1);
                    all.erase(its[m1]);
                }

                auto s = test();
                for (auto [m1, m2] : m)
                    if (s[m1 - 1] == '1' && s[m2 - 1] == '1') {
                        all.erase(its[m2]);
                        dsu.unite(m1, m2);
                    } else {
                        maybe_bad.emplace_back(m2);
                        maybe_bad.emplace_back(m1);
                        all.erase(its[m1]);
                        all.erase(its[m2]);
                    }
            }
        }

        string S(n, '0');
        vector<int> good;
        for (int i = 1; i <= n; i++)
            if (dsu.find(i) == maybe_good.back()) {
                good.emplace_back(i);
                S[i - 1] = '1';
            }
        maybe_good.pop_back();

        reverse(maybe_bad.begin(), maybe_bad.end());
        queue<int> maybe;
        for (int m : maybe_bad) maybe.emplace(m);
        for (int m : maybe_good) maybe.emplace(m);
        while (!maybe.empty()) {
            fill(x.begin(), x.end(), 0);
            for (int m : good) {
                if (maybe.empty()) break;
                x[m] = maybe.front();
                maybe.pop();
            }

            auto s = test();
            queue<int> q;
            for (int m : good)
                if (s[m - 1] == '1') q.emplace(x[m]);

            while (!q.empty()) {
                for (int i = 1; i <= n; i++)
                    if (dsu.find(i) == q.front()) {
                        good.emplace_back(i);
                        S[i - 1] = '1';
                    }
                q.pop();
            }
        }
        cout << "answer " << S << "\n" << flush;
    }
}
