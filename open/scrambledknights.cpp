#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    int m = n / 2;

    vector<int> a(n), b(n);
    for (int &ai : a) {
        cin >> ai;

        ai = ai > m;
    }
    for (int &bi : b) {
        cin >> bi;

        bi = bi > m;
    }

    int zeroes = 0;
    for (int i = 0; i < m; i++) zeroes += !(a[i] + a[i + m]);
    zeroes = min(zeroes, m / 4);

    vector<bool> used(n, false);
    array<int, 3> remaining{zeroes, m / 2 - 2 * zeroes, zeroes};
    for (int i = 0; i < m; i++)
        if (remaining[a[i] + a[i + m]]) {
            remaining[a[i] + a[i + m]]--;
            used[i] = used[i + m] = true;
        }

    vector<vector<int>> decrees(5);
    for (int i = 0; i < n; i++) decrees[!used[i]].emplace_back(i);
    for (int d = 0; d < 2; d++) {
        int sum = accumulate(decrees[d].begin(), decrees[d].end(), 0, [&](int s, int i) { return s + b[i]; });
        for (int i = 0; i < m; i++) b[decrees[d][i]] = (i >= m - sum);
    }

    fill(used.begin(), used.end(), false);
    for (int i = 0; i < n; i++) {
        if (b[i] != (i >= m)) {
            decrees[2].emplace_back(i);
            used[i] = true;
        }
        decrees[3 + (i >= m)].emplace_back(i);
    }
    for (int i = 0; decrees[2].size() < m; i++)
        if (!used[i]) decrees[2].emplace_back(i);

    cout << "5\n";
    for (auto &d : decrees) {
        for (int i : d) cout << i + 1 << " ";
        cout << "\n";
    }
}
