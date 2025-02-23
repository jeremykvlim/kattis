#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long a, b, c;
    cin >> a >> b >> c;

    vector<array<long long, 3>> raids;
    while (min({a, b, c})) {
        vector<pair<long long, int>> indices{{a, 0}, {b, 1}, {c, 2}};
        sort(indices.begin(), indices.end());

        for (auto i = indices[1].first / indices[0].first; i; i /= 2) {
            if (!indices[0].second) {
                if (indices[2 - (i & 1)].second == 1) b -= a;
                else c -= a;
                a *= 2;
            } else if (indices[0].second == 1) {
                if (!indices[2 - (i & 1)].second) a -= b;
                else c -= b;
                b *= 2;
            } else {
                if (!indices[2 - (i & 1)].second) a -= c;
                else b -= c;
                c *= 2;
            }
            raids.push_back({a, b, c});
        }
    }
    cout << raids.size() << "\n";
    for (auto [a, b, c] : raids) cout << a << " " << b << " " << c << "\n";
}
