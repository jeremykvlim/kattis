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

    int n;
    long long X;
    cin >> n >> X;

    vector<long long> x(n);
    for (auto &xi : x) {
        cin >> xi;

        if (xi < X) {
            cout << -1;
            exit(0);
        }
    }

    SparseTable<long long> st(x, [&](auto x, auto y) { return x & y; });
    int kmax = -1;
    for (int k = 1; k <= n; k++) {
        for (int l = 0; l < n; l += k)
            if (st.range_query(l, min(n, l + k)) < X) goto next;
        kmax = k;

        next:;
    }

    cout << kmax;
}
