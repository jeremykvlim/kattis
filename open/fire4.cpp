#include <bits/stdc++.h>
using namespace std;

template <typename T>
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

    int n, m;
    cin >> n >> m;

    vector<int> s(n + 1), e(n + 1);
    map<int, int> compress;
    for (int i = 1; i <= n; i++) {
        cin >> s[i] >> e[i];

        compress[s[i]];
        compress[e[i]];
    }

    int count = 1;
    for (auto &[t, i] : compress) i = count++;

    for (int i = 1; i <= n; i++) {
        s[i] = compress[s[i]];
        e[i] = compress[e[i]];
    }

    vector<FenwickTree<int>> fws(__lg(n) + 1, FenwickTree<int>(2 * n + 1, [](int x, int y) {return max(x, y);}));
    for (int i = 1; i <= n; i++)
        if (s[i] < e[i]) fws[0].update(s[i], e[i]);

    for (int j = 1; j <= __lg(n); j++)
        for (int i = 1; i <= 2 * n; i++)
            fws[j].update(i, fws[j - 1].range_query(fws[j - 1].range_query(i)));

    int least = INT_MAX;
    for (int i = 1; i <= n; i++)
        if (s[i] > e[i]) {
            int r = e[i], curr = 2;
            for (int j = __lg(n); ~j; j--) {
                int l = fws[j].range_query(r);
                if (l >= s[i]) continue;
                
                r = l;
                curr += 1 << j;
            }

            if (fws[0].range_query(r) >= s[i]) least = min(least, curr);
        }

    cout << (least == INT_MAX ? -1 : least);
}
