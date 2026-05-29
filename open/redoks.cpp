#include <bits/stdc++.h>
using namespace std;

struct RURQSegmentTree {
    struct Monoid {
        array<int, 10> count;
        int sum;

        Monoid() : count{}, sum(0) {}

        auto & operator=(const int &v) {
            fill(count.begin(), count.end(), 0);
            count[sum = (v + 10) % 10] = 1;
            return *this;
        }

        auto & operator+=(const int &v) {
            int shift = (v + 10) % 10;
            if (!shift) return *this;

            array<int, 10> temp{};
            sum = 0;
            for (int d1 = 0; d1 < 10; d1++) {
                int d2 = (d1 + shift) % 10;
                temp[d2] += count[d1];
                sum += d2 * count[d1];
            }
            count = temp;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            for (int d = 0; d < 10; d++) count[d] += monoid.count[d];
            sum += monoid.sum;
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            return ml += mr;
        }
    };

    int n, h;
    vector<Monoid> ST;
    vector<int> lazy;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    void apply(int i, const int &v) {
        ST[i] += v;
        if (i < n) lazy[i] += v;
    }

    void push(int i) {
        if (lazy[i]) {
            apply(i << 1, lazy[i]);
            apply(i << 1 | 1, lazy[i]);
            lazy[i] = 0;
        }
    }

    void push_down(int l, int r) {
        for (int b = h; b; b--) {
            if (((l >> b) << b) != l) push(l >> b);
            if (((r >> b) << b) != r) push((r - 1) >> b);
        }
    }

    void pull_up(int l, int r) {
        for (int b = 1; b <= h; b++) {
            if (((l >> b) << b) != l) pull(l >> b);
            if (((r >> b) << b) != r) pull((r - 1) >> b);
        }
    }

    void range_update(int l, int r, const int &v) {
        l += n;
        r += n;
        push_down(l, r);

        int temp_l = l, temp_r = r;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) apply(l++, v);
            if (r & 1) apply(--r, v);
        }

        pull_up(temp_l, temp_r);
    }

    Monoid range_query(int l, int r) {
        l += n;
        r += n;
        push_down(l, r);

        Monoid ml, mr;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ml = ml + ST[l++];
            if (r & 1) mr = ST[--r] + mr;
        }

        return ml + mr;
    }

    auto & operator[](int i) {
        return ST[i];
    }

    RURQSegmentTree(int n, const string &s) : n(n), h(__lg(n)), ST(2 * n), lazy(n, 0) {
        for (int i = 0; i < s.size(); i++) ST[i + n] = s[i] - '0';
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    string s;
    cin >> n >> m >> s;

    RURQSegmentTree st(bit_ceil((unsigned) n), s);
    while (m--) {
        int a, b;
        cin >> a >> b;

        cout << st.range_query(a - 1, b).sum << "\n";
        st.range_update(a - 1, b, 1);
    }
}
