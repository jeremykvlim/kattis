#include <bits/stdc++.h>
using namespace std;

struct KineticTournament {
    static inline vector<long long> A, S;
    static inline int time, C;

    static long long cost(int l, int r) {
        return S[l] + (A[r] - A[l + 1]) * (r - l) + C;
    }

    static long long eval(int i) {
        return i < 0 ? -1e18 : -cost(i, time);
    }

    static int overtake(int i, int j) {
        if (i < 0 || j < 0) return 1e9;
        if (cost(i, time) > cost(j, time)) return 0;
        int l = time, r = A.size(), m;
        while (l + 1 < r) {
            m = l + (r - l) / 2;

            if (cost(i, m) > cost(j, m)) r = m;
            else l = m;
        }
        return r;
    }

    struct Segment {
        int index, t;

        Segment(int i = -1, int t = 1e9) : index(i), t(t) {}

        friend Segment operator+(const Segment &sl, const Segment &sr) {
            int l = sl.index, r = sr.index;
            if (eval(l) < eval(r)) swap(l, r);
            return {l, min({sl.t, sr.t, overtake(l, r)})};
        }
    };

    int n;
    vector<Segment> KT;

    KineticTournament(int n, const vector<long long> &a, int c) : n(n), KT(2 * n) {
        time = 0;
        A = a;
        S = vector<long long>(n, 0);
        C = c;
    }

    void pull(int i) {
        KT[i] = KT[i << 1] + KT[i << 1 | 1];
    }

    int midpoint(int l, int r) {
        int i = 1 << __lg(r - l);
        return min(l + i, r - (i >> 1));
    }

    void refresh() {
        refresh(1, 0, n);
    }

    void refresh(int i, int l, int r) {
        if (KT[i].t > time || l + 1 == r) return;

        int m = midpoint(l, r);
        refresh(i << 1, l, m);
        refresh(i << 1 | 1, m, r);
        pull(i);
    }

    void modify(const int &pos) {
        modify(1, pos, 0, n);
    }

    void modify(int i, int pos, int l, int r) {
        if (l + 1 == r) {
            KT[i] = {KT[i].index == l ? -1 : l, (int) 1e9};
            return;
        }

        int m = midpoint(l, r);
        if (pos < m) modify(i << 1, pos, l, m);
        else modify(i << 1 | 1, pos, m, r);
        pull(i);
    }

    long long query(int t) {
        time = t;
        refresh();
        return eval(KT[1].index);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    cin >> n >> c;

    vector<long long> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    KineticTournament kt(n + 1, a, c);
    kt.modify(0);
    for (int i = 1; i <= n; i++) {
        int k;
        cin >> k;

        kt.S[i] = -kt.query(i);
        if (i == n) break;
        kt.modify(i);
        kt.modify(((kt.S[i] + k) % i) + 1);
    }
    cout << kt.S[n];
}
