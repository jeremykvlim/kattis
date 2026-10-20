#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] += v;
    }

    T pref_sum(int i) {
        T sum = 0;
        for (; i; i &= i - 1) sum += BIT[i];
        return sum;
    }

    T range_sum_query(int l, int r) {
        if (l >= r) return 0;
        return pref_sum(r) - pref_sum(l);
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, Q;
    cin >> n >> Q;

    vector<pair<string, int>> words(n), words_rev(n);
    for (int i = 0; i < n; i++) {
        string w;
        cin >> w;

        words[i] = {w, i};
        reverse(w.begin(), w.end());
        words_rev[i] = {w, i};
    }
    sort(words.begin(), words.end());
    sort(words_rev.begin(), words_rev.end());

    vector<int> rank(n), rank_rev(n);
    for (int i = 0; i < n; i++) {
        rank[words[i].second] = i;
        rank_rev[words_rev[i].second] = i;
    }

    vector<int> a(n);
    for (int i = 0; i < n; i++) a[i] = rank_rev[words[i].second];

    vector<int> total(Q, 0);
    vector<string> op(Q);
    vector<array<int, 5>> sweep;
    for (int q = 0; q < Q; q++) {
        string p, s;
        cin >> op[q] >> p >> s;

        auto rev = s;
        reverse(rev.begin(), rev.end());

        int l1 = lower_bound(words.begin(), words.end(), make_pair(p, -1)) - words.begin(),
            l2 = lower_bound(words_rev.begin(), words_rev.end(), make_pair(rev, -1)) - words_rev.begin();

        p.back()++;
        rev.back()++;

        int r1 = lower_bound(words.begin(), words.end(), make_pair(p, -1)) - words.begin(),
            r2 = lower_bound(words_rev.begin(), words_rev.end(), make_pair(rev, -1)) - words_rev.begin();

        total[q] = r1 - l1 + r2 - l2;

        if (l1 == r1 || l2 == r2) continue;
        sweep.push_back({l1, l2, r2, q, -1});
        sweep.push_back({r1, l2, r2, q, 1});
    }
    sort(sweep.begin(), sweep.end());

    vector<int> count(Q, 0);
    FenwickTree<int> fw(n + 1);
    for (int i = 0; auto [m, l, r, q, sgn] : sweep) {
        for (; i < m; i++) fw.update(a[i] + 1, 1);
        count[q] += sgn * fw.range_sum_query(l, r);
    }

    for (int i = 0; i < Q; i++)
        if (op[i] == "AND") cout << count[i] << "\n";
        else if (op[i] == "OR") cout << total[i] - count[i] << "\n";
        else cout << total[i] - 2 * count[i] << "\n";
}
