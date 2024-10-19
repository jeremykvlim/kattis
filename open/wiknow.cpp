#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T value) {
        for (; i < BIT.size(); i += i & -i) BIT[i] += value;
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

    vector<int> s(n + 1), first(n + 1, -1), prev(n + 1, -1), next(n + 1, -1);
    for (int i = 1; i <= n; i++) {
        cin >> s[i];

        if (first[s[i]] == -1) first[s[i]] = i;
        else next[prev[s[i]]] = i;

        prev[s[i]] = i;
    }

    FenwickTree<int> fw(n + 1);
    int a = INT_MAX;
    for (int i = n; i; i--)
        if (next[i] != -1) {
            if (fw.pref_sum(next[i])) a = min(a, s[i]);

            fw.update(i + 1, 1);
            fw.update(next[i] + 1, -1);
        }

    if (a == INT_MAX) {
        cout << -1;
        exit(0);
    }

    int b = INT_MAX, count = 1;
    vector<int> pref(n + 1, 0);
    for (int i = first[a]; i <= n; i++) {
        if (s[i] == a) {
            count++;
            continue;
        }

        if (pref[s[i]] && pref[s[i]] != count) b = min(b, s[i]);
        pref[s[i]] = count;
    }

    cout << a << " " << b;
}
