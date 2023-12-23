#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    multiset<int> difficulties;
    while (n--) {
        int d;
        cin >> d;

        difficulties.emplace(d);
    }

    while (q--) {
        int t, d;
        cin >> t >> d;

        auto it = difficulties.upper_bound(d);
        if (it == (t == 1 ? difficulties.end() : difficulties.begin())) cout << "-1\n";
        else {
            cout << (t == 1 ? *it : *prev(it)) << "\n";
            difficulties.erase(t == 1 ? it : prev(it));
        }
    }
}
