#include <bits/stdc++.h>
using namespace std;

int find(int p, vector<int> &sets) {
    return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, A, B;
    cin >> n >> A >> B;

    vector<int> l(n), r(n), t(n), sets(n);
    for (int i = 0; i < n; i++) cin >> l[i] >> r[i] >> t[i];
    iota(sets.begin(), sets.end(), 0);

    queue<tuple<int, int, int>> q;
    q.emplace(A, B, 0);
    while (!q.empty()) {
        auto [a, b, d] = q.front();
        q.pop();

        if (t[a] + t[b] == 1) {
            cout << d;
            exit(0);
        }

        int a_set = find(a, sets), b_set = find(b, sets);
        if (a_set != b_set) {
            sets[a_set] = sets[b_set];
            q.emplace(l[a], l[b], d + 1);
            q.emplace(r[a], r[b], d + 1);
        }
    }

    cout << "indistinguishable";
}
