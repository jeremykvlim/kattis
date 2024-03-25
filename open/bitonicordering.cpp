#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> cards(n);
    for (int i = 0; i < n; i++) {
        int c;
        cin >> c;

        cards[i] = {c, i};
    }
    sort(cards.rbegin(), cards.rend());

    int moves = 0;
    vector<int> fenwick(n, 0);
    for (int i = 0; i < n; i++) {
        int l = 0, j = cards[i].second;
        for (int k = j; k; k -= k & -k) l += fenwick[k];
        for (int k = j + 1; k < n; k += k & -k) fenwick[k]++;
        moves += min(l, i - l);
    }

    cout << moves;
}
