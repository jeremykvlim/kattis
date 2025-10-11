#include <bits/stdc++.h>
using namespace std;

template <typename T, typename F>
void fast_subset_transform(int n, vector<T> &v, F &&f) {
    for (int k = 1; k < n; k <<= 1)
        for (int i = 0; i < n; i += k << 1)
            for (int j = 0; j < k; j++) v[i + j + k] = f(v[i + j + k], v[i + j]);
}

template <typename T>
vector<T> fzt(int n, const vector<T> &f) {
    auto F = f;
    fast_subset_transform(n, F, [](auto x, auto y) { return x + y; });
    return F;
}

template <typename T>
vector<T> fmt(int n, const vector<T> &F) {
    auto f = F;
    fast_subset_transform(n, f, [](auto x, auto y) { return x - y; });
    return f;
}

template <typename T>
vector<T> OR_convolve(const vector<T> &a, const vector<T> &b) {
    int da = a.size(), db = b.size(), m = max(da, db), n = bit_ceil((unsigned) m);
    if (n <= 16 || min(da, db) <= __lg(n)) {
        vector<T> c(n);
        for (int i = 0; i < da; i++)
            for (int j = 0; j < db; j++) c[i | j] += a[i] * b[j];
        return c;
    }

    vector<T> fzt_a(n), fzt_b(n);
    for (int i = 0; i < da; i++) fzt_a[i] = a[i];
    for (int i = 0; i < db; i++) fzt_b[i] = b[i];

    vector<T> F_a = fzt(n, fzt_a), F_b = a == b ? F_a : fzt(n, fzt_b), F_c(n);
    for (int i = 0; i < n; i++) F_c[i] = F_a[i] * F_b[i];
    return fmt(n, F_c);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> x(n + 1), y(n + 1), t(n + 1);
    for (int i = 1; i <= n; i++) cin >> x[i] >> y[i] >> t[i];

    vector<array<int, 3>> points(n + 1);
    for (int i = 0; i <= n; i++) points[i] = {x[i] + y[i], x[i] - y[i], i};
    sort(points.begin(), points.end());

    vector<array<int, 4>> st;
    vector<vector<int>> blocks(n + 1);
    for (int i = 0, j = 0; i <= n; i = j) {
        int l = points[i][1], r = points[i][1];
        for (; j <= n && points[i][0] == points[j][0]; j++) {
            l = min(l, points[j][1]);
            r = max(r, points[j][1]);
            blocks[i].emplace_back(points[j][2]);
        }
        st.push_back({i, i, l, r});
        while (st.size() > 1 && st[st.size() - 2][2] <= st[st.size() - 1][3]) {
            st[st.size() - 2][1] = st[st.size() - 1][1];
            st[st.size() - 2][2] = min(st[st.size() - 2][2], st[st.size() - 1][2]);
            st[st.size() - 2][3] = max(st[st.size() - 2][3], st[st.size() - 1][3]);
            st.pop_back();
        }
    }

    vector<int> component(n + 1, -1);
    for (int c = 0; auto [bl, br, _, __] : st) {
        for (int b = bl; b <= br; b++)
            for (int i : blocks[b]) component[i] = c;
        c++;
    }

    vector<int> masks(st.size(), 0);
    for (int i = 1; i <= n; i++) masks[component[i]] |= 1 << (t[i] - 1);
    int unpurchased = ((1 << m) - 1) & (~masks[component[0]]);
    if (!unpurchased) {
        cout << 0;
        exit(0);
    }

    vector<int> base(1 << m, 0);
    for (int c = 0; c < st.size(); c++)
        if (c != component[0]) {
            int items = masks[c] & unpurchased;
            if (items) base[items] = 1;
        }

    int p = bit_ceil((unsigned) m);
    vector<vector<int>> pbase(p + 1);
    pbase[0] = base;
    for (int i = 1; i <= p; i++) pbase[i] = OR_convolve(pbase[i - 1], pbase[i - 1]);

    vector<int> coverage(1 << m, 0);
    coverage[0] = 1;
    int moves = 0;
    for (; ~p; p--) {
        auto temp = OR_convolve(coverage, pbase[p]);
        if (!temp[unpurchased]) {
            coverage = temp;
            moves += 1 << p;
        }
    }
    cout << moves + 2;
}
