#include <bits/stdc++.h>
using namespace std;

bool dfs(int l, int r, vector<int> &parent, vector<int> &left, vector<int> &right, int p = -1) {
    if (l + 1 > r) return true;
    for (int i = l, j = r - 1; i <= j; i++, j--) {
        parent[i] = p;
        if (left[i] < l && right[i] >= r) return dfs(l, i, parent, left, right, i) && dfs(i + 1, r, parent, left, right, i);
        parent[j] = p;
        if (left[j] < l && right[j] >= r) return dfs(l, j, parent, left, right, j) && dfs(j + 1, r, parent, left, right, j);
    }

    return false;
}

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

    vector<int> elements(n);
    for (int &a : elements) cin >> a;

    int biggest = *max_element(elements.begin(), elements.end());
    vector<int> lpf = sieve(biggest), prev(biggest + 1, -1), left(n, -1), right(n, n), parent(n, 0);
    for (int i = 0; i < n; i++) 
        for (int e = elements[i]; e > 1; e /= lpf[e]) {
            if (prev[lpf[e]] < i) {
                left[i] = max(left[i], prev[lpf[e]]);
                if (prev[lpf[e]] != -1) right[prev[lpf[e]]] = min(right[prev[lpf[e]]], i);
            }
            prev[lpf[e]] = i;
        }

    if (dfs(0, n, parent, left, right))
        for (int p : parent) cout << p + 1 << " ";
    else cout << "impossible";
}
