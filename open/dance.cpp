#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    vector<bool> visited(n + 1, false);
    vector<vector<int>> seqs;
    unordered_map<int, vector<int>> indices;
    for (int i = 1; i <= n; i++) {
        if (visited[i]) continue;

        vector<int> s;
        int j = i;
        do {
            visited[j] = true;
            s.emplace_back(j);
            j = a[j];
        } while (j != i);

        seqs.emplace_back(s);
        indices[s.size()].emplace_back(seqs.size() - 1);
    }

    vector<int> next(n + 1);
    for (auto [size, ids] : indices) {
        int s = size;
        while (s != size * __gcd(s, k)) s = size * __gcd(s, k);

        int q = s / size;
        if (ids.size() % q) {
            cout << "Impossible";
            exit(0);
        }

        for (int i = 0; i < ids.size(); i += q) {
            vector<int> p(s);
            for (int j = 0; j < q && i + j < ids.size(); j++) 
                for (int l = 0, m = j; l < size; l++) {
                    p[m] = seqs[ids[i + j]][l];
                    m = (m + k) % s;
                }

            for (int j = 0; j < s; j++) next[p[j]] = p[(j + 1) % s];
        }
    }

    for (int i = 1; i <= n; i++) cout << next[i] << " ";
}
