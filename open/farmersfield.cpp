#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, R;
    string s;
    cin >> n >> s >> R;

    vector<int> c;
    for (int i = 0; i < n; i++)
        if (s[i] == 'C') c.emplace_back(i);

    vector<int> pref(n + 1, 0);
    for (int i = 0; i < n; i++) pref[i + 1] = pref[i] + (s[i] == 'W' ? 1 : 0);

    for (int i = 1; i < c.size(); i++) {
        int l = c[i - 1], r = c[i];
        if (pref[r] > pref[l + 1]) 
            if (r - l < 2 * R + 2) {
                cout << "IMPOSSIBLE";
                exit(0);
            }
    }
    cout << "POSSIBLE";
}
