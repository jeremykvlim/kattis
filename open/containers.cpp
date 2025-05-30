#include <bits/stdc++.h>
using namespace std;

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
    cin.ignore();

    while (n--) {
        string m;
        getline(cin, m);

        istringstream iss(m);
        vector<string> pourings{istream_iterator<string>{iss}, istream_iterator<string>{}};

        auto parse = [&](const auto &container) -> pair<int, int> {
            int p = container[0] - '0', b = container[1] - '0';
            if (p == b) p = 0;
            return {p, b};
        };
        auto query = parse(pourings.back());
        pourings.pop_back();

        unordered_map<pair<int, int>, int, Hash> volume;
        for (auto p : pourings) {
            int i = p.find('-');
            auto s = p.substr(0, i), d = p.substr(i + 1);

            pair<int, int> src, dest;
            if (!s.empty()) {
                src = parse(s);
                if (!volume.count(src)) volume[src] = 0;
            }
            if (!d.empty()) {
                dest = parse(d);
                if (!volume.count(dest)) volume[dest] = 0;
            }
            
            auto [ps, bs] = src;
            auto [pd, bd] = dest;

            if (s.empty()) volume[dest] = volume[dest] < pd ? pd : bd;
            else if (d.empty()) volume[src] = volume[src] > ps ? ps : 0;
            else {
                int vs = volume[src], vd = volume[dest], poured = min(vs, (vd < pd ? pd : bd) - vd);
                volume[src] = vs - poured;
                volume[dest] = vd + poured;
            }
        }
        cout << volume[query] << "\n";
    }
}
