#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, c;
        cin >> n >> c;

        vector<long long> a(n);
        unordered_set<long long> seen;
        for (auto &ai : a) {
            cin >> ai;

            seen.emplace(ai);
        }

        vector<long long> diffs;
        for (int i = 0; i < n; i++)
            for (int len = 1; len <= c; len++) {
                int j = i + len;
                if (j < n) diffs.emplace_back(a[j] - a[i]);
            }
        sort(diffs.begin(), diffs.end());

        vector<long long> valid;
        int ratio = n / c, count = ratio - (n - ratio - 1) / c;
        for (int l = 0, r = 1; l < diffs.size(); l = r++) {
            for (; r < diffs.size() && diffs[l] == diffs[r]; r++);
            if (r - l >= count) valid.emplace_back(diffs[l]);
        }

        int len = 1;
        for (auto d : valid)
            for (auto ai : a) {
                if (ai >= d && seen.find(ai - d) != seen.end()) continue;
                int l = 1;
                for (auto sum = ai + d; sum <= a.back() && seen.count(sum); sum += d) l++;
                len = max(len, l);
            }
        cout << (len > ratio ? len : -1) << "\n";
    }
}
