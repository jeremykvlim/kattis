#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, x;
    cin >> n >> k >> x;

    auto sum = 0LL;
    vector<int> samples(n);
    for (int &s : samples) {
        cin >> s;

        sum += s;
    }
    sort(samples.rbegin(), samples.rend());
    auto ave_min = (double) (sum - accumulate(samples.begin(), samples.begin() + k, 0LL)) / (n - k);
    if (ave_min >= x) {
        cout << fixed << setprecision(4) << ave_min - x;
        exit(0);
    }
    sort(samples.begin(), samples.end());
    auto ave_max = (double) (sum - accumulate(samples.begin(), samples.begin() + k, 0LL)) / (n - k);
    if (ave_max <= x) {
        cout << fixed << setprecision(4) << x - ave_max;
        exit(0);
    }

    auto diff = abs((double) sum / n - x);
    auto dfs = [&](auto &&self, int i = 0, long long s = 0, int d = 0) {
        if (d > k) return;
        diff = min(diff, abs((double) (sum - s) / (n - d) - x));
        for (int j = i; j < n; j++) self(self, j + 1, s + samples[j], d + 1);
    };
    dfs(dfs);
    cout << fixed << setprecision(4) << diff;
}
