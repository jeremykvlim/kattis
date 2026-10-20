#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct SparseTable {
    vector<vector<T>> ST;
    function<T(T, T)> f;

    SparseTable() {}
    SparseTable(const vector<T> &v, function<T(T, T)> func) : f(func) {
        if (v.empty()) return;
        ST.resize(__lg(v.size()) + 1);
        ST[0] = v;
        for (int i = 1; i < ST.size(); i++) {
            ST[i].resize(v.size() - (1 << i) + 1);
            for (int j = 0; j < ST[i].size(); j++) ST[i][j] = f(ST[i - 1][j], ST[i - 1][j + (1 << (i - 1))]);
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

    int n, k, b, m;
    cin >> n >> k >> b >> m;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    vector<int> indices(n);
    iota(indices.begin(), indices.end(), 0);
    SparseTable<int> st(indices, [&](int i, int j) { return a[i] <= a[j] ? i : j; });

    auto dfs = [&](auto &&self, const vector<int> &positions = {-1}, long long hash = 0) -> void {
        priority_queue pq([&](auto &a1, auto &a2) { return a[a1[2]] > a[a2[2]]; }, vector<array<int, 3>>());
        for (int i : positions)
            if (i + 1 < n) pq.push({i + 1, n - 1, st.range_query(i + 1, n)});

        while (!pq.empty() && k) {
            vector<int> temp;
            int v = a[pq.top()[2]];
            while (!pq.empty() && temp.size() != k && v == a[pq.top()[2]]) {
                auto [l, r, i] = pq.top();
                pq.pop();

                temp.emplace_back(i);
                if (l < i) pq.push({l, i - 1, st.range_query(l, i)});
                if (i < r) pq.push({i + 1, r, st.range_query(i + 1, r + 1)});
            }

            auto h = (hash * b + v) % m;
            for (int _ = 0; _ < temp.size(); _++) {
                cout << h << "\n";

                if (!--k) exit(0);
            }
            self(self, temp, h);
        }
    };
    dfs(dfs);
}