#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    cin.ignore(2);

    while (t--) {
        int total = 0;
        map<string, int> count;
        string s;
        while (getline(cin, s) && !s.empty()) {
            count[s]++;
            total++;
        }

        for (auto [song, f] : count) cout << fixed << setprecision(4) << song << " " << 100. * f / total << "\n";
        cout << "\n";
    }
}
