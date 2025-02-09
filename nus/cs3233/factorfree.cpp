#include <bits/stdc++.h>
using namespace std;

vector<int> sieve(int n) {
    vector<int> lpf(n + 1, 0);
    for (int i = 2; i <= n; i++)
        if (!lpf[i])
            for (int j = i; j <= n; j += i) lpf[j] = i;

    return lpf;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;


    vector<int> a(n);
    int biggest = 0;
    for (int &ai : a) {
        cin >> ai;

        biggest = max(biggest, ai);
    }

    vector<int> lpf = sieve(biggest), prev(biggest + 1, -1), left(n, -1), right(n, n), parent(n, 0);
    for (int i = 0; i < n; i++)
        for (int ai = a[i]; ai > 1; ai /= lpf[ai]) {
            if (prev[lpf[ai]] < i) {
                left[i] = max(left[i], prev[lpf[ai]]);
                if (prev[lpf[ai]] != -1) right[prev[lpf[ai]]] = min(right[prev[lpf[ai]]], i);
            }
            prev[lpf[ai]] = i;
        }

    auto dfs = [&](auto &&self, int l, int r, int p = -1) {
        if (l + 1 > r) return true;
        for (int i = l, j = r - 1; i <= j; i++, j--) {
            parent[i] = p;
            if (left[i] < l && right[i] >= r) return self(self, l, i, i) && self(self, i + 1, r, i);
            parent[j] = p;
            if (left[j] < l && right[j] >= r) return self(self, l, j, j) && self(self, j + 1, r, j);
        }

        return false;
    };

    if (dfs(dfs, 0, n))
        for (int p : parent) cout << p + 1 << " ";
    else cout << "impossible";
}
