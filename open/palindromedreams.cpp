#include <bits/stdc++.h>
using namespace std;

bool ispalindrome(const string &s) {
    return s == string(s.rbegin(), s.rend());
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> pref(1e7 + 1, 0);
    for (int i = 1; i <= 1e7; i++) pref[i] = pref[i - 1] + ispalindrome(to_string(i));

    while (n--) {
        int a, b;
        cin >> a >> b;
        cout << pref[b] - pref[a - 1] << "\n";
    }
}
