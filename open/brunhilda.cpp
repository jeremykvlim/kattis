#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, q;
    cin >> m >> q;

    vector<int> p(m);
    for (int &pi : p) cin >> pi;

    vector<int> calls{0};
    while (calls.back() < 1e7) {
        int c = -1;
        for (int pi : p) c = max(c, calls.back() - (calls.back() % pi) + pi - 1);

        if (c <= calls.back()) break;
        calls.emplace_back(c);
    }

    while (q--) {
        int nj;
        cin >> nj;

        if (nj > calls.back()) cout << "oo\n";
        else cout << lower_bound(calls.begin(), calls.end(), nj) - calls.begin() << "\n";
    }
}
