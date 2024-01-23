#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    int biggest = -1, count = 0;
    for (int i = 0; i < n; i++) {
        cin >> a[i];

        if (a[i] > biggest) {
            biggest = a[i];
            if (a[i] == i + 1) count++;
        }
    }

    cout << count;
}
