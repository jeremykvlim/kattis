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

        vector<long long> array(n);
        unordered_set<long long> nums;
        for (auto &a : array) {
            cin >> a;

            nums.emplace(a);
        }

        mt19937 rng(random_device{}());
        uniform_int_distribution<int> dis;
        auto ans = -1LL;
        for (int i = 0; i < 200; i++) {
            int j = dis(rng) % n, offset;
            do {
                offset = dis(rng) % (2 * c + 1) - c;
            } while (!offset || j + offset < 0 || j + offset >= n);

            auto l = array[j], r = array[j], diff = abs(array[j + offset] - array[j]);
            while (nums.count(l - diff)) l -= diff;
            while (nums.count(r + diff)) r += diff;

            if ((r - l) / diff + 1 > n / c) ans = max(ans, (r - l) / diff + 1);
        }

        cout << ans << "\n";
    }
}
