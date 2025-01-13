#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<int>> jewels(301);
    for (int i = 0; i < n; i++) {
        int s, v;
        cin >> s >> v;

        jewels[s].emplace_back(v);
    }
    for (int s = 1; s <= 300; s++) sort(jewels[s].rbegin(), jewels[s].rend());

    vector<long long> dp(k + 1, -1e18);
    dp[0] = 0;
    for (int s = 1; s <= 300; s++)
        if (!jewels[s].empty())
            for (int r = 0; r < s; r++)
                if (r <= k) {
                    int size = (k - r) / s + 1;
                    vector<long long> prev(size);
                    for (int i = 0; i < size; i++) prev[i] = dp[i * s + r];

                    int steal = min(size, (int) jewels[s].size() + 1);
                    vector<long long> pref(steal, 0);
                    for (int i = 1; i < steal; i++) pref[i] = pref[i - 1] + jewels[s][i - 1];

                    auto search = [&](int i, int j) {
                        int l = j - 1, r = i + steal - 1, mid;
                        while (l + 1 < r) {
                            mid = l + (r - l) / 2;

                            if (prev[i] + pref[mid - i] > prev[j] + pref[mid - j]) l = mid;
                            else r = mid;
                        }
                        return l;
                    };

                    vector<long long> temp(size, -1);
                    deque<int> mono_indices;
                    for (int i = 0; i < size; i++) {
                        if (prev[i] >= 0) {
                            while (mono_indices.size() > 1) {
                                if (search(mono_indices[mono_indices.size() - 2], mono_indices.back()) >=
                                    search(mono_indices.back(), i))
                                    mono_indices.pop_back();
                                else break;
                            }
                            mono_indices.emplace_back(i);
                        }
                        while (!mono_indices.empty() && mono_indices[0] <= i - steal) mono_indices.pop_front();
                        while (mono_indices.size() > 1 && 
			       prev[mono_indices[0]] + pref[i - mono_indices[0]] <= prev[mono_indices[1]] + pref[i - mono_indices[1]]) mono_indices.pop_front();
                        if (!mono_indices.empty()) temp[i] = prev[mono_indices[0]] + pref[i - mono_indices[0]];
                    }
                    for (int i = 0; i < size; i++) dp[i * s + r] = temp[i];
                }

    for (int i = 1; i <= k; i++) {
        dp[i] = max(dp[i], dp[i - 1]);
        cout << dp[i] << " ";
    }
}
