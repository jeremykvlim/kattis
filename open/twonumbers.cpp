#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    auto sum = 0LL;
    vector<long long> b(n);
    for (auto &bi : b) {
        cin >> bi;

        sum += bi;
    }
    sort(b.begin(), b.end());

    int a;
    double m;
    cin >> a >> m;

    int mid = n / 2;
    auto prev = n & 1 ? 2 * b[mid] : b[mid - 1] + b[mid], s = 2 * sum / n - a * (n - 2);
    vector<pair<long long, long long>> pairs;
    for (int i = 0, j = n - 1; i < j;) {
        if (b[i] + b[j] < s) i++;
        else if (b[i] + b[j] > s) j--;
        else {
            auto index = [&](int k) {
                return k + (i <= k) + (j <= k + 1);
            };

            auto curr = n & 1 ? 2 * b[index(mid - 1)] : b[index(mid - 2)] + b[index(mid - 1)];
            if ((double) (curr - prev) / 2 == m) pairs.emplace_back(b[i], b[j]);
            i++;
            j--;
        }
    }
    sort(pairs.begin(), pairs.end());
    for (auto [l, r] : pairs) cout << l << " " << r << "\n";
}
