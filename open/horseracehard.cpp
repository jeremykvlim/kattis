#pragma GCC optimize("O3")
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(4e5), b(2e5);
    for (int i = 0; i < n; i++) {
        cin >> a[i];

        a[i + n] = a[i];
    }
    for (int i = 0; i < n; i++) cin >> b[i];

    vector<int> c(2e5);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) 
            c[j] += a[i + j] > b[i];

    vector<int> shifts;
    for (int i = 0; i < n; i++)
        if (c[(n - i) % n] * 2 > n) shifts.emplace_back(i);

    cout << shifts.size() << "\n";
    for (int s : shifts) cout << s << " ";
}
