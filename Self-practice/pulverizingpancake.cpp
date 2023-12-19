#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    string s;
    cin >> s;

    vector<bool> wild(n);
    for (int i = 0; i < n; i++) wild[i] = s[i] == '1';

    int count = 0;
    for (int i = 0; i < n; i++) {
        if (!wild[i]) continue;
        count++;
        wild[i] = false;
        if (i < n - 2) {
            wild[i + 2] = wild[i + 2] || wild[i + 1];
            wild[i + 1] = false;
        }
    }

    cout << count;
}
