#include <bits/stdc++.h>
using namespace std;

template <typename... I>
unsigned long long hilbert_index(I... c) {
    using T = common_type_t<I...>;

    constexpr int D = sizeof...(I);
    array<T, D> coords{(T) c...};
    T c_max = max({c...});
    int b = 0;
    if (c_max)
        while (c_max >>= 1) b++;

    for (T mask = ((T) 1) << b; mask > 1; mask >>= 1)
        for (int i = D - 1; ~i; i--)
            if (coords[i] & mask) coords[0] ^= mask - 1;
            else {
                T m = (coords[0] ^ coords[i]) & (mask - 1);
                coords[0] ^= m;
                coords[i] ^= m;
            }

    for (int i = 1; i < D; i++) coords[i] ^= coords[i - 1];
    T m = 0;
    for (T mask = ((T) 1) << b; mask > 1; mask >>= 1)
        if (coords[D - 1] & mask) m ^= mask - 1;
    for (int i = 0; i < D; i++) coords[i] ^= m;

    auto h = 0ULL;
    for (; ~b; b--)
        for (int i = 0; i < D; i++) h = (h << 1) | ((coords[i] >> b) & 1);
    return h;
}

struct QueryDecomposition {
    int size;
    vector<array<int, 3>> queries;

    QueryDecomposition(int n, const vector<array<int, 3>> &queries) : size(ceil(sqrt(n))), queries(queries) {}

    vector<int> mo(const vector<int> &a, int k) {
        int Q = queries.size();
        vector<int> answers(Q), freq(a.size() + 1, 0), count(k, 0);
        vector<unsigned long long> hilbert_order(Q);
        for (int q = 0; q < Q; q++) {
            auto [l, r, i] = queries[q];
            hilbert_order[q] = hilbert_index(l, r);
        }
        vector<int> order(Q);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int i, int j) { return hilbert_order[i] < hilbert_order[j]; });

        int L = 0, R = -1, ans = 0;

        freq[0] = k;
        auto add = [&](int i) {
            int v = a[i], c = count[v];
            freq[c]--;
            count[v]++;
            freq[c + 1]++;
            ans = max(ans, c + 1);
        };

        auto remove = [&](int i) {
            int v = a[i], c = count[v];
            freq[c]--;
            count[v]--;
            freq[c - 1]++;
            for (; ans && !freq[ans]; ans--);
        };

        for (int q : order) {
            auto [l, r, i] = queries[q];
            while (L > l) add(--L);
            while (R < r) add(++R);
            while (L < l) remove(L++);
            while (R > r) remove(R--);
            answers[i] = ans;
        }

        return answers;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int longest = 0;
    vector<int> len(n + 1);
    vector<string> words(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> words[i];

        reverse(words[i].begin(), words[i].end());
        longest = max(longest, len[i] = words[i].size());
    }

    vector<vector<int>> word_indices(longest + 1), states(longest + 1);
    for (int i = 1; i <= n; i++)
        for (int l = 1; l <= len[i]; l++) word_indices[l].emplace_back(i);

    vector<int> dp(n + 1);
    int k = 1;
    for (int l = 1, prev = 1; l <= longest; l++) {
        vector<int> temp(prev * 26, -1);
        int count = 0;
        for (int i : word_indices[l]) {
            int j = dp[i] * 26 + (words[i][l - 1] - 'a');
            if (!~temp[j]) temp[j] = count++;
            dp[i] = temp[j];
            states[l].emplace_back(dp[i]);
        }
        k = max(k, prev = count);
    }

    vector<int> pref(longest + 2, 0);
    for (int l = 1; l <= longest + 1; l++) pref[l] = pref[l - 1] + states[l - 1].size();

    vector<int> a(pref.back(), 0);
    for (int l = 1; l <= longest; l++) copy(states[l].begin(), states[l].end(), a.begin() + pref[l]);

    int q;
    cin >> q;

    vector<array<int, 3>> qs(q);
    for (auto &[l, r, t] : qs) cin >> l >> r >> t;

    vector<int> indices;
    vector<array<int, 3>> queries;
    for (int i = 0; i < q; i++) {
        auto [ql, qr, qt] = qs[i];
        if (qt > longest) continue;

        int l = lower_bound(word_indices[qt].begin(), word_indices[qt].end(), ql) - word_indices[qt].begin(),
            r = upper_bound(word_indices[qt].begin(), word_indices[qt].end(), qr) - word_indices[qt].begin();
        if (l < r) {
            l += pref[qt];
            r += pref[qt];
            indices.emplace_back(i);
            queries.push_back({l, r - 1, (int) indices.size() - 1});
        }
    }

    QueryDecomposition qd(a.size(), queries);
    auto answers = qd.mo(a, k);
    vector<int> count(q, 0);
    for (int i = 0; i < indices.size(); i++) count[indices[i]] = answers[i];
    for (int c : count) cout << c << "\n";
}