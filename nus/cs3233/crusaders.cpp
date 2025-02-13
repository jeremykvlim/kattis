#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int c, a;
    cin >> c >> a;

    vector<pair<int, int>> s(a);
    for (int i = 0; i < a; i++) {
        cin >> s[i].first;

        s[i].second = i;
    }

    vector<int> city(a);
    auto dnc = [&](auto &&self, int l, int r, vector<pair<int, int>> &s) {
        if (l + 1 == r || s.empty()) return;

        int m = l + (r - l) / 2, c;
        cout << "Q " << m << "\n" << flush;
        cin >> c;

        vector<pair<int, int>> sl, sr;
        for (auto [si, i] : s) {
            if (si < c) sl.emplace_back(si, i);
            else if (si > c) sr.emplace_back(si, i);

            if (si >= c) city[i] = m;
        }

        self(self, l, m, sl);
        self(self, m, r, sr);
    };
    dnc(dnc, 0, c + 1, s);

    cout << "A ";
    for (int p : city) cout << p << " ";
}
