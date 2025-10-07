#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    double sum = 0;
    vector<int> a(n);
    vector<double> pref(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];

        pref[i] += sum;
        sum = (sum + a[i]) / 2;
    }
    sum = 0;
    for (int i = n - 1; ~i; i--) {
        pref[i] += sum;
        sum = (sum + a[i]) / 2;
    }
    for (int i = 0; i < n; i++) cout << fixed << setprecision(5) << a[i] + pref[i] << " ";
}
