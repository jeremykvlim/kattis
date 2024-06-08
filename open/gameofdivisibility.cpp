#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, k;
        cin >> n >> k;

        vector<int> kmod(k);
        while (n--) {
            long long a;
            cin >> a;

            kmod[a % k]++;
        }

        auto sum = 0LL, sum_half = 0LL;
        vector<int> odd;
        for (int i = 0; i < k; i++) {
            sum += i * kmod[i];
            sum_half += i * (kmod[i] / 2);
            if (kmod[i] & 1) odd.emplace_back(i);
        }

        if (!(sum % k) || odd.empty() || odd.size() > 2) cout << "DRAW\n";
        else if (!((sum_half + odd.front()) % k) || !((sum_half + odd.back()) % k)) cout << "FIRST\n";
        else if (!(sum_half % k) && odd.size() < 2) cout << "SECOND\n";
        else cout << "DRAW\n";
    }
}
