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

    int n;
    cin >> n;

    vector<int> x(n);
    for (int &xi : x) cin >> xi;

    auto z = z_function(x);
    int len = n;
    for (int i = 1; i < n; i++)
        if (z[i] == n - i) {
            len = i;
            break;
        }

    cout << len << "\n";
    for (int i = 0; i < len; i++) cout << x[i] << " ";
}