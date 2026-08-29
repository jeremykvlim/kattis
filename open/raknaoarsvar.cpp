#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] += v;
    }

    T pref_sum(int i) {
        T sum = 0;
        for (; i; i &= i - 1) sum += BIT[i];
        return sum;
    }

    T range_sum_query(int l, int r) {
        if (l >= r) return 0;
        return pref_sum(r) - pref_sum(l);
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

struct PURQSegmentTree {
    struct Monoid {
        int value;

        Monoid() : value(-1e9) {}

        auto & operator=(const int &v) {
            value = v;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            value = max(value, monoid.value);
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            ml += mr;
            return ml;
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

    int find_last(int v) {
        int i = 1;
        while (i < n) {
            if (ST[i << 1 | 1].value >= v) (i <<= 1) |= 1;
            else i <<= 1;
        }
        return i - n;
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

    int n, T, q;
    cin >> n >> T >> q;

    vector<int> countries(T), sizes(T), islands(n, 0);
    for (int i = 0; i < T; i++) {
        cin >> countries[i] >> sizes[i];

        islands[--countries[i]]++;
    }

    vector<tuple<char, int, int>> queries(q);
    for (auto &[t, i, s] : queries) {
        cin >> t >> i;

        if (t == '=') {
            cin >> s;
            i--;
        }
    }

    vector<int> countries_big, indices_big(n, -1);
    for (int c = 0; c < n; c++)
        if (islands[c] > 1e3) {
            countries_big.emplace_back(c);
            indices_big[c] = countries_big.size() - 1;
        }

    vector<vector<int>> big(countries_big.size()), small(n);
    for (int i = 0; i < T; i++) {
        int c = countries[i];
        if (~indices_big[c]) big[indices_big[c]].emplace_back(sizes[i]);
        else small[c].emplace_back(sizes[i]);
    }

    for (auto [t, i, s] : queries)
        if (t == '=' && ~indices_big[countries[i]]) big[indices_big[countries[i]]].emplace_back(s);

    vector<FenwickTree<int>> fws;
    for (auto &s : big) {
        sort(s.begin(), s.end());
        s.erase(unique(s.begin(), s.end()), s.end());
        fws.emplace_back(s.size() + 1);
    }

    for (int i = 0; i < T; i++)
        if (~indices_big[countries[i]]) {
            int j = indices_big[countries[i]];
            fws[j].update(lower_bound(big[j].begin(), big[j].end(), sizes[i]) - big[j].begin() + 1, 1);
        }

    vector<vector<int>> countries_small(1e3 + 1), indices_small(n);
    for (int c = 0; c < n; c++)
        if (!~indices_big[c]) {
            sort(small[c].begin(), small[c].end());
            indices_small[c].resize(small[c].size() + 1);
            for (int k = 1; k <= small[c].size(); k++) {
                countries_small[k].emplace_back(c);
                indices_small[c][k] = countries_small[k].size() - 1;
            }
        }

    vector<PURQSegmentTree> sts;
    for (int k = 0; k <= 1e3; k++) {
        vector<int> a;
        for (int c : countries_small[k]) a.emplace_back(small[c][small[c].size() - k]);
        sts.emplace_back(bit_ceil(a.size()), a);
    }

    for (auto [t, i, s] : queries)
        if (t == '=') {
            if (sizes[i] == s) continue;

            int c = countries[i];
            if (~indices_big[c]) {
                int j = indices_big[c];
                fws[j].update(lower_bound(big[j].begin(), big[j].end(), sizes[i]) - big[j].begin() + 1, -1);
                fws[j].update(lower_bound(big[j].begin(), big[j].end(), s) - big[j].begin() + 1, 1);
            } else {
                int prev = lower_bound(small[c].begin(), small[c].end(), sizes[i]) - small[c].begin();
                small[c].erase(small[c].begin() + prev);
                int next = lower_bound(small[c].begin(), small[c].end(), s) - small[c].begin();
                small[c].insert(small[c].begin() + next, s);
                for (int j = min(prev, next); j <= max(prev, next); j++) {
                    int k = small[c].size() - j;
                    sts[k].point_update(indices_small[c][k], small[c][j]);
                }
            }
            sizes[i] = s;
        } else {
            int l = 0, r = 1e3 + 1, m;
            while (l + 1 < r) {
                m = l + (r - l) / 2;

                if (sts[m].ST[1].value >= i) l = m;
                else r = m;
            }

            int country = l ? countries_small[l][sts[l].find_last(i)] : n - 1;
            for (int j = 0; j < countries_big.size(); j++) {
                int k = fws[j].range_sum_query(lower_bound(big[j].begin(), big[j].end(), i) - big[j].begin(), big[j].size());
                if (l < k || (l == k && country < countries_big[j])) {
                    l = k;
                    country = countries_big[j];
                }
            }
            cout << country + 1 << "\n";
        }
}
