#include <bits/stdc++.h>
using namespace std;

template <typename T>
vector<T> min_plus_convolution(const vector<T> &convex, const vector<T> &arbitrary){
    int da = convex.size(), db = arbitrary.size(), n = da + db - 1;

    vector<T> c(n, numeric_limits<T>::max() / 4);
    c[0] = convex[0] + arbitrary[0];
    vector<int> indices(n + 1, 0);
    indices[n] = db - 1;
    for (int k = bit_ceil((unsigned) n) >> 1; k; k >>= 1)
        for (int i = k; i < n; i += k << 1) {
            int l = i - k, r = min(i + k, n);
            indices[i] = indices[l];
            for (int j = indices[l]; j <= indices[r]; j++) {
                if (j <= i && i < j + da) {
                    T t = arbitrary[j] + convex[i - j];
                    if (c[i] > t) {
                        c[i] = t;
                        indices[i] = j;
                    }
                }
            }
        }
    return c;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<int>> jewels(301);
    while (n--) {
        int s, v;
        cin >> s >> v;

        jewels[s].emplace_back(v);
    }

    vector<long long> dp(k + 1, -1e18), temp(k + 1, -1e18);
    dp[0] = 0;
    for(int s = 1; s <= 300; s++) {
        int steal = min((int) jewels[s].size(), k / s);
        if (!steal) continue;

        sort(jewels[s].rbegin(), jewels[s].rend());
        vector<long long> pref(steal + 1, 0);
        for(int i = 0; i < steal; i++) pref[i + 1] = pref[i] + jewels[s][i];

        vector<long long> a(steal + 1);
        for (int i = 0; i <= steal; i++) a[i] = -pref[i];

        fill(temp.begin(), temp.end(), -1e18);
        for (int r = 0; r < s && r <= k; r++) {
            int size = (k - r) / s + 1;
            vector<long long> b(size);
            for (int i = 0; i < size; i++) b[i] = -dp[i * s + r];

            auto c = min_plus_convolution(a, b);
            for (int i = 0; i < size; i++) temp[i * s + r] = -c[i];
        }
        dp = temp;
    }

    for (int i = 1; i <= k; i++) {
        dp[i] = max(dp[i], dp[i - 1]);
        cout << dp[i] << " ";
    }
}
