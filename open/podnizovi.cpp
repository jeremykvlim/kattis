#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct SparseTable {
    vector<vector<T>> ST;
    function<T(T, T)> f;

    SparseTable(vector<T> v, function<T(T, T)> func) : f(move(func)) {
        int n = __lg(v.size()) + 1;
        ST.resize(n);
        ST.front() = v;
        for (int i = 1; i < n; i++) {
            ST[i].resize(v.size() - (1 << i) + 1);
            for (int j = 0; j <= v.size() - (1 << i); j++)
                ST[i][j] = f(ST[i - 1][j], ST[i - 1][j + (1 << (i - 1))]);
        }
    }

    auto size() {
        return ST.size();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, b, m;
    cin >> n >> k >> b >> m;

    vector<int> a(n + 1);
    int biggest = 0;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];

        biggest = max(biggest, a[i]);
    }

    vector<int> last(biggest + 1);
    vector<vector<int>> indices(biggest + 1);
    for (int i = 1; i <= n; i++) {
        last[a[i]] = i;
        indices[a[i]].emplace_back(i);
    }

    SparseTable<int> st(last, [](int x, int y) {return max(x, y);});
    auto dfs = [&](auto &&self, vector<int> curr = {-1}, int v = 0) {
        for (int j = 0; j <= biggest; j++) {
            for (int i = st.size() - 1; ~i; i--)
                if (j < st.ST[i].size() && st.ST[i][j] && st.ST[i][j] <= curr[0]) {
                    j += (1 << i);
                    if (j > biggest) return;
                }

            int h = ((long long) v * b % m + j) % m;
            vector<int> next;
            for (int i = upper_bound(indices[j].begin(), indices[j].end(), curr[0]) - indices[j].begin(); i < indices[j].size(); i++)
                for (int l : curr) {
                    if (l >= indices[j][i]) break;

                    cout << h << "\n";
                    if (!--k) exit(0);

                    next.emplace_back(indices[j][i]);
                }

            if (!next.empty()) self(self, next, h);
        }
    };

    dfs(dfs);
}
