#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    while (cin >> n) {
        cin.ignore();

        vector<string> p(n);
        for (auto &l : p) getline(cin, l);

        string t;
        getline(cin, t);

        for (auto &l : p) {
            int i = 0;
            while ((i = t.find(l, i)) != string::npos) cout << i++ << " ";
            cout << "\n";
        }
    }
}
