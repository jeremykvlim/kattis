#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    static inline int u;
    static inline vector<pair<int, int>> pipes;

    struct Segment {
        vector<int> nodes;

        Segment() {}

        auto & operator=(const int &v) {
            nodes.emplace_back(v);
            return *this;
        }

        friend auto operator+(const Segment &sl, const Segment &sr) {
            Segment seg;
            if (sl.nodes.empty() || sr.nodes.empty()) return seg;
            for (int i = 0; i < sl.nodes.size(); i++) {
                int v = u++;
                pipes.emplace_back(sl.nodes[i], v);
                pipes.emplace_back(sr.nodes[i], v);
                seg.nodes.emplace_back(u++);
                pipes.emplace_back(v, seg.nodes.back());
                seg.nodes.emplace_back(u++);
                pipes.emplace_back(v, seg.nodes.back());
            }
            return seg;
        }
    };

    int n;
    vector<Segment> ST;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void assign(int i, const int &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    auto & operator[](int i) {
        return ST[i];
    }

    SegmentTree(int n, const vector<int> &a) : n(n), ST(2 * n) {
        u = a[n - 1] + 1;
        for (int i = 0; i < a.size(); i++) assign(i, a[i]);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    int k = bit_ceil((unsigned) m);
    vector<int> slots(k);
    iota(slots.begin(), slots.end(), n + m + 1);

    SegmentTree st(k, slots);
    for (int i = 0; i < n; i++) st.pipes.emplace_back(i + 1, slots[i]);
    for (int i = 0; i < m; i++) st.pipes.emplace_back(st[1].nodes[i], i + n + 1);
    for (int i = m; i < k; i++) st.pipes.emplace_back(st[1].nodes[i], slots[i]);

    cout << st.u - 1 << " " << st.pipes.size() << "\n";
    for (auto [a, b] : st.pipes) cout << a << " " << b << "\n";
}
