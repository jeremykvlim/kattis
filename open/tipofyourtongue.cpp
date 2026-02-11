#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct WaveletMatrix {
    int lg;
    vector<int> mid;
    vector<vector<int>> pref;
    vector<pair<T, int>> order;

    WaveletMatrix(int n, vector<T> a) : lg(__lg(*max_element(a.begin(), a.end()) + 1) + 1), order(n) {
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

    int n, q;
    cin >> n >> q;

    vector<pair<string, int>> words(n), words_rev(n);
    for (int i = 0; i < n; i++) {
        string w;
        cin >> w;

        words[i] = {w, i};
        reverse(w.begin(), w.end());
        words_rev[i] = {w, i};
    }
    sort(words.begin(), words.end());
    sort(words_rev.begin(), words_rev.end());

    vector<int> rank(n), rank_rev(n);
    for (int i = 0; i < n; i++) {
        rank[words[i].second] = i;
        rank_rev[words_rev[i].second] = i;
    }

    vector<int> a(n);
    for (int i = 0; i < n; i++) a[i] = rank_rev[words[i].second];
    WaveletMatrix<int> wm(n, a);

    auto count_AND = [&](int l1, int r1, int l2, int r2) {
        if (l1 == r1 || l2 == r2) return 0;
        return wm.freq_less(l1, r1, r2) - wm.freq_less(l1, r1, l2);
    };

    while (q--) {
        string o, p, s;
        cin >> o >> p >> s;

        auto rev = s;
        reverse(rev.begin(), rev.end());

        int l1 = lower_bound(words.begin(), words.end(), make_pair(p, -1)) - words.begin(), 
            l2 = lower_bound(words_rev.begin(), words_rev.end(), make_pair(rev, -1)) - words_rev.begin();
        
        p.back()++;
        rev.back()++;
        
        int r1 = lower_bound(words.begin(), words.end(), make_pair(p, -1)) - words.begin(),
            r2 = lower_bound(words_rev.begin(), words_rev.end(), make_pair(rev, -1)) - words_rev.begin();

        if (o == "AND") cout << count_AND(l1, r1, l2, r2) << "\n";
        else if (o == "OR") cout << r1 - l1 + r2 - l2 - count_AND(l1, r1, l2, r2) << "\n";
        else cout << r1 - l1 + r2 - l2 - 2 * count_AND(l1, r1, l2, r2) << "\n";
    }
}
