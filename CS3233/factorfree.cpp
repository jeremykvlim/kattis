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
    vector<int> spf(n + 1, 0);
    for (int i = 2; i <= n; i++)
        if (!spf[i])
            for (int j = i; j <= n; j += i) spf[j] = i;

    return spf;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> elements(n);
    for (int &a : elements) cin >> a;

    int biggest = *max_element(elements.begin(), elements.end());
    vector<int> spf = sieve(biggest), prev(biggest + 1, -1), left(n, -1), right(n, n), parent(n, 0);
    for (int i = 0; i < n; i++)
        for (int num = elements[i]; num > 1; num /= spf[num]) {
            if (prev[spf[num]] < i) {
                left[i] = max(left[i], prev[spf[num]]);
                if (prev[spf[num]] != -1) right[prev[spf[num]]] = min(right[prev[spf[num]]], i);
            }
            prev[spf[num]] = i;
        }

    if (dfs(0, n, parent, left, right))
        for (int p : parent) cout << p + 1 << " ";
    else cout << "impossible";
}
