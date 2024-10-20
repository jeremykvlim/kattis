#include <bits/stdc++.h>
using namespace std;

template<typename T>
struct FenwickTree {
    vector<T> BIT;
    function<T(T, T)> f;

    void update(int i, T v) {
        for (; i < BIT.size(); i += i & -i) BIT[i] = f(BIT[i], v);
    }

    T range_query(int i) {
        T v = 0;
        for (; i; i &= (i - 1)) value = f(v, BIT[i]);
        return v;
    }

    FenwickTree(int n, function<T(T, T)> func) : BIT(n, 0), f(move(func)) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<deque<int>> indices(n + 1);
    for (int i = 0; i < n * k; i++) {
        int xi;
        cin >> xi;

        indices[xi].emplace_front(i);
    }

    FenwickTree<int> fw(n * k + 1, [](int x, int y) {return max(x, y);});
    for (int i = 0; i < n * k; i++) {
        int xi;
        cin >> xi;

        for (int j : indices[xi]) fw.update(j + 1, fw.range_query(j) + 1);
    }

    cout << fw.range_query(n * k);
}
