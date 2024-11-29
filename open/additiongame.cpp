#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> pi(n), sigma(n), perm(n);
    for (int i = 0; i < n; i++) pi[i] = sigma[i] = perm[i] = i;

    int sum = 0;
    vector<int> a(n), indices(n);
    for (int i = 0; i < n; i++) {
        int aj;
        cin >> aj;

        sum = (sum + aj) % n;
        if (i == n - 1) break;

        for (int j = 0; j < n; j++) indices[perm[j]] = j;
        int j = indices[i], k = indices[i + 1];
        a[j] = aj;
        a[k] = (n - sum) % n;
        for (int _ = 0; _ < n; _++) {
            int s1 = (a[j] + j) % n, s2 = (a[k] + j) % n;
            if (s1 == sigma[j] || s2 == sigma[j] || s1 == sigma[k] || s2 == sigma[k]) {
                int s = (s1 == sigma[j] || s1 == sigma[k]) ? s1 : s2;
                if (s == s2) {
                    swap(a[j], a[k]);
                    swap(perm[j], perm[k]);
                }
                if (s == sigma[k]) swap(sigma[j], sigma[k]);
                break;
            }

            int l = (sigma[j] - a[j] + n) % n;
            swap(a[j], a[l]);
            swap(perm[j], perm[l]);
            swap(sigma[j], sigma[l]);
            swap(sigma[j], sigma[k]);
        }
    }

    if (sum % n) {
        cout << "impossible";
        exit(0);
    }

    for (int i = 0; i < n; i++) {
        indices[perm[i]] = i;
        pi[i] = (n - pi[i]) % n;
    }

    for (int i : indices) cout << (!pi[i] ? n : pi[i]) << " ";
    cout << "\n";
    for (int i : indices) cout << (!sigma[i] ? n : sigma[i]) << " ";
}
