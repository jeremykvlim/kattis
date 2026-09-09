#include <bits/stdc++.h>
using namespace std;

vector<int> z_function(const vector<int> &s) {
    int n = s.size();
    vector<int> z(n, 0);
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (i < r) z[i] = min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }
    return z;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n;
    cin >> m >> n;

    vector<int> p(m), h(n);
    for (int &pi : p) cin >> pi;
    for (int &hi : h) cin >> hi;

    vector<int> s;
    for (int i = 1; i < m; i++) s.emplace_back(p[i] - p[i - 1]);
    s.emplace_back(INT_MAX);
    for (int i = 1; i < n; i++) s.emplace_back(h[i] - h[i - 1]);
    auto z = z_function(s);

    vector<int> indices;
    for (int i = m; i < s.size(); i++)
        if (z[i] >= m - 1) indices.emplace_back(i - m);

    if (indices.empty()) cout << "tyndur";
    else
        for (int i : indices) cout << i << " ";
}
