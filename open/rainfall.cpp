#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    double c, d;
    cin >> T >> c >> d;

    vector<int> r(T), pref(T + 1);
    for (int i = 0; i < T; i++) {
        cin >> r[i];
        
        pref[i + 1] = pref[i] + r[i];
    }

    auto least = DBL_MAX;
    for (int i = 0; i < T; i++)
        for (int j = i; j < T; j++) {
            int s = max(r[i], r[j]);
            auto t = sqrt(c * d * 60 * d * 60 / s);
            if (t < (j - i) || t > (j - i + 1)) t = j - i + 1;
            least = min(c * d * 60 * d * 60 / t + pref[j + 1] - pref[i] - s * (j - i + 1 - t), least);
        }

    cout << fixed << setprecision(6) << least;
}
