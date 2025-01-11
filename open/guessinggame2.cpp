#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    struct Segment {
        int sum, height;

        Segment() : sum(1), height(1) {}

        auto & operator=(const Segment &s) {
            sum = s.sum;
            height = s.height;
            return *this;
        }

        friend auto operator+(const Segment &sl, const Segment &sr) {
            Segment seg;
            seg.sum = sl.sum + sr.sum;
            seg.height = sl.height + 1;
            return seg;
        }
    };

    int n;
    vector<Segment> ST;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    int midpoint(int l, int r) {
        int i = 1 << __lg(r - l);
        return min(l + i, r - (i >> 1));
    }

    int query(int pos) {
        return query(1, pos, 0, n);
    }

    int query(int i, int pos, int l, int r) {
        if (!--ST[i].sum) return ST[i].height;

        int m = midpoint(l, r);
        if (pos < m) return query(i << 1, pos, l, m);
        else return query(i << 1 | 1, pos, m, r);
    }

    auto & operator[](int i) {
        return ST[i];
    }

    SegmentTree(int n) : n(n), ST(2 * n) {
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int p, n;
    cin >> p >> n;

    int m = 1 << 16;
    if (p == 1) {
        cout << "7\n" << flush;

        int sum = 0;
        vector<bool> seen(n);
        for (int _ = 0; _ < n - 32; _++) {
            int i;
            cin >> i;

            seen[i] = true;
            sum = (sum + i) % m;
            cout << "7\n" << flush;
        }

        vector<int> id(n, -1);
        for (int i = 0, count = 0; i < n; i++)
            if (!seen[i]) id[i] = count++;

        SegmentTree st(32);
        for (int _ = 0; _ < 31; _++) {
            int i;
            cin >> i;

            int ai = st.query(id[i]);
            if (ai == 1 && ~id[i])
                if ((sum >> (id[i] / 2)) & 1) ai = 6;
            cout << ai << "\n" << flush;
        }
    } else {
        int count = 0, sum = 0;
        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];

            if (a[i] == 7) {
                count++;
                sum = (sum + i) % m;
            }
        }

        if (count == n - 32) {
            vector<int> s;
            for (int i = 0; i < n; i++) {
                if (a[i] == 6) a[i] = 1;
                if (a[i] != 7) s.emplace_back(i);
            }

            int l = 0, r = 32;
            for (int k = 5; k; k--) {
                int i = -1, j = -1;
                for (int h = l; h < r; h++)
                    if (a[s[h]] == k) {
                        if (!~i) i = h;
                        else if (!~j) j = h;
                        else goto next;
                    }

                if (~i && ~j) {
                    cout << s[i] << " " << s[j];
                    exit(0);
                } else if (~i && l + 1 < r) {
                    int mid = l + (r - l) / 2;

                    if (i < mid) l = mid;
                    else r = mid;
                }
                next:;
            }
            cout << s[l] << " " << s[l];
        } else {
            int s = 0;
            for (int i = n - 1; ~i; i--) {
                if (a[i] == 1 || a[i] == 6) s *= 2;
                s += a[i] == 6;
            }

            int s1 = (sum - s + m) % m, s2 = min(n - 1, s1 + m);
            cout << s1 << " " << s2;
        }
    }
}
