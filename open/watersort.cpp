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

    vector<string> grid(4, string("", n));
    for (auto &row : grid)
        for (char &c : row) cin >> c;

    vector<string> src(n);
    for (int c = 0; c < n; c++)
        for (int r = 3; ~r; r--) src[c] += grid[r][c];
    src.resize(n + 2);
    sort(src.begin(), src.end());

    auto solved = [&](const auto &v) {
        for (auto &s : v)
            if (!s.empty())
                if (s.size() != 4 || adjacent_find(s.begin(), s.end(), not_equal_to<>()) != s.end()) return false;
        return true;
    };

    if (solved(src)) {
        cout << 0;
        exit(0);
    }

    auto h = [&](const auto &v) {
        int h = 0;
        for (auto &s : v)
            if (!s.empty()) h += s.size() - (find_if(s.begin(), s.end(), [&](char c) { return c != s[0]; }) - s.begin());
        return h;
    };

    unordered_map<vector<string>, int, Hash> dist;
    dist[src] = 0;
    array<deque<pair<int, vector<string>>>, 9> buckets;
    buckets[0].emplace_back(0, src);
    for (int b = h(src);;) {
        while (buckets[0].empty()) {
            b++;
            for (int i = 0; i + 1 < 9; i++) buckets[i].swap(buckets[i + 1]);
        }

        auto [d, v] = buckets[0].front();
        buckets[0].pop_front();

        if (d != dist[v]) continue;

        if (solved(v)) {
            cout << d;
            exit(0);
        }

        int empty = -1;
        for (int i = 0; i < n + 2; i++)
            if (v[i].empty()) {
                empty = i;
                break;
            }

        for (int i = 0; i < n + 2; i++)
            if (!v[i].empty()) {
                char c = v[i].back();
                int freq = 0;
                for (int j = v[i].size() - 1; ~j && v[i][j] == c; j--) freq++;
                if (!freq) continue;

                for (int j = 0; j < n + 2; j++)
                    if (i != j && v[j].size() != 4) {
                        if (v[j].empty()) {
                            if (~empty && j != empty || freq == v[i].size()) continue;
                        } else if (v[j].back() != c) continue;

                        int w = min(freq, 4 - (int) v[j].size());
                        auto u = v;
                        u[i].resize(v[i].size() - w);
                        u[j].append(w, c);
                        sort(u.begin(), u.end());
                        if (!dist.count(u) || dist[u] > d + w) {
                            dist[u] = d + w;
                            buckets[d + w + h(u) - b].emplace_back(d + w, u);
                        }
                    }
            }
    }
}
