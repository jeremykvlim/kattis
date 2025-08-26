#include <bits/stdc++.h>
using namespace std;

template <typename... I>
long long hilbert_index(I... c) {
    constexpr int D = sizeof...(I);
    array<int, D> coords{c...};
    int lg = __lg(max({c...}));

    for (int mask = 1 << lg; mask > 1; mask >>= 1)
        for (int i = D - 1; ~i; i--)
            if (coords[i] & mask) coords[0] ^= mask - 1;
            else {
                int t = (coords[0] ^ coords[i]) & (mask - 1);
                coords[0] ^= t;
                coords[i] ^= t;
            }

    for (int i = 1; i < D; i++) coords[i] ^= coords[i - 1];
    int m = 0;
    for (int mask = 1 << lg; mask > 1; mask >>= 1)
        if (coords[D - 1] & mask) m ^= mask - 1;
    for (int i = 0; i < D; i++) coords[i] ^= m;

    auto h = 0LL;
    for (int b = lg; ~b; b--)
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
        vector<long long> indices(Q);
        for (int q = 0; q < Q; q++) {
            auto [l, r, i] = queries[q];
            indices[q] = hilbert_index(l / size, r / size);
        }
        vector<int> order(Q);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int i, int j) { return indices[i] < indices[j]; });

        int L = 0, R = -1, ans = 0;

        freq[0] = k;
        auto add = [&](int i) {
            int v = a[i], c = count[v];
            freq[c]--;
            c++;
            count[v] = c;
            freq[c]++;
            ans = max(ans, c);
        };

        auto remove = [&](int i) {
            int v = a[i], c = count[v];
            freq[c]--;
            c--;
            count[v] = c;
            freq[c]++;
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

struct Trie {
    enum ascii {
        LOWER = 97,
        UPPER = 65,
        NUM = 48,
        SYM = 32,
        NA = 0
    };

    struct TrieNode {
        vector<int> next;

        TrieNode(int range = 26) : next(range, -1) {}
    };

    vector<TrieNode> T;
    ascii a;
    int r;
    vector<int> depth, index_in_depth;

    Trie(int n = 1, ascii alpha = LOWER, int range = 26) : T(n, TrieNode(range)), a(alpha), r(range), depth(1), index_in_depth(1) {}

    void add(string &s, int i, vector<int> &nodes_at_depth, vector<vector<int>> &word_indices, vector<vector<int>> &depth_indices) {
        int node = 0;
        for (char c : s) {
            int pos = c - a;

            if (T[node].next[pos] == -1) {
                T[node].next[pos] = T.size();
                T.emplace_back(TrieNode(r));

                int d = depth[node] + 1;
                depth.emplace_back(d);
                nodes_at_depth[d]++;
                index_in_depth.emplace_back(nodes_at_depth[d] - 1);
            }
            node = T[node].next[pos];
            int d = depth[node];
            word_indices[d].emplace_back(i);
            depth_indices[d].emplace_back(index_in_depth[node]);
        }
    }

    auto & operator[](int i) {
        return T[i];
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

    Trie trie;
    vector<int> nodes_at_depth(longest + 1);
    nodes_at_depth[0] = 1;
    vector<vector<int>> word_indices(longest + 1), depth_indices(longest + 1);
    for (int i = 1; i <= n; i++) trie.add(words[i], i, nodes_at_depth, word_indices, depth_indices);

    vector<int> pref1(longest + 2, 0), pref2(longest + 2, 0);
    for (int d = 1; d <= longest + 1; d++) {
        pref1[d] = pref1[d - 1] + depth_indices[d - 1].size();
        pref2[d] = pref2[d - 1] + nodes_at_depth[d - 1];
    }

    vector<int> a(pref1.back(), 0);
    for (int d = 1; d <= longest; d++)
        for (int i = 0; i < depth_indices[d].size(); i++) a[pref1[d] + i] = pref2[d] + depth_indices[d][i];

    int q;
    cin >> q;

    vector<array<int, 3>> qs(q);
    for (auto &[l, r, t] : qs) cin >> l >> r >> t;

    vector<int> indices;
    vector<array<int, 3>> queries;
    for (int i = 0; i < q; i++) {
        auto [ql, qr, qt] = qs[i];
        if (qt > longest || depth_indices[qt].empty()) continue;

        int l = lower_bound(word_indices[qt].begin(), word_indices[qt].end(), ql) - word_indices[qt].begin(),
            r = upper_bound(word_indices[qt].begin(), word_indices[qt].end(), qr) - word_indices[qt].begin();
        if (l < r) {
            l += pref1[qt];
            r += pref1[qt];
            indices.emplace_back(i);
            queries.push_back({l, r - 1, (int) indices.size() - 1});
        }
    }

    QueryDecomposition qd(a.size(), queries);
    auto answers = qd.mo(a, pref2.back());
    vector<int> count(q, 0);
    for (int i = 0; i < indices.size(); i++) count[indices[i]] = answers[i];
    for (int c : count) cout << c << "\n";
}
