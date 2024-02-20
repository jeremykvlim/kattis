#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> a(1);
    int m;
    cin >> a[0] >> m;

    auto b = make_unique<bitset<(int) 2e8 + 1>>();
    b->set();
    (*b)[0] = (*b)[a[0]] = false;

    int n = 1;
    for (int prev = 0, d; (*b)[m]; prev = d, n++) {
        d = b->_Find_next(prev);
        (*b)[d] = false;
        if (a[n - 1] + d < b->size()) (*b)[a[n - 1] + d] = false;
        for (int ai : a)
            if (a[n - 1] + d - ai < b->size()) (*b)[a[n - 1] + d - ai] = false;

        a.emplace_back(a[n - 1] + d);
    }

    cout << n;
}
