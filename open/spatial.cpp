#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<array<unsigned long long, 5>> observations;
    unordered_map<unsigned long long, int> pos;
    while (n--) {
        unsigned long long i, t;
        string s;
        cin >> i >> s >> t;

        if (s == "Start") {
            unsigned long long L;
            string X;
            cin >> X >> L;

            auto x = stoull(X, 0, 0);
            observations.push_back({i, t, 0, x, x + L - 1});
            pos[i] = observations.size() - 1;
        } else observations[pos[i]][2] = t;
    }

    while (q--) {
        string _;
        unsigned long long t1, t2;
        string X1, X2;
        cin >> _ >> t1 >> t2 >> X1 >> X2;

        auto x1 = stoull(X1, 0, 0), x2 = stoull(X2, 0, 0);
        vector<unsigned long long> visible;
        for (auto &[id, tl, tr, xl, xr] : observations)
            if (tl <= t2 && t1 <= tr && xl <= x2 && x1 <= xr) visible.emplace_back(id);
        sort(visible.begin(), visible.end());

        int k = visible.size();
        cout << k << " ";
        for (int i = 0; i < min(20, k); i++) cout << visible[i] << " ";
        cout << "\n";
    }
}
