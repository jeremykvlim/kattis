#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    map<int, int> count;
    for (int i = 0; i < n; i++) {
        int l;
        cin >> l;

        count[l]++;
    }

    int l = count.begin()->first;
    if (l * count[l] + (l + 1) * count[l + 1] < n || count[100] && count[99] + count[100] != n) {
        cout << "impossible";
        exit(0);
    }

    cout << "possible";
}
