#include <bits/stdc++.h>
using namespace std;

void update(int i, int value, vector<int> &fenwick) {
    for (; i < fenwick.size(); i += i & -i) fenwick[i] += value;
}

int pref_sum(int i, vector<int> &fenwick) {
    int sum = 0;
    for (; i; i &= (i - 1)) sum += fenwick[i];
    return sum;
}

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

    vector<int> fenwick(n + 1, 0);
    int a = INT_MAX;
    for (int i = n; i; i--)
        if (next[i] != -1) {
            if (pref_sum(next[i], fenwick) > 0) a = min(a, s[i]);

            update(i + 1, 1, fenwick);
            update(next[i] + 1, -1, fenwick);
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
