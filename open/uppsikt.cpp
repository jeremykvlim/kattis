#include <bits/stdc++.h>
using namespace std;

template <typename C>
struct CartesianTree {
    int n, root;
    vector<array<int, 3>> CT;

    template <typename T>
    CartesianTree(const vector<T> &a) : n(a.size()), root(-1), CT(n, {-1, -1, -1}) {
        deque<int> st{-1};
        for (int i = 0; i <= n; i++) {
            int j = -1;
            while (st.size() > 1 && (i == n || !C()(a[st.back()], a[i]))) {
                j = st.back();
                st.pop_back();
            }

            if (i == n) root = j;
            else {
                if (st.back() != -1) {
                    CT[i][2] = st.back();
                    CT[st.back()][1] = i;
                }
                if (j != -1) {
                    CT[j][2] = i;
                    CT[i][0] = j;
                }
                st.emplace_back(i);
            }
        }
    }

    vector<int> post_order_traversal() {
        vector<int> order;
        auto dfs = [&](auto &&self, int v) -> void {
            if (!~v) return;
            auto [l, r, p] = CT[v];
            self(self, l);
            self(self, r);
            order.emplace_back(v);
        };
        dfs(dfs, root);
        return order;
    }

    auto & operator[](int i) {
        return CT[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> p(n);
    for (int &pi : p) cin >> pi;

    CartesianTree<greater<int>> ct(p);
    vector<vector<vector<int>>> dp(n, vector<vector<int>>(4, vector<int>(4, 1e9)));
    for (int v : ct.post_order_traversal()) {
        auto [l, r, _] = ct[v];

        vector<int> l1(4, 1e9), r1(4, 1e9);
        vector<array<int, 2>> l2(4, {(int) 1e9, (int) 1e9}), r2(4, {(int) 1e9, (int) 1e9}), l3(4, {(int) 1e9, (int) 1e9}), r3(4, {(int) 1e9, (int) 1e9});
        for (int i = 0; i < 4; i++) {
            if (!~l) l1[i] = l2[i][0] = 0;
            else
                for (int j = 0; j < 4; j++) {
                    l1[i] = min(l1[i], dp[l][i][j]);
                    l2[i][j & 1] = min(l2[i][j & 1], dp[l][i][j]);
                    if ((j >> 1) & 1) l3[i][j & 1] = min(l3[i][j & 1], dp[l][i][j]);
                }

            if (!~r) r1[i] = r2[i][0] = 0;
            else
                for (int j = 0; j < 4; j++) {
                    r1[i] = min(r1[i], dp[r][i][j]);
                    r2[i][(j >> 1) & 1] = min(r2[i][(j >> 1) & 1], dp[r][i][j]);
                    if (j & 1) r3[i][(j >> 1) & 1] = min(r3[i][(j >> 1) & 1], dp[r][i][j]);
                }
        }

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) dp[v][i][j] = min(dp[v][i][j], (i ? l2[i & 1][j & 1] + r2[i & 2][(j >> 1) & 1] : min(l2[0][j & 1] + r3[0][(j >> 1) & 1], l3[0][j & 1] + r2[0][(j >> 1) & 1])));
            dp[v][i][3] = min(dp[v][i][3], l1[(i & 1) | 2] + r1[(i & 2) | 1] + 1);
        }
    }
    cout << *min_element(dp[ct.root][0].begin(), dp[ct.root][0].end());
}
