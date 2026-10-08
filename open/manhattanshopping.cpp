#include <bits/stdc++.h>
using namespace std;

template <typename T, typename F>
void fast_subset_transform(int n, vector<T> &v, F &&f) {
    for (int k = 1; k < n; k <<= 1)
        for (int i = 0; i < n; i += k << 1)
            for (int j = 0; j < k; j++) v[i + j + k] = f(v[i + j + k], v[i + j]);
}

template <typename T>
void subset_zeta_transform(int n, vector<T> &f) {
    fast_subset_transform(n, f, [](T x, T y) { return x + y; });
}

template <typename T>
void subset_mobius_transform(int n, vector<T> &f) {
    fast_subset_transform(n, f, [](T x, T y) { return x - y; });
}

template <typename T>
vector<T> OR_convolve(const vector<T> &a, const vector<T> &b, size_t truncate = INT_MAX) {
    auto m = min(bit_ceil(max(a.size(), b.size())), truncate), da = min(a.size(), m), db = min(b.size(), m), n = bit_ceil(max(da, db));
    if (n <= 256 || min(da, db) <= __lg(n)) {
        vector<T> c(m, 0);
        for (int i = 0; i < da; i++)
            for (int j = 0; j < db; j++)
                if ((i | j) < m) c[i | j] += a[i] * b[j];
        return c;
    }

    static vector<T> fzt_a, fzt_b;
    fzt_a.resize(n);
    copy_n(a.begin(), da, fzt_a.begin());
    fill(fzt_a.begin() + da, fzt_a.end(), T{});

    if (a != b) {
        fzt_b.resize(n);
        copy_n(b.begin(), db, fzt_b.begin());
        fill(fzt_b.begin() + db, fzt_b.end(), T{});
    }
    subset_zeta_transform(n, fzt_a);

    if (a == b)
        for (int i = 0; i < n; i++) fzt_a[i] *= fzt_a[i];
    else {
        fzt_b.resize(n);
        copy_n(b.begin(), db, fzt_b.begin());
        fill(fzt_b.begin() + db, fzt_b.end(), T{});
        subset_zeta_transform(n, fzt_b);
        for (int i = 0; i < n; i++) fzt_a[i] *= fzt_b[i];
    }
    subset_mobius_transform(n, fzt_a);
    return {fzt_a.begin(), fzt_a.begin() + m};
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
