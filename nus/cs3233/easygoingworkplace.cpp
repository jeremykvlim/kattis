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

    FenwickTree(int n) : BIT(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    vector<vector<int>> adj_list(n + 1);
    for (int i = 2; i <= n; i++) {
        int b;
        cin >> b;

        adj_list[b].emplace_back(i);
    }

    vector<int> in(n + 1), out(n + 1);
    int count = 0;
    auto dfs = [&](auto &&self, int v = 1) -> void {
        in[v] = count++;
        for (int u : adj_list[v]) self(self, u);
        out[v] = count;
    };
    dfs(dfs);

    FenwickTree<int> fw(n + 1);
    int slack = 0, j = 1;
    for (int i = n; i; i--) {
        if (a[i] < fw.pref_sum(out[i]) - fw.pref_sum(in[i] + 1)) slack++;
        else fw.update(in[i] + 1, 1);

        if (slack == n - k) {
            j = i;
            break;
        }
    }

    if (slack < n - k) {
        cout << -1;
        exit(0);
    }

    for (int i = 1; i < j; i++) cout << i << " ";
    for (int i = n; i >= j; i--) cout << i << " ";
}
