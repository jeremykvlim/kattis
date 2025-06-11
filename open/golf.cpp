#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    unordered_map<string, int> indices;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        indices[s] = i;
    }

    vector<long long> scores(n, 0);
    tree<pair<long long, int>, null_type, less<>, rb_tree_tag, tree_order_statistics_node_update> ost;
    for (int i = 0; i < n; i++) ost.insert({0, i});
    while (q--) {
        char c;
        cin >> c;

        if (c == '!') {
            int k;
            cin >> k;

            while (k--) {
                string s;
                int delta;
                cin >> s >> delta;

                int i = indices[s];
                ost.erase({scores[i], i});
                scores[i] += delta;
                ost.insert({scores[i], i});
            }
        } else {
            string s;
            cin >> s;

            int i = indices[s];
            cout << ost.order_of_key({scores[i], -1}) + 1 << " " << scores[i] << "\n";
        }
    }
}
