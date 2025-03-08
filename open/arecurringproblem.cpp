#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires {tuple_size<T>::value;})
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires {declval<T>().begin(); declval<T>().end();} && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    unordered_map<pair<vector<long long>, vector<long long>>, map<long long, long long>, Hash> memo;
    auto dfs1 = [&](auto &&self, vector<long long> seq, vector<long long> start) -> map<long long, long long> {
        if (memo.count({seq, start})) return memo[{seq, start}];
        if (!seq[0]) {
            if (all_of(seq.begin(), seq.end(), [](auto e) {return !e;})) return memo[{seq, start}] = {{0, 1}};
            else return memo[{seq, start}] = {};
        }

        if (seq.size() > 1)
            if (self(self, {seq.begin(), seq.end() - 1}, {start.begin(), start.end() - 1}).empty()) return memo[{seq, start}] = {};

        map<long long, long long> m;
        for (int ci = 1; ci <= seq[0]; ci++)
            for (int ai = 1; ci * ai <= seq[0]; ai++) {
                vector<long long> s = seq, st{ai};
                st.insert(st.end(), start.begin(), start.end() - 1);
                for (int k = 0; k < seq.size(); k++) {
                    if (s[k] < ci * st[k]) goto next;
                    s[k] -= ci * st[k];
                }

                for (auto [e, freq] : self(self, s, st)) m[e + ci * start.back()] += freq;
                next:;
            }
        return memo[{seq, start}] = m;
    };

    vector<long long> seq;
    for (int i = 1;; i++) {
        auto total = 0LL;
        for (auto [e, freq] : dfs1(dfs1, {i}, {i})) total += freq;

        if (n <= total) {
            seq.emplace_back(i);
            break;
        }
        n -= total;
    }

    do {
        for (auto [e, freq] : dfs1(dfs1, seq, seq)) {
            if (n <= freq) {
                seq.emplace_back(e);
                break;
            }
            n -= freq;
        }
    } while (n > 1e5);
    dfs1(dfs1, seq, seq);

    vector<array<vector<long long>, 3>> rec;
    auto dfs2 = [&](auto &&self, vector<long long> seq, vector<long long> start, vector<long long> c = {}, vector<long long> a = {}) {
        if (!memo.count({seq, start}) || memo[{seq, start}].empty()) return;
        if (!seq[0] && !memo[{seq, start}].empty()) {
            reverse(c.begin(), c.end());
            reverse(a.begin(), a.end());

            int k = a.size();
            vector<long long> s;
            while (s.size() < 20) {
                auto si = 0LL;
                for (int i = 0; i < k; i++) {
                    if (s.size() + i < k) si += c[i] * a[s.size() + i];
                    else si += c[i] * s[s.size() + i - k];
                }
                s.emplace_back(si);
            }
            rec.push_back({c, a, s});
            return;
        }

        for (int ci = 1; ci <= seq[0]; ci++)
            for (int ai = 1; ci * ai <= seq[0]; ai++) {
                vector<long long> s = seq, st{ai};
                st.insert(st.end(), start.begin(), start.end() - 1);
                for (int k = 0; k < seq.size(); k++) {
                    if (s[k] < ci * st[k]) goto next;
                    s[k] -= ci * st[k];
                }

                c.emplace_back(ci);
                a.emplace_back(ai);
                self(self, s, st, c, a);
                c.pop_back();
                a.pop_back();
                next:;
            }
    };
    dfs2(dfs2, seq, seq);

    sort(rec.begin(), rec.end(), [](auto a1, auto a2) {return a1[2] != a2[2] ? a1[2] < a2[2] : a1[0] < a2[0];});
    auto [c, a, s] = rec[n - 1];
    s.resize(10);
    cout << c.size() << "\n";
    for (auto ci : c) cout << ci << " ";
    cout << "\n";
    for (auto ai : a) cout << ai << " ";
    cout << "\n";
    for (auto si : s) cout << si << " ";
}
