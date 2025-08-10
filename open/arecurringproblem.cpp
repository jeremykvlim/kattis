#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires { tuple_size<T>::value; })
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires { declval<T>().begin(); declval<T>().end(); } && !is_same_v<T, string>) {
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

    vector<long long> seq;
    vector<int> divisor_count(25, 0), dp(25, 0);
    dp[0] = 1;
    for (int i = 1; i < 25; i++) {
        for (int j = i; j < 25; j += i) divisor_count[j]++;
        for (int j = 1; j <= i; j++) dp[i] += divisor_count[j] * dp[i - j];

        if (n > dp[i]) n -= dp[i];
        else {
            seq.emplace_back(i);
            break;
        }
    }

    auto search = [&](const auto &v, auto e) -> bool {
        auto it = lower_bound(v.begin(), v.end(), e, [](const auto &p, auto x) { return p.first < x; });
        return it != v.end() && it->first == e;
    };

    vector<pair<long long, long long>> empty, base{{0, 1}};
    cc_hash_table<pair<vector<long long>, vector<long long>>, vector<pair<long long, long long>>, Hash> memo;
    auto dfs1 = [&](auto &&self, const vector<long long> &seq, const vector<long long> &start) -> const vector<pair<long long, long long>> & {
        if (!seq[0]) {
            if (all_of(seq.begin(), seq.end(), [](auto e) { return !e; })) return base;
            return empty;
        }

        if (any_of(seq.begin(), seq.end(), [](auto e) { return !e; })) return empty;

        if (seq.size() > 1) {
            auto temp_seq = seq, temp_start = start;
            temp_seq.pop_back();
            temp_start.pop_back();
            auto it = memo.find({temp_seq, temp_start});
            if (it == memo.end() || !search(it->second, seq.back())) return empty;
        }
        if (memo.find({seq, start}) != memo.end()) return memo[{seq, start}];

        int c_max = seq[0];
        if (seq.size() > 1)
            for (int i = 1; i < seq.size(); i++)
                if (start[i - 1]) c_max = min((long long) c_max, seq[i] / start[i - 1]);
        if (!c_max) return empty;

        vector<long long> st{0};
        st.insert(st.end(), start.begin(), start.end());
        gp_hash_table<long long, long long> m;
        for (int ci = 1; ci <= c_max; ci++) {
            auto s = seq;
            for (int k = 1; k < seq.size(); k++) {
                if (s[k] < ci * st[k]) goto next;
                s[k] -= ci * st[k];
            }

            for (int ai = 1; ai * ci <= seq[0]; ai++) {
                s[0] = seq[0] - ai * ci;
                st[0] = ai;
                st.pop_back();
                for (auto [e, freq] : self(self, s, st)) m[e + ci * start.back()] += freq;
                st.emplace_back(start.back());
            }
            next:;
        }

        vector<pair<long long, long long>> v{m.begin(), m.end()};
        sort(v.begin(), v.end());
        return memo[{seq, start}] = v;
    };

    bool change = true;
    while (seq.size() < 8 && change) {
        change = false;
        for (auto [e, freq] : dfs1(dfs1, seq, seq)) {
            if (n <= freq) {
                seq.emplace_back(e);
                change = true;
                break;
            }
            n -= freq;
        }
    }
    dfs1(dfs1, seq, seq);

    vector<array<vector<long long>, 3>> rec;
    auto dfs2 = [&](auto &&self, const vector<long long> &seq, const vector<long long> &start, vector<long long> c = {}, vector<long long> a = {}) {
        if (!seq[0]) {
            if (any_of(seq.begin(), seq.end(), [](auto e) { return e; })) return;

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
            rec.push_back({s, c, a});
            return;
        }

        if (any_of(seq.begin(), seq.end(), [](auto e) { return !e; })) return;

        if (seq.size() > 1) {
            auto temp_seq = seq, temp_start = start;
            temp_seq.pop_back();
            temp_start.pop_back();
            auto it = memo.find({temp_seq, temp_start});
            if (it == memo.end() || !search(it->second, seq.back())) return;
        }

        int c_max = seq[0];
        if (seq.size() > 1)
            for (int i = 1; i < seq.size(); i++)
                if (start[i - 1]) c_max = min((long long) c_max, seq[i] / start[i - 1]);
        if (!c_max) return;

        vector<long long> st{0};
        st.insert(st.end(), start.begin(), start.end());
        for (int ci = 1; ci <= c_max; ci++) {
            auto s = seq;
            for (int k = 1; k < seq.size(); k++) {
                if (s[k] < ci * st[k]) goto next;
                s[k] -= ci * st[k];
            }

            for (int ai = 1; ai * ci <= seq[0]; ai++) {
                s[0] = seq[0] - ai * ci;
                st[0] = ai;
                st.pop_back();
                c.emplace_back(ci);
                a.emplace_back(ai);
                self(self, s, st, c, a);
                a.pop_back();
                c.pop_back();
                st.emplace_back(st.back());
            }
            next:;
        }
    };
    dfs2(dfs2, seq, seq);

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
