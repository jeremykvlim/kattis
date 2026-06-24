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

    auto & operator[](int i) {
        return CT[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s1;
    getline(cin, s1);
    istringstream iss1(s1);
    vector<int> v1{istream_iterator<int>{iss1}, istream_iterator<int>{}};

    string s2;
    getline(cin, s2);
    istringstream iss2(s2);
    vector<int> v2{istream_iterator<int>{iss2}, istream_iterator<int>{}};

    if (v1.size() != v2.size()) {
        cout << "NO";
        exit(0);
    }

    auto build = [&](const vector<int> &v) {
        vector<int> order(v.size());
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int i, int j) { return v[i] < v[j]; });
        return CartesianTree<less<>>(order);
    };
    auto ct1 = build(v1), ct2 = build(v2);

    auto dfs = [&](auto &&self, int u, int v, bool flip) -> bool {
        if (u == -1 || v == -1) return u == v;

        if (!flip) return self(self, ct1[u][0], ct2[v][0], flip) && self(self, ct1[u][1], ct2[v][1], flip);
        return self(self, ct1[u][0], ct2[v][1], flip) && self(self, ct1[u][1], ct2[v][0], flip);
    };
    cout << (dfs(dfs, ct1.root, ct2.root, false) || dfs(dfs, ct1.root, ct2.root, true) ? "YES" : "NO");
}
