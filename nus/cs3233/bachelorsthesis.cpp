#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    if (n == k) {
        tree<int, null_type, less_equal<>, rb_tree_tag, tree_order_statistics_node_update> t, t_rev;
        auto inversions = 0LL, inversions_rev = 0LL;

        reverse(a.begin(), a.end());
        auto a_rev = a;
        reverse(a_rev.begin(), a_rev.end());

        for (int ai : a) {
            t.insert(ai);
            inversions += t.order_of_key(ai);
        }

        for (int ai : a_rev) {
            t_rev.insert(ai);
            inversions_rev += t_rev.order_of_key(ai);
        }

        if (inversions <= inversions_rev) cout << inversions << "\n0";
        else cout << inversions_rev << "\n1\n" << string(n, '1');
        exit(0);
    }

    vector<pair<int, int>> A(n);
    for (int i = 0; i < n; i++) A[i] = {a[i], i};
    sort(A.begin(), A.end());

    vector<int> rank(n);
    for (int i = 0; i < n; i++) rank[A[i].second] = i;

    vector<string> ops;
    auto add = [&](bool rev) {
        for (int i = (rev ? n - 1 : 0); rev == i > k; (rev ? i-- : i++)) {
            string s(n, '0');
            vector<int> l, r;
            for (int j = 0; j < n; j++) {
                if (rev ? (rank[j] < k - 1 || rank[j] == i) : (rank[j] <= k && rank[j] != i)) {
                    l.emplace_back(rank[j]);
                    s[j] = '1';
                } else {
                    r.emplace_back(rank[j]);
                    s[j] = '0';
                }
            }

            if (rev) reverse(l.begin(), l.end());
            l.insert(l.end(), r.begin(), r.end());
            rank = l;
            ops.emplace_back(s);
            if (!rev) ops.emplace_back(string(k, '1') + string(n - k, '0'));
        }
    };
    add(true);
    add(false);

    cout << "0\n" << ops.size() << "\n";
    for (auto s : ops) cout << s << "\n";
}
