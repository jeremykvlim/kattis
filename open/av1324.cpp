#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct WaveletMatrix {
    int lg;
    vector<int> mid;
    vector<vector<int>> pref;
    vector<pair<T, int>> order;

    WaveletMatrix(int n, vector<T> a) : lg(__lg(*max_element(a.begin(), a.end())) + 1), order(n) {
        mid.resize(lg);
        pref.assign(lg, vector<int>(n + 1, 0));

        vector<pair<T, int>> temp(n);
        for (int i = 0; i < n; i++) order[i] = {a[i], i};
        for (int b = lg - 1; ~b; b--) {
            for (int i = 0; i < n; i++) pref[b][i + 1] = pref[b][i] + ((order[i].first >> b) & 1);

            int zeroes = 0, ones = mid[b] = n - pref[b][n];
            for (int i = 0; i < n; i++) temp[((order[i].first >> b) & 1) ? ones++ : zeroes++] = order[i];
            order.swap(temp);
        }
    }

    tuple<T, int, int, int> quantile(int l, int r, int k) {
        T v = 0;
        for (int b = lg - 1; ~b; b--) {
            int zeroes = (r - pref[b][r]) - (l - pref[b][l]);
            if (k >= zeroes) {
                k -= zeroes;
                v |= ((T) 1) << b;
                l = pref[b][l] + mid[b];
                r = pref[b][r] + mid[b];
            } else {
                l -= pref[b][l];
                r -= pref[b][r];
            }
        }
        return {v, k, r - l, order[l + k].second};
    }

    int freq_less(int l, int r, T x) {
        int f = 0;
        for (int b = lg - 1; ~b; b--) {
            if ((x >> b) & 1) {
                f += (r - pref[b][r]) - (l - pref[b][l]);
                l = pref[b][l] + mid[b];
                r = pref[b][r] + mid[b];
            } else {
                l -= pref[b][l];
                r -= pref[b][r];
            }
        }
        return f;
    }

    int freq(int l, int r, T x) {
        if ((((T) 1) << lg) <= x) return 0;
        for (int b = lg - 1; ~b; b--) {
            if ((x >> b) & 1) {
                l = pref[b][l] + mid[b];
                r = pref[b][r] + mid[b];
            } else {
                l -= pref[b][l];
                r -= pref[b][r];
            }
        }
        return r - l;
    }

    T lower_bound(int l, int r, T x) {
        int f = freq_less(l, r, x);
        return f == r - l ? -1 : get<0>(quantile(l, r, f));
    }

    T predecessor(int l, int r, T x) {
        int f = freq_less(l, r, x);
        return !f ? -1 : get<0>(quantile(l, r, f - 1));
    }

    T successor(int l, int r, T x) {
        int f = freq_less(l, r, x) + freq(l, r, x);
        return f == r - l ? -1 : get<0>(quantile(l, r, f));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        vector<int> perm(n);
        for (int i = 0; i < n; i++) cin >> perm[i];

        vector<pair<int, int>> suff_max(n);
        pair<int, int> p{0, -1};
        for (int i = n - 1; ~i; i--) {
            suff_max[i] = p;
            p = max(p, make_pair(perm[i], i));
        }

        vector<pair<int, int>> pref_min(n);
        p = {INT_MAX, -1};
        for (int i = 0; i < n; i++) {
            p = min(p, make_pair(perm[i], i));
            pref_min[i] = p;
        }

        vector<pair<int, int>> queries;
        for (int i = 0; i < n; i++)
            if (suff_max[i].first > perm[i]) {
                int l = -1, r = i, m;
                while (l + 1 < r) {
                    m = l + (r - l) / 2;

                    if (pref_min[m].first < perm[i]) r = m;
                    else l = m;
                }
                if (r + 1 < i) queries.emplace_back(r + 1, i);
            }

        if (queries.empty()) {
            cout << "Ekkert mynstur!\n";
            continue;
        }

        auto deduped = perm;
        sort(deduped.begin(), deduped.end());
        deduped.erase(unique(deduped.begin(), deduped.end()), deduped.end());

        vector<int> rank(n);
        for (int i = 0; i < n; i++) rank[i] = lower_bound(deduped.begin(), deduped.end(), perm[i]) - deduped.begin();

        WaveletMatrix wm(n, rank);
        for (auto [l, r] : queries) {
            int k = wm.freq_less(l, r, lower_bound(deduped.begin(), deduped.end(), perm[r]) - deduped.begin());
            if (k < r - l) {
                int i = get<3>(wm.quantile(l, r, k));
                if (perm[i] < suff_max[r].first) {
                    cout << l << " " << i + 1 << " " << r + 1 << " " << suff_max[r].second + 1 << "\n";
                    goto next;
                }
            }
        }
        cout << "Ekkert mynstur!\n";
        next:;
    }
}

