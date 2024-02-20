#include <bits/stdc++.h>
using namespace std;

bool dfs(int l, int r, vector<int> &prev, vector<int> &left, vector<int> &right, int p = -1) {
    if (l + 1 > r) return true;
    
    for (int i = l, j = r - 1; i <= j; i++, j--) {
        prev[i] = p;
        if (left[i] < l && right[i] >= r) return dfs(l, i, prev, left, right, i) && dfs(i + 1, r, prev, left, right, i);
        
        prev[j] = p;
        if (left[j] < l && right[j] >= r) return dfs(l, j, prev, left, right, j) && dfs(j + 1, r, prev, left, right, j);
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
    vector<int> lpf = sieve(biggest), prev(biggest + 1, -1), left(n, -1), right(n, n), prev(n, 0);
    for (int i = 0; i < n; i++)
        for (int num = elements[i]; num > 1; num /= lpf[num]) {
            if (prev[lpf[num]] < i) {
                left[i] = max(left[i], prev[lpf[num]]);
                if (prev[lpf[num]] != -1) right[prev[lpf[num]]] = min(right[prev[lpf[num]]], i);
            }
            prev[lpf[num]] = i;
        }

    if (dfs(0, n, prev, left, right))
        for (int p : prev) cout << p + 1 << " ";
    else cout << "impossible";
}
