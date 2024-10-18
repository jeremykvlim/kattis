#pragma GCC optimize("O3")
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    array<int, (int) 4e5> a;
    for (int i = 0; i < n; i++) {
        cin >> a[i];

        a[i + n] = a[i];
    }

    array<int, (int) 2e5> b;
    for (int i = 0; i < n; i++) cin >> b[i];

    array<int, (int) 2e5> c;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) 
            c[j] += a[i + j] > b[i];

    vector<int> shifts;
    for (int i = 0; i < n; i++)
        if (c[(n - i) % n] * 2 > n) shifts.emplace_back(i);

    cout << shifts.size() << "\n";
    for (int s : shifts) cout << s << " ";
}
