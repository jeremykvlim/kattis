#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int &e : a) cin >> e;
    reverse(a.begin(), a.end());

    tree<int, null_type, less<>, rb_tree_tag, tree_order_statistics_node_update> t;
    auto inversions = 0LL;
    for (int e : a) {
        t.insert(e);
        inversions += t.order_of_key(e);
    }

    cout << inversions;
}
