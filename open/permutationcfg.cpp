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

    int n, s, Q;
    cin >> n >> s >> Q;

    vector<int> perm(n), indices(n + 1);
    for (int i = 0; i < n; i++) {
        cin >> perm[i];

        indices[perm[i]] = i;
    }

    int a_max = 0;
    vector<array<int, 3>> queries(Q);
    for (int q = 0; q < Q; q++) {
        int k, a;
        cin >> k >> a;

        a_max = max(a_max, a);
        queries[q] = {a, k, q};
    }
    sort(queries.begin(), queries.end());

    int a_curr = 0, i = 0;
    vector<int> answers(Q);
    FenwickTree<int> fw1(n + 1);
    vector<array<int, 4>> sweep;
    auto dfs = [&](auto &&self, int prev, int depth) -> void {
        if (depth == 1) {
            for (; i < Q && queries[i][0] <= a_curr + prev; i++) {
                auto [a, k, q] = queries[i];
                answers[q] += fw1.range_sum_query(k - 1, n);
                if (prev >= k) sweep.push_back({prev, a - a_curr, k, q});
            }
            fw1.update(prev, 1);
            a_curr += prev;
            return;
        }

        for (int p : perm)
            if (p <= prev) {
                self(self, p, depth - 1);
                if (a_curr >= a_max) return;
            }
    };
    dfs(dfs, n, s);

    FenwickTree<int> fw2(n + 1);
    sort(sweep.begin(), sweep.end());
    i = 0;
    for (int j = 1; j <= n; j++) {
        fw2.update(indices[j] + 1, 1);
        for (; i < sweep.size() && sweep[i][0] == j; i++) {
            auto [p, diff, k, q] = sweep[i];
            answers[q] += fw2.pref_sum(indices[k] + 1) <= diff;
        }
    }

    for (int answer : answers) cout << answer << "\n";
}
