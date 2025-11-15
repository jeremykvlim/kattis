#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;
    function<T(T, T)> f;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] = f(BIT[i], v);
    }

    T range_query(int i) {
        T v = numeric_limits<T>::max();
        for (; i; i &= i - 1) v = f(v, BIT[i]);
        return v;
    }

    FenwickTree(int n, function<T(T, T)> func) : BIT(n, numeric_limits<T>::max()), f(move(func)) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int p_max = 0;
    vector<int> p(n);
    for (int &pi : p) {
        cin >> pi;

        p_max = max(p_max, pi);
    }

    vector<int> score(n);
    FenwickTree<long long> fw(p_max + 2, [](auto x, auto y) { return min(x, y); });
    for (int i = n - 1; ~i; i--) {
        score[i] = fw.range_query(p[i] + 1) - 1;
        fw.update(p[i] + 1, i + 1);
    }

    for (int i = 0; i < n; i++)
        if (score[i] < 1) cout << "infinity\n";
        else cout << score[i] - i << "\n";
}
