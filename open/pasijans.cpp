#include <bits/stdc++.h>
using namespace std;

vector<int> duval(const vector<int> &s) {
    int n = s.size();

    vector<int> factors;
    for (int i = 0; i < n;) {
        int j = i + 1, k = i;
        for (; j < n && s[j] >= s[k]; j++)
            if (s[j] > s[k]) k = i;
            else k++;

        for (; i <= k; i += j - k) factors.emplace_back(i);
    }
    factors.emplace_back(n);
    return factors;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> seqs(n);
    vector<array<int, 3>> f;
    for (int i = 0; i < n; i++) {
        int L;
        cin >> L;

        seqs[i].resize(L);
        for (int &e : seqs[i]) cin >> e;

        auto temp = seqs[i];
        for (int &e : temp) e = -e;
        auto factors = duval(temp);
        for (int j = 0; j < factors.size() - 1; j++) f.push_back({i, factors[j], factors[j + 1]});
    }
    sort(f.begin(), f.end(), [&](const auto &a1, const auto &a2) {
        auto [i1, l1, r1] = a1;
        auto [i2, l2, r2] = a2;
        for (int j = 0; j < min(r1 - l1, r2 - l2); j++)
            if (seqs[i1][j + l1] != seqs[i2][j + l2]) return seqs[i1][j + l1] < seqs[i2][j + l2];
        return r1 - l1 > r2 - l2;
    });

    for (auto [i, l, r] : f)
        for (int j = l; j < r; j++) cout << seqs[i][j] << " ";
}
