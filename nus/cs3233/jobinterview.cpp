#include <bits/stdc++.h>
using namespace std;

struct KineticTournament {
    static inline vector<long long> A, S;
    static inline int time;

    static long long cost(int l, int r) {
        return S[l] + (A[r] - A[l + 1]) * (r - l);
    }

    static long long eval(int i) {
        return i < 0 ? -1e18 : -cost(i, time);
    }

    static int overtake(int i, int j) {
        if (i < 0 || j < 0) return 1e9;
        if (cost(i, time) > cost(j, time)) return 0;
        if (time >= A.size() - 1 || cost(i, A.size() - 1) <= cost(j, A.size() - 1)) return 1e9;
        int l = time, r = A.size(), m;
        while (l + 1 < r) {
            m = l + (r - l) / 2;

            if (cost(i, m) > cost(j, m)) r = m;
            else l = m;
        }
        return r;
    }

    struct Monoid {
        int t, index;

        Monoid(int t = 1e9, int i = -1) : t(t), index(i) {}

        friend Monoid operator+(const Monoid &ml, const Monoid &mr) {
            int l = ml.index, r = mr.index;
            if (eval(l) < eval(r)) swap(l, r);
            return {min({ml.t, mr.t, overtake(l, r)}), l};
        }
    };

    int n;
    vector<Monoid> ST;

    KineticTournament(int n, const vector<long long> &a) : n(n), ST(2 * n) {
        time = 0;
        A = a;
        S = vector<long long>(n, 0);
    }

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void point_update(int i) {
        for (ST[i += n] = {(int) 1e9, ST[i + n].index == i ? -1 : i}; i > 1; i >>= 1) pull(i >> 1);
    }

    void advance(int t) {
        for (time = t; ST[1].t <= time;) {
            int i = 1;
            while (i < n) {
                if (ST[i << 1].t <= time) i = i << 1;
                else if (ST[i << 1 | 1].t <= time) i = i << 1 | 1;
                else break;
            }
            for (; i; i >>= 1) pull(i);
        }
    }

    long long top() {
        return eval(ST[1].index);
    }

    auto & operator[](int i) {
        return ST[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    cin >> n >> c;

    vector<long long> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    KineticTournament kt(n + 1, a);
    kt.point_update(0);
    for (int i = 1; i <= n; i++) {
        int k;
        cin >> k;

        kt.advance(i);
        kt.S[i] = c - kt.top();
        if (i == n) break;
        kt.point_update(i);
        kt.point_update(((kt.S[i] + k) % i) + 1);
    }
    cout << kt.S[n];
}
