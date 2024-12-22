#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, t;
    cin >> n >> t;

    pair<long long, int> q;
    auto &[d, m] = q;
    map<long long, pair<long long, int>> memo;
    auto query = [&](long long x) {
        if (memo.count(x)) {
            q = memo[x];
            return;
        } 

        cout << "? " << x << "\n" << flush;
        cin >> d >> m;
        
        memo[x] = q;
    };

    vector<long long> a(n + 1, 0);
    query(1);
    a[2] = d + 1;

    long long l = 0, r = 1e18, mid;
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        query(mid);

        if (a[2] >= mid + d) l = mid;
        else r = mid;
    }

    query(l);
    a[1] = 2 * l - a[2] + (m == 1);

    mt19937_64 rng(random_device{}());
    vector<long long> diffs;
    auto add = [&](auto x) {
        query(x);
        if (x - d > a[2]) diffs.emplace_back(x - d);
        if (x + d <= 1e18) diffs.emplace_back(x + d);
    };
    for (int _ = 0; _ < 100; _++) {
        auto x = rng() % (long long) 1e18;
        add(x);
    }
    for (int i = diffs.size() - 1; ~i; i--) {
        auto x = diffs[i];
        add(x);
    }
    sort(diffs.rbegin(), diffs.rend());

    auto prev = l;
    for (int i = 3; i <= n; i++) {
        auto curr = 0LL;

        l = max(prev, (a[i - 1] + a[i - 2]) / 2), r = 1e18;
        for (auto [x, p] : memo) {
            tie(d, m) = p;

            if (x - d > a[i - 2] && x - d < a[i - 1]) {
                a[i] = x + d;
                break;
            }

            if (x - d > a[i - 1]) r = min(r, (a[i - 2] + x + d + 1) / 2);
            else if (x - d > a[i - 2]) r = min(r, x - 1);
            else {
                curr = max(curr, x);
                l = curr + 1;
            }
        }
        if (a[i]) continue;

        while (!diffs.empty()) {
            auto diff = diffs.back(), x = (a[i - 2] + diff) / 2;
            
            query(x);
            if (x - d > a[i - 2] && diff > a[i - 1]) {
                r = min(r, x - 1);
                break;
            }

            curr = max(curr, x);
            l = max(l, x + 1);
            diffs.pop_back();
        }

        auto dnc = [&](auto &&self, auto l, auto r) {
            if (l > r) return;

            auto mid = l + (r - l) / 2;

            auto it = memo.lower_bound(mid + 1);
            if (it != memo.end()) {
                auto [x, p] = *it;
                if (x <= a[i - 2]) {
                    curr = max(curr, x);
                    return self(self, mid + 1, r);
                }
            }

            if (it != memo.begin()) {
                auto [x, p] = *--it;
                tie(d, m) = p;
                if (x - d > a[i - 2]) return self(self, l, mid - 1);
            }

            query(mid);
            auto x = mid - d;
            if (x > a[i - 2] && x < a[i - 1]) {
                a[i] = mid + d;
                prev = (a[i] + a[i - 2]) / 2;
                return;
            }

            if (x <= a[i - 2]) {
                curr = max(curr, mid);
                self(self, mid + 1, r);
            } else self(self, l, mid - 1);
        };
        dnc(dnc, l, r);

        if (a[i]) continue;

        a[i] = 2 * curr - a[i - 2];
        if (!curr) {
            curr = prev;
            a[i]++;
        } else {
            prev = curr;
            query(curr);
            a[i] += m - (a[i - 1] == curr + d) != 2;
        }
    }

    cout << "! ";
    for (int i = 1; i <= n; i++) cout << a[i] << " ";
    cout << flush;
}
