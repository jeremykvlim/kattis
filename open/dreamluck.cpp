#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, K;
    cin >> n >> K;

    if (K == 1) {
        cout << 1;
        exit(0);
    }

    vector<pair<int, int>> a(n);
    for (int i = 0; i < n; i++) {
        int ai;
        cin >> ai;

        a[i] = {ai, i};
    }
    sort(a.begin(), a.end());

    double luck = 0;
    for (int i = 0, j = 1; i < n; i = j++) {
        while (j < n && a[i].first == a[j].first) j++;

        vector<int> indices(j - i);
        for (int k = i; k < j; k++) indices[k - i] = a[k].second;

        int len = 0;
        for (int l = 0, r = 0; r < j - i; r++) {
            while (indices[r] - indices[l] + 1 >= K) l++;
            len = max(len, r - l + 1);
        }

        luck = max(luck, (double) len / K);
        if (indices.back() - indices.front() + 1 >= K) {
            double l = 0, r = 1, mid;
            while (l + 1e-8 < r && l + l * 1e-8 < r) {
                mid = l + (r - l) / 2;

                auto valid = [&]() {
                    double m = 1e20;
                    for (int l = 0, r = 0; r < j - i; r++) {
                        for (; l < j - i && indices[r] - indices[l] + 1 >= K; l++) m = min(m, l - mid * indices[l]);

                        if (m < 1e20 && m + mid - 1 <= r - mid * indices[r]) return true;
                    }
                    return false;
                };

                if (valid()) l = mid;
                else r = mid;
            }
            luck = max(luck, l);
        }
    }
    cout << fixed << setprecision(6) << luck;
}
