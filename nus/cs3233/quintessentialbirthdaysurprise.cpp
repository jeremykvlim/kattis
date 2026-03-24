#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct AffineFunction {
    T m, c;
    int i;
    mutable T cutoff;

    AffineFunction(T m = 0, T c = numeric_limits<T>::lowest() / 4, int i = -1) : m(m), c(c), i(i), cutoff(0) {}

    T operator()(T x) const {
        return m * x + c;
    }

    bool operator<(const AffineFunction &f) const {
        return m < f.m;
    }

    bool operator<(T x) const {
        return cutoff < x;
    }
};

template <typename T>
struct DynamicHull : multiset<AffineFunction<T>, less<>> {
    T div(T a, T b) {
        if constexpr (!is_floating_point_v<T>) return a / b - ((a ^ b) < 0 && a % b);
        else return a / b;
    }

    bool update(auto it_l, auto it_r) {
        if (it_r == this->end()) {
            it_l->cutoff = numeric_limits<T>::max();
            return false;
        }

        if (it_l->m == it_r->m) it_l->cutoff = it_l->c > it_r->c ? numeric_limits<T>::max() : numeric_limits<T>::lowest();
        else it_l->cutoff = div(it_r->c - it_l->c, it_l->m - it_r->m);
        return it_l->cutoff >= it_r->cutoff;
    }

    void add(const AffineFunction<T> &f) {
        auto it = this->insert(f);
        for (auto it_r = next(it); update(it, it_r); it_r = this->erase(it_r));

        if (it != this->begin()) {
            auto it_l = prev(it);
            if (update(it_l, it)) {
                this->erase(it);
                it = it_l;
                update(it, next(it));
            } else it = it_l;
        }

        while (it != this->begin()) {
            auto it_l = prev(it);
            if (it_l->cutoff < it->cutoff) break;
            update(it_l, this->erase(it));
            it = it_l;
        }
    }

    void add(T m, T c, int i = -1) {
        add(AffineFunction<T>(m, c, i));
    }

    pair<T, int> query(T x) {
        auto f = *this->lower_bound(x);
        return {f(x), f.i};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<tuple<long long, long long, int>> events(n);
    for (auto &[t, v, e] : events) cin >> v >> t >> e;
    sort(events.begin(), events.end());

    int M;
    cin >> M;

    vector<vector<bool>> adj_matrix(6, vector<bool>(6, true));
    while (M--) {
        int a, b;
        cin >> a >> b;

        adj_matrix[a][b] = adj_matrix[b][a] = false;
    }

    vector<DynamicHull<long long>> dhs(6);
    vector<int> prev(n, -1);
    int j = -1;
    long long m = -1e10;
    for (int i = 0; i < n; i++) {
        auto [t, v, e] = events[i];

        int k = -1;
        long long satisfaction = -1e10;
        for (int d = 1; d <= 5; d++)
            if (adj_matrix[d][e] && !dhs[d].empty()) {
                auto [s, l] = dhs[d].query(v);
                if (satisfaction < s) {
                    satisfaction = s;
                    k = l;
                }
            }

        auto c = 0LL;
        if (satisfaction < v) satisfaction = v;
        else {
            c = satisfaction;
            prev[i] = k;
        }
        dhs[e].add(v, c, i);

        if (m < satisfaction) {
            m = satisfaction;
            j = i;
        }
    }

    vector<int> subseq;
    for (; ~j; j = prev[j]) subseq.emplace_back(j + 1);
    reverse(subseq.begin(), subseq.end());

    cout << m << "\n" << subseq.size() << "\n";
    for (int i : subseq) cout << i << " ";
}
