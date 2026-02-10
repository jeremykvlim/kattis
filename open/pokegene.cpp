#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct SparseTable {
    vector<vector<T>> ST;
    function<T(T, T)> f;

    SparseTable() {}
    SparseTable(vector<T> v, function<T(T, T)> func) : f(move(func)) {
        if (v.empty()) return;
        int n = __lg(v.size()) + 1;
        ST.resize(n);
        ST.front() = v;
        for (int i = 1; i < n; i++) {
            ST[i].resize(v.size() - (1 << i) + 1);
            for (int j = 0; j <= v.size() - (1 << i); j++)
                ST[i][j] = f(ST[i - 1][j], ST[i - 1][j + (1 << (i - 1))]);
        }
    }

    T range_query(int l, int r) {
        int i = __lg(r - l);
        return f(ST[i][l], ST[i][r - (1 << i)]);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<pair<string, int>> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i].first;

        a[i].second = i;
    }
    sort(a.begin(), a.end());

    vector<int> rank(n);
    for (int i = 0; i < n; i++) rank[a[i].second] = i;

    vector<int> lcp(n - 1);
    for (int i = 0; i < n - 1; i++) {
        int j = 0;
        for (; j < min(a[i].first.size(), a[i + 1].first.size()) && a[i].first[j] == a[i + 1].first[j]; j++);
        lcp[i] = j;
    }

    SparseTable<int> st(lcp, [](int x, int y) { return min(x, y); });
    while (q--) {
        int k, l;
        cin >> k >> l;

        vector<int> r(k);
        for (int &ri : r) {
            int x;
            cin >> x;

            ri = rank[x - 1];
        }
        sort(r.begin(), r.end());

        auto query = [&](int l, int r) -> int {
            if (l == r) return a[l].first.size();
            return st.range_query(l, r);
        };

        int ancs = 0;
        for (int i = 0; i + l <= k; i++) ancs += query(r[i], r[i + l - 1]) - max(i ? query(r[i - 1], r[i + l - 1]) : 0, i + l < k ? query(r[i], r[i + l]) : 0);
        cout << ancs << "\n";
    }
}
