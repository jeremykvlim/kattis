#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> h(n + 2);
    for (int i = 1; i <= n + 1; i++) cin >> h[i];

    while (k--) {
        double g;
        cin >> g;
        g *= 10;

        vector<double> H(n + 3);
        for (int i = 1; i <= n + 1; i++) H[i] = h[i] - g * i;
        H[0] = H[n + 2] = 1e20;

        vector<int> indices{1};
        vector<double> mono_h{-H[1]};
        double len = -1;
        for (int i = 2; i <= n + 1; i++) {
            int j = lower_bound(mono_h.begin(), mono_h.end(), -H[i]) - mono_h.begin();
            if (j < mono_h.size()) {
                j = indices[j];
                len = max(len, min(max((H[i] - H[j]) / abs(H[i + 1] - H[i]), (H[i] - H[j]) / abs(H[j] - H[j - 1])), 1.) + i - j);
            }

            if (mono_h.back() < -H[i]) {
                mono_h.emplace_back(-H[i]);
                indices.emplace_back(i);
            }
        }

        if (len == -1) cout << "impossible\n";
        else cout << fixed << setprecision(6) << len << "\n";
    }
}
