#include <bits/stdc++.h>
using namespace std;

struct Trie {
    struct TrieNode {
        vector<tuple<int, long long, long long>> next;

        TrieNode() = default;
    };

    vector<TrieNode> T;

    Trie(int n = 1) : T(n) {}

    int add(const auto &seq) {
        int v = 0;
        for (int i = seq.size() - 1; ~i; i--) {
            auto [a, b] = seq[i];
            int u = -1;
            for (auto [t, x, y] : T[v].next)
                if (x == a && y == b) {
                    u = t;
                    break;
                }

            if (!~u) {
                T.emplace_back();
                u = T.size() - 1;
                T[v].next.emplace_back(u, a, b);
            }
            v = u;
        }
        return v;
    }

    int find(int v, long long a, long long b) const {
        for (auto [u, x, y] : T[v].next)
            if (x == a && y == b) return u;
        return -1;
    }

    int size() {
        return T.size();
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

    vector<pair<long long, long long>> base;
    vector<int> divisor_count(25, 0), dp(25, 0);
    dp[0] = 1;
    for (int i = 1; i < 25; i++) {
        for (int j = i; j < 25; j += i) divisor_count[j]++;
        for (int j = 1; j <= i; j++) dp[i] += divisor_count[j] * dp[i - j];

        if (n > dp[i]) n -= dp[i];
        else {
            base.emplace_back(i, i);
            break;
        }
    }

    auto search = [&](const auto &freq, auto e) -> bool {
        auto it = lower_bound(freq.begin(), freq.end(), e, [](const auto &p, auto x) { return p.first < x; });
        return it != freq.end() && it->first == e;
    };

    Trie trie;
    int node = trie.add(base);
    vector<vector<pair<long long, int>>> freq(trie.size());
    auto dfs1 = [&](auto &&self, const auto &curr, int v) -> void {
        if (!freq[v].empty()) return;

        int c_max = curr[0].first;
        for (int i = 1; i < curr.size(); i++) c_max = min((long long) c_max, curr[i].first / curr[i - 1].second);

        vector<pair<long long, long long>> next(curr.size());
        vector<pair<long long, int>> values;
        for (int ci = 1; ci <= c_max; ci++) {
            for (int i = 1; i < curr.size(); i++) {
                next[i].first = curr[i].first - curr[i - 1].second * ci;
                next[i].second = curr[i - 1].second;
            }

            int p = 0;
            for (int i = curr.size() - 2; i > 0; i--) {
                p = trie.find(p, next[i].first, next[i].second);
                if (!~p) break;
            }

            auto add = curr.back().second * ci;
            for (int ai = 1; ai * ci <= curr[0].first; ai++) {
                next[0] = {curr[0].first - ai * ci, ai};
                if (!next[0].first) {
                    if (all_of(next.begin() + 1, next.end(), [](auto e) { return !e.first; })) values.emplace_back(add, 1);
                    continue;
                }
                if (!~p) continue;

                if (curr.size() >= 2) {
                    int t = trie.find(p, next[0].first, ai);
                    if (!~t || freq[t].empty() || !search(freq[t], next.back().first)) continue;
                }

                int u = trie.add(next);
                freq.resize(trie.size());
                self(self, next, u);
                for (auto [e, f] : freq[u]) values.emplace_back(e + add, f);
            }
        }
        sort(values.begin(), values.end());

        for (auto [e, f] : values)
            if (freq[v].empty() || freq[v].back().first != e) freq[v].emplace_back(e, f);
            else freq[v].back().second += f;
    };

    while (base.size() < 8) {
        dfs1(dfs1, base, node);
        for (auto [e, f] : freq[node])
            if (n > f) n -= f;
            else {
                base.emplace_back(e, e);
                if (base.size() < 8) {
                    node = trie.add(base);
                    freq.resize(trie.size());
                }
                break;
            }
    }

    vector<long long> C, A;
    vector<array<vector<long long>, 3>> rec;
    auto dfs2 = [&](auto &&self, const auto &curr) -> void {
        if (!curr[0].first) {
            if (any_of(curr.begin(), curr.end(), [](auto e) { return e.first; })) return;

            auto c = C, a = A;
            reverse(c.begin(), c.end());
            reverse(a.begin(), a.end());

            int k = a.size();
            vector<long long> s;
            while (s.size() < 20) {
                auto si = 0LL;
                for (int i = 0; i < k; i++)
                    if (s.size() + i < k) si += c[i] * a[s.size() + i];
                    else si += c[i] * s[s.size() + i - k];
                s.emplace_back(si);
            }
            rec.push_back({s, c, a});
            return;
        }

        int c_max = curr[0].first;
        for (int i = 1; i < curr.size(); i++) c_max = min((long long) c_max, curr[i].first / curr[i - 1].second);

        vector<pair<long long, long long>> next(curr.size());
        for (int ci = 1; ci <= c_max; ci++) {
            for (int i = 1; i < curr.size(); i++) {
                next[i].first = curr[i].first - curr[i - 1].second * ci;
                next[i].second = curr[i - 1].second;
            }

            int p = 0;
            for (int i = curr.size() - 2; i > 0; i--) {
                p = trie.find(p, next[i].first, next[i].second);
                if (!~p) break;
            }

            for (int ai = 1; ai * ci <= curr[0].first; ai++) {
                next[0] = {curr[0].first - ai * ci, ai};
                if (next[0].first) {
                    if (!~p) continue;

                    if (curr.size() >= 2) {
                        int t = trie.find(p, next[0].first, ai);
                        if (!~t || freq[t].empty() || !search(freq[t], next.back().first)) continue;
                    }
                }

                C.emplace_back(ci);
                A.emplace_back(ai);
                self(self, next);
                A.pop_back();
                C.pop_back();
            }
        }
    };
    dfs2(dfs2, base);

    nth_element(rec.begin(), rec.begin() + n - 1, rec.end());
    auto [s, c, a] = rec[n - 1];
    s.resize(10);
    cout << c.size() << "\n";
    for (auto ci : c) cout << ci << " ";
    cout << "\n";
    for (auto ai : a) cout << ai << " ";
    cout << "\n";
    for (auto si : s) cout << si << " ";
}