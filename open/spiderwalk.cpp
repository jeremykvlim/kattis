#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct SqrtDecomposition {
    int size;
    vector<T> arr, blocks;

    SqrtDecomposition(int n, vector<T> arr) : size(ceil(sqrt(n))), blocks(ceil(sqrt(n)), 0), arr(arr) {
        for (int i = 0; i < arr.size(); i++) blocks[i / size] += arr[i];
    }

    void update(int l, int r, T v) {
        int b_l = l / size, b_r = r / size;
        if (b_l == b_r)
            for (int i = l; i <= r; i++) arr[i] += v;
        else {
            for (int i = l; i < b_l * size + size; i++) arr[i] += v;
            for (int i = b_l + 1; i < b_r; i++) blocks[i] += v;
            for (int i = b_r * size; i <= r; i++) arr[i] += v;
        }
    }

    T query(int i) {
        return arr[i] + blocks[i / size];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s;
    cin >> n >> m >> s;

    vector<pair<int, int>> edges;
    while (m--) {
        int d, t;
        cin >> d >> t;

        edges.emplace_back(d, t - 1);
    }
    sort(edges.rbegin(), edges.rend());

    SqrtDecomposition<int> sd(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) sd.update(i, i, min(abs(i - s + 1), n - abs(i - s + 1)));

    for (auto [d, t1] : edges) {
        auto index = [&](int i) {return (i + n) % n;};

        int t2 = (t1 + 1) % n, dp1 = sd.query(t1), dp2 = sd.query(t2);
        if (dp1 == dp2) continue;

        int dp = min(dp1, dp2), t = dp1 > dp2 ? t1 : t2, dir = dp1 > dp2 ? -1 : 1;
        sd.update(t1, t1, dp1 > dp2 ? -1 : 1);
        sd.update(t2, t2, dp1 > dp2 ? 1 : -1);
        if (dp > sd.query(index(t - 2 * dir))) sd.update(index(t - dir), index(t - dir), -1);

        int l = 0, r = n, mid;
        while (l + 1 < r) {
            mid = l + (r - l) / 2;

            if (sd.query(index(t + dir * mid)) <= dp + mid) r = mid;
            else l = mid;
        }

        if (l) {
            auto update = [&](int l, int r, int v) {
                if (l <= r) sd.update(l, r, v);
                else {
                    sd.update(0, r, v);
                    sd.update(l, n - 1, v);
                }
            };

            if (~dir) update(index(t + 1), index(t + l), -1);
            else update(index(t - l), index(t - 1), -1);
        }
    }
    for (int i = 0; i < n; i++) cout << sd.query(i) << "\n";
}
