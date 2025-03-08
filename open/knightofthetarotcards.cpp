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
    array<int, 5> card0;
    auto &[r0, c0, a0, b0, p0] = card0;
    cin >> n >> r0 >> c0 >> a0 >> b0 >> p0;

    if (!r0 && !c0) {
        cout << "0";
        exit(0);
    }

    auto reduce = [&](int a, int b) -> pair<int, int> {
        int g = __gcd(a, b);

        if (((a / g) & 1) && ((b / g) & 1)) return {2, g};
        return {1, g};
    };

    vector<array<int, 5>> cards(n - 1);
    vector<pair<int, int>> parity(n - 1);
    vector<long long> P(n - 1);
    gp_hash_table<pair<int, int>, long long, Hash> visited;
    visited[reduce(a0, b0)] = p0;
    for (int i = 0; i < n - 1; i++) {
        auto &[r, c, a, b, p] = cards[i];
        cin >> r >> c >> a >> b >> p;

        parity[i] = reduce(a, b);
        P[i] = p;
        if (r == r0 && c == c0) visited[parity[i]] = visited.find(parity[i]) != visited.end() ? min(visited[parity[i]], P[i]) : P[i];
    }

    priority_queue<pair<long long, pair<int, int>>, vector<pair<long long, pair<int, int>>>, greater<>> pq;
    for (auto [par, p] : visited) pq.emplace(p, par);
    while (!pq.empty()) {
        auto [cost, curr] = pq.top();
        pq.pop();

        auto reachable = [&](pair<int, int> p, int r = 0, int c = 0) {
            auto [type, g] = p;
            bool aligns = !((r - r0) % g) && !((c - c0) % g);

            if (type == 1) return aligns;
            return aligns && !(((r - r0) / g + (c - c0) / g) & 1);
        };

        if (reachable(curr)) {
            cout << cost;
            exit(0);
        }

        if (visited.find(curr) != visited.end() && visited[curr] < cost) continue;

        for (int i = 0; i < n - 1; i++) {
            if (!reachable(curr, cards[i][0], cards[i][1])) continue;

            int g = __gcd(curr.second, parity[i].second);

            pair<int, int> next;
            if (curr.first == parity[i].first) next = {curr.first, g};
            else {
                int a = curr.first == 1 ? curr.second : parity[i].second;

                if (!(a % (2 * g))) next = {2, g};
                else next = {1, g};
            }

            if (curr == next) continue;

            if (visited.find(next) == visited.end() || visited[next] > cost + P[i]) {
                visited[next] = cost + P[i];
                pq.emplace(cost + P[i], next);
            }
        }
    }
    cout << -1;
}