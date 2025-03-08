#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &seed, const T &v) {
        seed ^= Hash{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template <typename ... Ts>
    static size_t seed_value(const Ts & ... args) {
        size_t seed = 0;
        (combine(seed, args), ...);
        return seed;
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires {tuple_size<T>::value;})
            return apply([](const auto & ... elems) {return seed_value(elems...);}, v);
        else if constexpr (requires {declval<T>().begin(); declval<T>().end();} && !is_same_v<T, string>) {
            size_t seed = 0;
            for (const auto &e : v) combine(seed, e);
            return seed;
        } else return hash<T>{}(v);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<string> words(n);
    for (auto &w : words) cin >> w;
    sort(words.begin(), words.end());
    words.erase(unique(words.begin(), words.end()), words.end());

    unordered_map<tuple<string, string, int, int>, int, Hash> pairs;
    for (int i = 0; i < words.size(); i++)
        for (int j = i + 1; j < words.size(); j++) {
            if (words[i].size() != words[j].size()) continue;

            vector<int> diff;
            for (int k = 0; k < words[i].size(); k++)
                if (words[i][k] != words[j][k]) diff.emplace_back(k);

            if (diff.size() == 2 && diff[0] + 1 == diff[1]) {
                auto s1 = words[i].substr(diff[0], 2), s2 = words[j].substr(diff[0], 2);
                pairs[{min(s1, s2), max(s1, s2), diff[0], words[i].size()}]++;
            }
        }

    int count = 0;
    for (auto [p, c] : pairs)
        if (c == 1) count++;

    cout << count;
}
