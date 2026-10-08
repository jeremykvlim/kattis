#include <bits/stdc++.h>
using namespace std;

struct RURQSegmentTree {
    struct Monoid {
        long long first, last, left, right;
        int begin, end;

        Monoid() : first(0), last(0), left(LLONG_MAX), right(LLONG_MAX), begin(2), end(0) {}

        auto & operator=(long long v) {
            first = last = v;
            left = right = LLONG_MAX;
            begin = end = 0;
            return *this;
        }

        auto & operator+=(long long v) {
            if (begin == 2) return *this;

            first += v;
            last += v;
            if (left != LLONG_MIN && left != LLONG_MAX) {
                left += v;
                right += v;
            }
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            if (left == LLONG_MIN || monoid.begin == 2) return *this;
            if (monoid.left == LLONG_MIN) {
                left = LLONG_MIN;
                return *this;
            }
            if (begin == 2) return *this = monoid;

            Monoid m;
            m.first = first;
            m.last = monoid.last;

            int dir = (last < monoid.first) - (last > monoid.first);
            m.begin = begin ? begin : dir ? dir : monoid.begin;
            m.end = monoid.end ? monoid.end : dir ? dir : end;

            auto update = [&](long long v) {
                if (m.left == LLONG_MAX) m.left = v;
                else if (m.right >= v) m.left = LLONG_MIN;
                m.right = v;
            };

            if (left != LLONG_MAX) {
                update(left);
                if (left != right) update(right);
            }

            if (min(end, dir) < 0 && max(dir, monoid.begin) > 0) update(min(last, monoid.first));

            if (monoid.left != LLONG_MAX) {
                update(monoid.left);
                if (monoid.left != monoid.right) update(monoid.right);
            }
            return *this = m;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            ml += mr;
            return ml;
        }
    };

    int n, h;
    vector<Monoid> ST;
    vector<long long> lazy;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    void apply(int i, const long long &v) {
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

    RURQSegmentTree(int n, const vector<int> &a) : n(n), h(__lg(n)), ST(2 * n), lazy(n, 0) {
        for (int i = 0; i < a.size(); i++) ST[i + n] = a[i];
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> e(n);
    for (int &ei : e) cin >> ei;

    int m;
    cin >> m;

    RURQSegmentTree st(bit_ceil((unsigned) n), e);
    while (m--) {
        string op;
        int s, f;
        cin >> op >> s >> f;

        if (op == "update") {
            int d;
            cin >> d;

            st.range_update(s - 1, f, d);
        } else {
            auto [first, last, left, right, begin, end] = st.range_query(s - 1, f);
            auto l = begin == 1 ? first : LLONG_MIN, r = end == -1 ? last : LLONG_MAX;
            cout << ((left == LLONG_MAX ? l < r : l < left && right < r) ? "YES\n" : "NO\n");
        }
    }
}
