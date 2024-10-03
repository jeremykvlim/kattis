#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n == 1) {
        cout << "0\n";
        exit(0);
    }

    vector<int> t(n), v(n);
    int last = 0;
    for (int i = 0; i < n; i++) {
        cin >> t[i] >> v[i];

        last = max(last, t[i]);
    }

    auto len = [&](auto time) {
        double longest = 0, shortest = 1e20;
        for (int i = 0; i < n; i++) {
            longest = max(longest, (double) (time - t[i]) * v[i]);
            shortest = min(shortest, (double) (time - t[i]) * v[i]);
        }
        return longest - shortest;
    };

    double l = last, r = 1e11, mid1, mid2;
    while (l + 1e-12 < r && l + l * 1e-12 < r) {
        mid1 = l + (r - l) / 3, mid2 = r - (r - l) / 3;

        if (len(mid1) < len(mid2)) r = mid2;
        else l = mid1;
    }

    cout << fixed << setprecision(3) << len((mid1 + mid2) / 2);
}
