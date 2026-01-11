#include <bits/stdc++.h>
using namespace std;

struct PURQSegmentTree {
    struct Monoid {
        int value, freq;

        Monoid() : value(0), freq(0) {}

        auto & operator=(const int &v) {
            value = v;
            freq = 1;
            return *this;
        }

        auto & operator+=(const int &v) {
            value += v;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            if (value == monoid.value) freq += monoid.freq;
            else if (freq > monoid.freq) freq -= monoid.freq;
            else {
                value = monoid.value;
                freq = monoid.freq - freq;
            }

            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            return ml += mr;
        }
    };

    int n;
    vector<Monoid> ST;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    void point_update(int i, const int &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    Monoid range_query(int l, int r) {
        Monoid ml, mr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ml = ml + ST[l++];
            if (r & 1) mr = ST[--r] + mr;
        }

        return ml + mr;
    }

    auto & operator[](int i) {
        return ST[i];
    }

    PURQSegmentTree(int n, const vector<int> &a) : n(n), ST(2 * n) {
        for (int i = 0; i < a.size(); i++) ST[i + n] = a[i];
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    cin >> n >> c;

    vector<int> colors(n + 1);
    vector<vector<int>> indices(c + 1);
    for (int i = 0; i < n; i++) {
        cin >> colors[i];

        indices[colors[i]].emplace_back(i + 1);
    }
    PURQSegmentTree st(n, colors);

    int m;
    cin >> m;

    while (m--) {
        int a, b;
        cin >> a >> b;

        int color = st.range_query(a - 1, b).value;
        if (!color) cout << "no\n";
        else {
            if (upper_bound(indices[color].begin(), indices[color].end(), b) - lower_bound(indices[color].begin(), indices[color].end(), a) > (b - a + 1) / 2) cout << "yes " << color << "\n";
            else cout << "no\n";
        }
    }
}
