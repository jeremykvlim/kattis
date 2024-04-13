#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> h(k), m(k);
    for (int i = 0; i < k; i++) cin >> h[i] >> m[i];

    int switches = INT_MAX;
    for (int i = 0; i < k; i++) {
        auto temp_h = h, temp_m = m;
        temp_h[i] = temp_m[i] = -1;
        sort(temp_h.rbegin(), temp_h.rend());
        sort(temp_m.rbegin(), temp_m.rend());

        int n_h = n - h[i], n_m = n - m[i], s = 0;
        auto sw = [&](int n, vector<int> temp) {
            for (int j = 0; j < k && n > 0; j++) {
                n -= temp[j];

                if (n <= 0) s += j + 1;
            }
        };

        sw(n_h, temp_h);
        sw(n_m, temp_m);
        switches = min(switches, s);
    }

    cout << switches;
}
