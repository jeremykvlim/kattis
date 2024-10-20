#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T v) {
        for (; i < BIT.size(); i += i & -i) BIT[i] += v;
    }

    T pref_sum(int i) {
        T sum = 0;
        for (; i; i &= (i - 1)) sum += BIT[i];
        return sum;
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

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

    FenwickTree<int> fw(n);
    auto moves = 0LL;
    for (int i = 0; i < n; i++) {
        int s = fw.pref_sum(cards[i].second);
        moves += min(s, i - s);
        fw.update(cards[i].second + 1, 1);
    }

    cout << moves;
}
