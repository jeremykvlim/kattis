#include <bits/stdc++.h>
using namespace std;

void dnc(int l, int r, int &fee, vector<pair<int, int>> &s, vector<int> &city) {
    if (l > r || s.empty() || fee++ > 1500) return;

    int m = l + (r - l) / 2;
    cout << "Q " << m << "\n" << flush;

    int c;
    cin >> c;

    vector<pair<int, int>> left, right;
    for (auto [si, i] : s) {
        if (si < c) left.emplace_back(si, i);
        else if (si > c) right.emplace_back(si, i);

        if (si >= c) city[i] = m;
    }

    dnc(l, m - 1, fee, left, city);
    dnc(m + 1, r, fee, right, city);
}

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
    int fee = 0;
    dnc(1, c, fee, s, city);

    cout << "A ";
    for (int p : city) cout << p << " ";
}
