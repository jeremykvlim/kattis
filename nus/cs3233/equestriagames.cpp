#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, c;
    cin >> n >> k >> c;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    auto temp = a;
    sort(temp.begin(), temp.end());
    temp.erase(unique(temp.begin(), temp.end()), temp.end());
    for (int &ai : a) ai = lower_bound(temp.begin(), temp.end(), ai) - temp.begin();

    a.resize(2 * n);
    for (int i = n; i < 2 * n; i++) a[i] = a[i - n];

    int m = 2 * n;
    vector<int> next(m + 2, m + 1), freq(temp.size(), 0);
    for (int l = 1, r = 1, count = 0; l <= m; l++) {
        for (; r <= m && count < c; r++)
            if (!freq[a[r - 1]]++) count++;

        if (count >= c) next[l] = max(r, l + k);
        else next[l] = m + 1;

        if (!--freq[a[l - 1]]) count--;
    }

    vector<int> block(m + 2, 0), jump(m + 2, m + 1), count(m + 2, 0);
    for (int i = 1; i <= m + 1; i++) block[i] = (i - 1) / ceil(sqrt(m));

    for (int i = m; i; i--)
        if (next[i] >= m + 1 || block[i] != block[next[i]]) {
            jump[i] = next[i];
            count[i] = 1;
        } else {
            jump[i] = jump[next[i]];
            count[i] = count[next[i]] + 1;
        }

    int sold = 0;
    for (int l = 1; l <= n; l++) {
        int i = l, s = 0;
        while (i <= l + n)
            if (jump[i] <= l + n) {
                s += count[i];
                i = jump[i];
            } else if (next[i] <= l + n) {
                s++;
                i = next[i];
            } else break;
        sold = max(sold, s);
    }
    cout << sold;
}