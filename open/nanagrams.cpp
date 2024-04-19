#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

long long ways(unordered_map<long long, vector<long long>> &adj_list, unordered_map<long long, long long> &count, long long s) {
    map<long long, long long> ways_len;
    unordered_map<long long, long long> ways_num{{s, 1}}, digits{{s, 0}};
    unordered_set<long long> visited;
    queue<long long> q;
    q.emplace(s);
    while (!q.empty()) {
        auto v = q.front();
        q.pop();

        if (visited.count(v)) continue;

        visited.emplace(v);
        ways_len[digits[v]] = (ways_len[digits[v]] + ways_num[v]) % MODULO;
        for (auto u : adj_list[v]) {
            if (!ways_num.count(u)) ways_num[u];
            ways_num[u] = (ways_num[u] + (ways_num[v] * count[u]) % MODULO) % MODULO;
            digits[u] = digits[v] + 1;

            q.emplace(u);
        }
    }

    return (*prev(ways_len.end())).second;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, a, b, c, d;
    cin >> n >> m >> a >> b >> c >> d;

    auto encode = [](long long n) -> long long {
        if (!n) return 1;

        auto e = 0LL;
        while (n) {
            e += 1LL << ((n % 10) << 2);
            n /= 10;
        }

        return e;
    };

    unordered_map<long long, long long> count;
    unordered_set<long long> seen;
    long long x = d;
    while (n--) {
        if (!seen.count(x)) count[encode(x)]++;

        seen.emplace(x);
        x = (a * x + b) % c;
    }

    auto make = [](long long xi, long long xj) -> bool {
        auto decode = [](long long x, int d) -> long long {
            return (x >> (d << 2)) & 15;
        };

        int digits_diff = 0;
        for (int d = 0; d <= 9; d++) {
            auto count_diff = decode(xj, d) - decode(xi, d);

            if (count_diff == 1) digits_diff++;
            if (count_diff < 0 || count_diff > 1 || digits_diff > 1) return false;
        }

        return digits_diff;
    };

    unordered_map<long long, vector<long long>> adj_list;
    for (auto [xi, ci] : count)
        for (auto [xj, cj] : count)
            if (xi != xj && make(xi, xj)) adj_list[xi].emplace_back(xj);

    unordered_map<long long, long long> cache;
    while (m--) {
        long long s;
        cin >> s;

        s = encode(s);
        if (cache.count(s)) {
            cout << cache[s] << "\n";
            continue;
        }

        cache[s] = ways(adj_list, count, s);
        cout << cache[s] << "\n";
    }
}
