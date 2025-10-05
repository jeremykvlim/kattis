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

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    auto impossible = [&]() {
        cout << "impossible";
        exit(0);
    };

    vector<int> a(n), b(n);
    for (int &ai : a) cin >> ai;
    for (int &bi : b) cin >> bi;

    if (n == 1) {
        if (!a[0] && !b[0]) {
            cout << "1 0";
            exit(0);
        }
        impossible();
    }

    int sum_a = 0, sum_b = 0;
    for (int i = 0; i < n; i++) {
        if (!(a[i] + b[i])) impossible();
        sum_a += a[i];
        sum_b += b[i];
    }

    if (sum_a != sum_b) impossible();

    int m = sum_a;
    if (m < n - 1) impossible();

    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) { return a[i] != a[j] ? a[i] > a[j] : i < j; });

    vector<int> version(n, 0);
    priority_queue<array<int, 4>> pq;
    for (int i = 0; i < n; i++)
        if (b[i]) pq.push({b[i], a[i], i, version[i]});

    vector<int> visited(n, -1);
    vector<pair<int, int>> edges;
    for (int u : order)
        if (a[u]) {
            vector<int> requeue1;
            auto process = [&](int i) {
                version[i]++;
                if (b[i]) requeue1.emplace_back(i);
            };

            while (a[u]--) {
                int v = -1;
                vector<int> requeue2;
                while (!pq.empty()) {
                    auto [bi, ai, i, ver] = pq.top();
                    pq.pop();

                    if (ver != version[i]) continue;

                    if (i == u || visited[i] == u) {
                        requeue2.emplace_back(i);
                        continue;
                    }
                    v = i;
                    break;
                }
                if (v == -1) impossible();

                for (int i : requeue2) pq.push({b[i], a[i], i, version[i]});
                edges.emplace_back(u + 1, v + 1);
                visited[v] = u;
                b[v]--;
                process(v);
            }
            process(u);
            for (int v : requeue1) pq.push({b[v], a[v], v, version[v]});
        }
    if (edges.size() != m) impossible();

    cout << n << " " << m << "\n";
    for (auto [u, v] : edges) cout << u << " " << v << "\n";
}
