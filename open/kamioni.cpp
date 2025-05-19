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

    int n, m;
    cin >> n >> m;

    vector<int> k(n + 1);
    vector<vector<int>> A(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> k[i];

        A[i].resize(k[i]);
        for (int &Aj : A[i]) cin >> Aj;
    }

    vector<int> count(m + 1, 0), state(m + 1);
    vector<vector<pair<int, int>>> queries(n + 1);
    unordered_map<pair<int, int>, int, Hash> seen;
    for (int i = 1; i <= m; i++) {
        int a, b;
        cin >> a >> b;
        if (a > b) swap(a, b);

        if (seen.count({a, b})) {
            count[i] = seen[{a, b}];
            continue;
        }
        seen[{a, b}] = i;
        if (make_pair(k[a], a) > make_pair(k[b], b)) swap(a, b);
        state[i] = A[a][0] > A[b][0];
        queries[a].emplace_back(b, i);
    }

    vector<int> dir(n + 1), ord(n + 1);
    vector<tuple<long long, int, int>> events;
    for (int a = 1; a <= n; a++) {
        auto t = 0LL;
        ord[a] = A[a][0];
        dir[a] = A[a][1] > A[a][0] ? 1 : -1;
        for (int j = 1; j < k[a]; j++) {
            t += abs(A[a][j] - A[a][j - 1]);
            events.emplace_back(t, a, j);
        }
    }
    sort(events.begin(), events.end());

    vector<long long> time(n + 1, 0);
    for (auto [t, a, j] : events) {
        for (auto [b, i] : queries[a])
            if (~state[i]) {
                auto x = A[a][j], y = ord[b];

                auto d = t - time[b];
                if (dir[b]) y += dir[b] == 1 ? d : -d;
                else x -= dir[a] == 1 ? d : -d;

                count[i] -= state[i] != x > y;
                state[i] = dir[b] ? x > y : -1;
            }
        time[a] = t;
        ord[a] = A[a][j];
        dir[a] = k[a] == j + 1 ? 0 : A[a][j + 1] > A[a][j] ? 1 : -1;
    }

    for (int i = 1; i <= m; i++) cout << (count[i] > 0 ? -count[count[i]] : -count[i]) << "\n";
}
