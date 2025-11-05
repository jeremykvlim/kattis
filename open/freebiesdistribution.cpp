#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    cin >> m;

    vector<int> b(m);
    for (int &bi : b) cin >> bi;
    sort(b.begin(), b.end());

    int x, y;
    cin >> x >> y;

    int k = b.size();
    vector<long long> diff(k);
    for (int i = 0; i < k; i++) diff[i] = x + abs(b[i] - y);

    int n;
    cin >> n;

    vector<int> count(k, 0);
    while (n--) {
        int r, c;
        cin >> r >> c;

        auto search = [&](int c) -> int {
            int i = lower_bound(b.begin(), b.end(), c) - b.begin();
            if (!i) return 0;
            if (i == k) return k - 1;
            if (b[i] + b[i - 1] >= 2 * c) return i - 1;
            return i;
        };

        int i = search(c);
        count[i] += r + abs(b[i] - c) <= diff[i];
    }
    cout << *min_element(count.begin(), count.end());
}
