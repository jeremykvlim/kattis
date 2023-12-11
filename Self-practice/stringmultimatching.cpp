#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n) {
        cin.ignore();
        vector<string> patterns(n);
        for (auto &l : patterns) getline(cin, l);
        string t;
        getline(cin, t);
        for (auto &l : patterns) {
            size_t i = 0;
            while ((i = t.find(l, i)) != string::npos) cout << i++ << " ";
            cout << "\n";
        }
    }
}
