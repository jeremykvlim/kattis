#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<__int128> pref_a(n + 1, 0);
    vector<vector<__int128>> pref_pow(4, vector<__int128>(n + 1, 0));
    deque<tuple<int, int, int>> update;
    for (auto i = 1LL; i <= n; i++) {
        pref_pow[3][i] = pref_pow[3][i - 1] + i * i * i;
        pref_pow[2][i] = pref_pow[2][i - 1] + i * i;
        pref_pow[1][i] = pref_pow[1][i - 1] + i;
        pref_pow[0][i] = pref_pow[0][i - 1] + 1;
    }

    while (m--) {
        int t, x, y;
        cin >> t >> x >> y;

        if (!t) {
            auto sum = pref_a[y] - pref_a[x - 1];
            for (auto [sign, i, j] : update) {
                int l = max(x, i), r = min(y, j);
                if (l > r) continue;

                sum += sign * (pref_pow[3][r] - pref_pow[3][l - 1]
                            + (pref_pow[2][r] - pref_pow[2][l - 1]) * (-3 * i + 6)
                            + (pref_pow[1][r] - pref_pow[1][l - 1]) * ((long long) 3 * i * i - 12 * i + 11)
                            + (pref_pow[0][r] - pref_pow[0][l - 1]) * ((long long) -i * i * i + (long long) 6 * i * i - 11 * i + 6));
            }

            cout << (long long) ((sum % MODULO + MODULO) % MODULO) << "\n";
        } else {
            update.emplace_back((t == 1 ? 1 : -1), x, y);

            if (update.size() >= (int) sqrt(n)) {
                vector<vector<__int128>> temp1(4, vector<__int128>(n + 2, 0));
                while (!update.empty()) {
                    auto [sign, i, j] = update.front();
                    update.pop_front();

                    for (int k = i, s = sign; k < j + 2; k += j + 1 - i, s *= -1) {
                        temp1[3][k] += s;
                        temp1[2][k] += s * (-3 * i + 6);
                        temp1[1][k] += s * ((long long) 3 * i * i - 12 * i + 11);
                        temp1[0][k] += s * ((long long) -i * i * i + (long long) 6 * i * i - 11 * i + 6);
                    }
                }

                for (auto &t : temp1)
                    for (int i = 1; i <= n; i++) t[i] += t[i - 1];

                vector<__int128> temp2(n + 1, 0);
                for (auto i = 1LL; i <= n; i++)
                    temp2[i] += temp1[3][i] * i * i * i
                              + temp1[2][i] * i * i
                              + temp1[1][i] * i
                              + temp1[0][i];

                for (int i = 1; i <= n; i++) temp2[i] += temp2[i - 1];
                for (int i = 1; i <= n; i++) pref_a[i] += temp2[i];
            }
        }
    }
}
