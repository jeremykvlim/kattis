#include <bits/stdc++.h>
using namespace std;

void search(int l, int r, int &fee, vector<pair<int, int>> &s, vector<int> &city) {
    if (l > r || s.empty() || fee++ > 1500) return;

    int m = l + (r - l) / 2;
    cout << "Q " << m << "\n";
    cout.flush();

    int c;
    cin >> c;

    vector<pair<int, int>> left, right;
    for (auto &p : s) {
        if (p.first < c) left.emplace_back(p);
        else if (p.first > c) right.emplace_back(p);
        
        if (p.first >= c) city[p.second] = m;
    }

    search(l, m - 1, fee, left, city);
    search(m + 1, r, fee, right, city);
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

    vector<int> city(a + 1);
    int fee = 0;
    search(1, c, fee, s, city);

    cout << "A ";
    for (int i = 0; i < a; i++) cout << city[i] << " ";
    cout.flush();
}
