#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    while (n--) {
        double X;
        int k;
        cin >> X >> k;

        vector<double> f(k);
        for (auto &fi : f) cin >> fi;
        sort(f.begin(), f.end());

        vector<int> indices(k, 0);
        vector<pair<double, bool>> states{{1, true}, {1e3, false}};
        for (double x = 1; x <= X;) {
            bool change;
            do {
                change = false;
                for (int i = 0; i < k; i++) {
                    auto xf = x * f[i];
                    for (; xf > states[indices[i]].first + (states[indices[i]].second ? 1e-9 : -1e-9); indices[i]++);

                    auto [size, win] = states[indices[i]];
                    if (win && xf < size - 1e-9) {
                        x = size / f[i];
                        change = true;
                    }
                }
            } while (change);

            states.pop_back();
            states.emplace_back(x, false);
            states.emplace_back(1e3, true);

            for (int &i : indices)
                if (states[i].second) i++;

            x = states[indices[0]].first / f[0];
            for (int i = 1; i < k; i++) x = min(x, states[indices[i]].first / f[i]);

            states.pop_back();
            states.emplace_back(x, true);
            states.emplace_back(1e3, false);
        }

        cout << ((lower_bound(states.begin(), states.end(), pair<double, bool>(X - 1e-9, false))->second) ? "Mikael\n" : "Nils\n");
    }
}
