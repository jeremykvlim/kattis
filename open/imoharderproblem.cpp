#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    auto k = 0.5 * count(s.begin(), s.end(), '?'), l = 0.;
    for (int i = 0; i < s.size(); i++)
        if (s[i] != 'T') l += (s[i] == 'H' ? 2 : 1) * (i + 1);
    cout << fixed << setprecision(6) << l - pow(count(s.begin(), s.end(), 'H') + k, 2) - 0.5 * k;
}
