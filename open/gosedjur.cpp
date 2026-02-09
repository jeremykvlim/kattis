#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] += v;
    }

    T pref_sum(int i) {
        T sum = 0;
        for (; i; i &= i - 1) sum += BIT[i];
        return sum;
    }

    T range_sum_query(int l, int r) {
        if (l >= r) return 0;
        return pref_sum(r) - pref_sum(l);
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, R, C;
    cin >> n >> R >> C;

    vector<int> col_l(n, C), col_r(n, -1), prev(C), curr(C), degree(n, 0);
    vector<vector<int>> adj_list(n);
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            int aij;
            cin >> aij;
            aij--;

            curr[j] = aij;
            col_l[aij] = min(col_l[aij], j);
            col_r[aij] = max(col_r[aij], j);

            if (i && prev[j] != aij) {
                adj_list[prev[j]].emplace_back(aij);
                degree[aij]++;
            }
        }
        prev = curr;
    }

    priority_queue<pair<int, int>> pq;
    int count = 0;
    auto temp = degree;
    vector<int> l(n, -1);
    for (int i = 0; i < n; i++)
        if (!degree[i]) pq.emplace(-col_l[i], i);

    while (!pq.empty()) {
        int v = pq.top().second;
        pq.pop();

        if (l[v] != -1) continue;

        l[v] = count++;
        for (int u : adj_list[v])
            if (!--degree[u]) pq.emplace(-col_l[u], u);
    }

    count = 0;
    degree = temp;
    vector<int> r(n, -1);
    for (int i = 0; i < n; i++)
        if (!degree[i] && ~l[i]) pq.emplace(col_r[i], i);

    while (!pq.empty()) {
        int v = pq.top().second;
        pq.pop();

        if (r[v] != -1) continue;

        r[v] = count++;
        for (int u : adj_list[v])
            if (!--degree[u] && ~l[u]) pq.emplace(col_r[u], u);
    }

    vector<int> animals;
    for (int x = 0; x < n; x++)
        if (l[x] != -1) animals.emplace_back(x);
    sort(animals.begin(), animals.end(), [&](int x, int y) { return l[x] < l[y]; });

    FenwickTree<int> fw(animals.size() + 1);
    vector<int> moves(n, 0);
    for (int x : animals) {
        fw.update(r[x] + 1, 1);
        moves[x] = fw.pref_sum(r[x] + 1);
    }

    for (int x = 0; x < n; x++)
        if (!~l[x]) cout << "-1 ";
        else cout << moves[x] - 1 << " ";
}
