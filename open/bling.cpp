#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int d, b, f;
    vector<int> t(3);
    cin >> d >> b >> f >> t[0] >> t[1] >> t[2];

    vector<int> fruit(d + 4, 1), tree(d + 4, 0);
    for (int i = d - 1; ~i; i--) {
        fruit[i] = max(1, tree[i + 3]);
        tree[i] = 3 * fruit[i] + tree[i + 3];
    }

    auto base = 0LL;
    for (int i = 0; i < 3; i++) {
        base += (long long) max(0, t[i] - 4) * tree[i];
        t[i] = min(t[i], 4);
    }
    base += (long long) max(0, f - 16) * fruit[0];
    f = min(f, 16);

    auto index = [&](int bl, int n0, int n1, int n2, int e0, int e1, int e2) {
        return ((((((bl * 5 + n0) * 5 + n1) * 5 + n2) * 2 + e0) * 2 + e1) * 2 + e2);
    };

    int size = 161 * 5 * 5 * 5 * 2 * 2 * 2;
    vector<long long> dp(size, -1);
    dp[index(b / 100, t[0], t[1], t[2], 0, 0, 0)] = base;
    long long bling = 0;
    for (int day = 0; day < d; day++) {
        vector<long long> temp(size, -1);
        int r = day % 3, limit = 4 * (d - day - 1);
        for (int bl1 = 0; bl1 <= 160; bl1++)
            for (int n0 = 0; n0 <= 4; n0++)
                for (int n1 = 0; n1 <= 4; n1++)
                    for (int n2 = 0; n2 <= 4; n2++)
                        for (int e0 = 0; e0 <= 1; e0++)
                            for (int e1 = 0; e1 <= 1; e1++)
                                for (int e2 = 0; e2 <= 1; e2++) {
                                    auto p = dp[index(bl1, n0, n1, n2, e0, e1, e2)];
                                    if (!~p) continue;

                                    array<int, 3> normal{n0, n1, n2}, exotic{e0, e1, e2};
                                    int fruits_normal = 3 * normal[r] + (!day ? f : 0), fruits_exotic = 3 * exotic[r];
                                    for (int sold_n = 0; sold_n <= fruits_normal; sold_n++) {
                                        auto temp_normal = normal;
                                        int total_n = normal[r] + fruits_normal - sold_n;
                                        temp_normal[r] = min(4, total_n);
                                        auto profit_normal = p + (long long) max(0, total_n - 4) * fruit[day];
                                        for (int sold_e = 0; sold_e <= fruits_exotic; sold_e++) {
                                            auto temp_exotic = exotic;
                                            int total_e = exotic[r] + fruits_exotic - sold_e;
                                            temp_exotic[r] = min(1, total_e);
                                            auto profit_exotic = profit_normal + (long long) 5 * max(0, total_e - 1) * fruit[day];

                                            auto update = [&](int bl, long long profit) {
                                                profit += bl - min(bl, limit);
                                                if (!limit) bling = max(bling, profit);
                                                else {
                                                    int i = index(min(bl, limit), temp_normal[0], temp_normal[1], temp_normal[2], temp_exotic[0], temp_exotic[1], temp_exotic[2]);
                                                    temp[i] = max(temp[i], profit);
                                                }
                                            };

                                            int bl2 = bl1 + sold_n + 5 * sold_e;
                                            update(bl2, profit_exotic);
                                            if (bl2 >= 4) {
                                                update(bl2 + 1, profit_exotic);
                                                temp_exotic[r] = 1;
                                                update(bl2 - 4, profit_exotic + (total_e ? 5 * fruit[day] : 0));
                                            }
                                        }
                                    }
                                }

        dp = temp;
    }
    cout << 100 * bling + b % 100;
}