#include <bits/stdc++.h>
using namespace std;

void update(int i, int value, vector<int> &fenwick) {
    for (; i < fenwick.size(); i += i & -i) fenwick[i] += value;
}

int pref_sum(int i, vector<int> &fenwick) {
    int sum = 0;
    for (; i; i -= i & -i) sum += fenwick[i];
    return sum;
}

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
        int j = cards[i].second, l = pref_sum(j, fenwick);
        update(j + 1, 1, fenwick);
        moves += min(l, i - l);
    }

    cout << moves;
}
