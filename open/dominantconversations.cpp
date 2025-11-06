#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    vector<vector<int>> indices(k);
    for (int i = 0; i < n; i++) indices[a[i]].emplace_back(i);

    for (int ai = 0; ai < k; ai++) {
        int m = indices[ai].size();
        if (!m) {
            cout << "0\n";
            continue;
        }

        auto process = [&](int l, int r) -> long long {
            int len = r - l + 1,
                lz = min(l ? indices[ai][l] - indices[ai][l - 1] - 1 : indices[ai][0], len - 1),
                tz = min(r + 1 < m ? indices[ai][r + 1] - indices[ai][r] - 1 : n - indices[ai][r] - 1, len - 1);

            vector<int> deltas(lz, -1);
            for (int i = l; i <= r; i++) {
                deltas.emplace_back(1);
                if (i < r) deltas.insert(deltas.end(), indices[ai][i + 1] - indices[ai][i] - 1, -1);
            }
            deltas.insert(deltas.end(), tz, -1);

            auto total = 0LL;
            int sum = 0, p = 0, offset = deltas.size() + 1;
            vector<int> freq(2 * offset + 1, 0);
            freq[p + offset] = 1;
            for (int d : deltas)
                if (d == 1) {
                    total += sum + freq[p + offset];
                    sum += freq[p + offset];
                    p++;
                    freq[p + offset]++;
                } else {
                    total += sum - freq[p + offset - 1];
                    sum -= freq[p + offset - 1];
                    p--;
                    freq[p + offset]++;
                }
            return total;
        };

        vector<int> base(m), suff(m);
        for (int i = 0; i < m; i++) base[i] = 2 * (i + 1) - indices[ai][i];
        suff[m - 1] = base[m - 1];
        for (int i = m - 2; ~i; i--) suff[i] = max(suff[i + 1], base[i]);

        int l = 0, pref = INT_MAX;
        auto dominated = 0LL;
        for (int r = 0; r < m - 1; r++) {
            pref = min(pref, base[r]);
            if (pref > suff[r + 1]) {
                dominated += process(l, r);
                l = r + 1;
            }
        }
        cout << dominated + process(l, m - 1) << "\n";
    }
}
