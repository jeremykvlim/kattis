#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<tuple<int, int, long double>> edges;
    unordered_map<string, int> items{{"Emerald", 0}};
    int count = 1;
    while (n--) {
        int t;
        cin >> t;

        while (t--) {
            int a1, a2;
            string s1, s2;
            cin >> a1 >> s1 >> a2 >> s2;

            if (!items.count(s1)) items[s1] = count++;
            if (!items.count(s2)) items[s2] = count++;
            edges.emplace_back(items[s2], items[s1], (long double) log(a1) - log(a2));
        }
    }

    vector<long double> dist(count--, LDBL_MAX);
    dist[0] = 0;
    while (count--)
        for (auto [u, v, w] : edges)
            if (dist[u] != LDBL_MAX) dist[v] = min(dist[v], dist[u] + w);

    for (auto [u, v, w] : edges)
        if (dist[u] != LDBL_MAX && dist[u] + w < dist[v]) {
            cout << "yes";
            exit(0);
        }

    cout << "no";
}
