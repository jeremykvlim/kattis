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

    int n, D, M;
    cin >> n >> D >> M;

    vector<int> arr(n);
    for (int &e : arr) cin >> e;

    D = min(D, n - 1);
    vector<vector<tuple<int, int, int, int, bool>>> adj_list(1e6);
    gp_hash_table<vector<int>, int, Hash> cache;
    auto jump = [&](int v) -> void {
        enum State {
            UNVISITED = 0,
            VISITED = 1,
            PATH = 2,
            LOOP1 = 3,
            LOOP2 = 4
        };

        int w = v;
        vector<int> states;
        while (w) {
            states.emplace_back(w % 5);
            w /= 5;
        }
        states.resize(max(D + 1, (int) states.size()), UNVISITED);

        vector<int> path, unvisited;
        gp_hash_table<int, pair<int, int>> loop;
        for (int i = 0; i < states.size(); i++)
            if (states[i] == UNVISITED) unvisited.emplace_back(i);
            else if (states[i] == PATH) path.emplace_back(i);
            else if (states[i] >= LOOP1) {
                if (loop.find(states[i]) != loop.end()) loop[states[i]].second = i;
                else loop[states[i]] = {i, -1};
            }

        auto encode = [&](vector<int> states) {
            if (cache.find(states) != cache.end()) return cache[states];

            gp_hash_table<int, int> indices;
            vector<pair<int, int>> matches;
            for (int i = 0; i < states.size(); i++)
                if (states[i] >= LOOP1) {
                    if (indices.find(states[i]) != indices.end()) matches.emplace_back(indices[states[i]], i);
                    else indices[states[i]] = i;
                }

            if (!matches.empty()) {
                sort(matches.begin(), matches.end());
                int s = matches.size() + 2;
                for (auto [i, j] : matches) states[i] = states[j] = s--;
            }

            int v = 0;
            for (int i = states.size() - 1; ~i; i--) v = v * 5 + states[i];
            return cache[states] = v;
        };

        vector<int> temp;
        for (int i : path)
            for (int j : unvisited) {
                temp = states;
                temp[i] = VISITED;
                temp[j] = PATH;
                adj_list[v].emplace_back(encode(temp), 1, min(i, j), max(i, j), false);
            }

        if (loop.size() < 2)
            for (int i : unvisited)
                for (int j : unvisited) {
                    if (i == j) continue;

                    for (int s : {LOOP1, LOOP2})
                        if (loop.find(s) == loop.end()) {
                            temp = states;
                            temp[i] = temp[j] = s;
                            adj_list[v].emplace_back(encode(temp), 2, i, j, false);
                            break;
                        }
                }
        else
            for (auto [s1, p1] : loop)
                for (auto [s2, p2] : loop) {
                    if (s1 == s2) continue;

                    for (auto [i1, j1] : {p1, {p1.second, p1.first}})
                        for (auto [i2, j2] : {p2, {p2.second, p2.first}}) {
                            temp = states;
                            temp[i1] = temp[i2] = VISITED;
                            temp[j1] = temp[j2] = s1;
                            adj_list[v].emplace_back(encode(temp), 0, min(i1, i2), max(i1, i2), false);
                        }
                }

        for (int i : path)
            for (auto [s, p] : loop)
                for (auto [j, k] : {p, {p.second, p.first}}) {
                    temp = states;
                    temp[i] = temp[j] = VISITED;
                    temp[k] = PATH;
                    adj_list[v].emplace_back(encode(temp), 0, min(i, j), max(i, j), false);
                }

        if (path.size() < 2)
            for (int i : unvisited) {
                temp = states;
                temp[i] = PATH;
                adj_list[v].emplace_back(encode(temp), 1, -1, -1, false);
            }
        else if (path.size() == 2) {
            temp = states;
            temp[path[0]] = temp[path[1]] = VISITED;

            int u = encode(temp);
            while (u) {
                if (u % 5 > 1) goto next;
                u /= 5;
            }

            adj_list[v].emplace_back(u, 0, path[0], path[1], true);
        }

        next:;
        for (int i : unvisited)
            for (auto [s, p] : loop)
                for (int j : {p.first, p.second}) {
                    temp = states;
                    temp[i] = s;
                    temp[j] = VISITED;
                    adj_list[v].emplace_back(encode(temp), 1, min(i, j), max(i, j), false);
                }

        sort(adj_list[v].begin(), adj_list[v].end());
        adj_list[v].erase(unique(adj_list[v].begin(), adj_list[v].end()), adj_list[v].end());
    };

    vector<bool> visited(1e6, false);
    vector<int> visits;
    queue<int> q;
    q.emplace(0);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        visits.emplace_back(v);
        jump(v);
        for (auto [u, jumps, src, dest, end] : adj_list[v])
            if (!visited[u]) {
                visited[u] = true;
                q.emplace(u);
            }
    }

    while (!visits.empty()) {
        int v = visits.back();
        visits.pop_back();

        visited[v] = false;
    }

    vector<vector<int>> dp(2, vector<int>(1e6, 0));
    int len = -1;
    for (int i = 0; i + D < n; i++) {
        q.emplace(0);
        for (int v : visits) {
            if (v % 5 > 1) continue;

            int u = v / 5;
            dp[i & 1][u] = max(dp[i & 1][u], dp[(i & 1) ^ 1][v]);

            if (!visited[u]) {
                visited[u] = true;
                q.emplace(u);
            }
        }

        while (!visits.empty()) {
            int v = visits.back();
            visits.pop_back();

            dp[(i & 1) ^ 1][v] = 0;
        }

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            visits.emplace_back(v);
            for (auto [u, jumps, src, dest, end] : adj_list[v])
                if (abs(arr[i + src] - arr[i + dest]) <= M) {
                    if (end) {
                        len = max(len, dp[i & 1][v]);
                        continue;
                    }

                    dp[i & 1][u] = max(dp[i & 1][u], dp[i & 1][v] + jumps);
                    if (!visited[u]) {
                        visited[u] = true;
                        q.emplace(u);
                    }
                }
        }

        for (int v : visits) visited[v] = false;
    }

    cout << len;
}
