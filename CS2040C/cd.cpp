#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    while (cin >> n >> m && n && m) {
        unordered_set<int> cds;

        for (int i = 0; i < n + m; i++) {
            int number;
            cin >> number;
            cds.insert(number);
        }

        cout << n + m - cds.size() << endl;
    }
}
