#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<vector<int>> count(40, vector<int>(64, 0));
        int max_p3 = 0, max_p2 = 0;
        while (n--) {
            long long x;
            cin >> x;

            int p3 = 0, p2 = 0;
            while (!(x % 3)) {
                x /= 3;
                p3++;
            }
            while (!(x & 1)) {
                x >>= 1;
                p2++;
            }
            if (x > 1) break;

            count[p3][p2]++;
            max_p3 = max(max_p3, p3);
            max_p2 = max(max_p2, p2);
        }

        if (n != -1) {
            while (n--) {
                long long x;
                cin >> x;
            }
            cout << "NO\n";
            continue;
        }

        vector<int> p2_l(max_p3 + 1), p2_r(max_p3 + 1);
        int max_r = 0;
        for (int p3 = 0; p3 <= max_p3; p3++) {
            int l = 63, r = -1;
            for (int p2 = 0; p2 <= max_p2; p2++)
                if (count[p3][p2]) {
                    l = min(l, p2);
                    r = max(r, p2);
                }
            p2_l[p3] = l;
            p2_r[p3] = r;
            max_r = max(max_r, r);
        }

        vector<bool> dp((max_p3 + 1) * (max_r + 1) * (max_r + 1), false);
        auto dp_index = [&](int p3, int p2_i, int p2_j) -> int {
            return p3 * (max_r + 1) * (max_r + 1) + p2_i * (max_r + 1) + p2_j;
        };

        vector<int> temp(max_r + 1, 0);
        for (int p3 = 0; p3 <= max_p3; p3++)
            for (int p2_i = p2_l[p3]; p2_i <= p2_r[p3]; p2_i++)
                for (int p2_j = p2_l[p3]; p2_j <= p2_r[p3]; p2_j++) {
                    int l = p2_l[p3], r = p2_r[p3];
                    if (!count[p3][p2_i] || !count[p3][p2_j] ||
                        !(l <= p2_i && p2_i <= r) ||
                        !(l <= p2_j && p2_j <= r)) continue;

                    for (int p2 = l; p2 <= r; p2++) temp[p2] = count[p3][p2];

                    if (p2_i <= p2_j) {
                        for (int p2 = l; p2 < p2_i; p2++)
                            if (temp[p2]) {
                                if (temp[p2 + 1] > temp[p2]) temp[p2 + 1] -= temp[p2];
                                else goto done;
                            }

                        for (int p2 = p2_i; p2 < p2_j; p2++) {
                            if (temp[p2] > 0) temp[p2 + 1] -= temp[p2] - 1;
                            else goto done;
                        }

                        for (int p2 = r; p2 > p2_j; p2--)
                            if (temp[p2]) {
                                if (temp[p2 - 1] > temp[p2]) temp[p2 - 1] -= temp[p2];
                                else goto done;
                            }

                    } else {
                        for (int p2 = r; p2 > p2_i; p2--)
                            if (temp[p2]) {
                                if (temp[p2 - 1] > temp[p2]) temp[p2 - 1] -= temp[p2];
                                else goto done;
                            }

                        for (int p2 = p2_i; p2 > p2_j; p2--) {
                            if (temp[p2] > 0) temp[p2 - 1] -= temp[p2] - 1;
                            else goto done;
                        }

                        for (int p2 = l; p2 < p2_j; p2++)
                            if (temp[p2]) {
                                if (temp[p2 + 1] > temp[p2]) temp[p2 + 1] -= temp[p2];
                                else goto done;
                            }
                    }

                    dp[dp_index(p3, p2_i, p2_j)] = temp[p2_j] == 1;
                    done:;
                }

        vector<int> prev((max_p3 + 1) * (max_p2 + 1), -1);
        auto prev_index = [&](int p3, int p2) {
            return p3 * (max_p2 + 1) + p2;
        };

        for (int i = 0; i <= max_p2; i++)
            if (dp[dp_index(0, 0, i)]) prev[prev_index(0, i)]++;

        for (int i = 1; i <= max_p3; i++)
            for (int j = 0; j <= max_p2; j++)
                for (int k = 0; k <= max_p2; k++)
                    if (~prev[prev_index(i - 1, k)] && dp[dp_index(i, k, j)]) {
                        prev[prev_index(i, j)] = k;
                        break;
                    }

        for (int p2_j = 0; p2_j <= max_p2; p2_j++) {
            if (~prev[prev_index(max_p3, p2_j)]) {
                cout << "YES\n";
                vector<long long> a;
                for (int p3 = max_p3; ~p3; p3--) {
                    int p2_i = prev[prev_index(p3, p2_j)], l = p2_l[p3], r = p2_r[p3];
                    for (int p2 = l; p2 <= r; p2++) temp[p2] = count[p3][p2];

                    auto calc = [&](int p3, int p2) {
                        auto a = 1LL;
                        while (p2--) a *= 2;
                        while (p3--) a *= 3;
                        return a;
                    };

                    vector<long long> seq;
                    if (p2_i <= p2_j) {
                        for (int p2 = p2_i; p2 > l; p2--) {
                            temp[p2]--;
                            seq.emplace_back(calc(p3, p2));
                        }

                        for (int p2 = l; p2 < p2_j; p2++) {
                            while (temp[p2] > 1) {
                                seq.emplace_back(calc(p3, p2));
                                seq.emplace_back(calc(p3, p2 + 1)), temp[p2 + 1]--;
                                temp[p2]--;
                            }
                            seq.emplace_back(calc(p3, p2));
                        }

                        for (int p2 = p2_j; p2 < r; p2++) {
                            temp[p2]--;
                            seq.emplace_back(calc(p3, p2));
                        }

                        for (int p2 = r; p2 > p2_j; p2--) {
                            while (temp[p2] > 1) {
                                seq.emplace_back(calc(p3, p2));
                                seq.emplace_back(calc(p3, p2 - 1)), temp[p2 - 1]--;
                                temp[p2]--;
                            }
                            seq.emplace_back(calc(p3, p2));
                        }

                        seq.emplace_back(calc(p3, p2_j));
                    } else {
                        for (int p2 = p2_i; p2 < r; p2++) {
                            temp[p2]--;
                            seq.emplace_back(calc(p3, p2));
                        }

                        for (int p2 = r; p2 > p2_j; p2--) {
                            while (temp[p2] > 1) {
                                seq.emplace_back(calc(p3, p2));
                                seq.emplace_back(calc(p3, p2 - 1)), temp[p2 - 1]--;
                                temp[p2]--;
                            }
                            seq.emplace_back(calc(p3, p2));
                        }

                        for (int p2 = p2_j; p2 > l; p2--) {
                            temp[p2]--;
                            seq.emplace_back(calc(p3, p2));
                        }

                        for (int p2 = l; p2 < p2_j; p2++) {
                            while (temp[p2] > 1) {
                                seq.emplace_back(calc(p3, p2));
                                seq.emplace_back(calc(p3, p2 + 1)), temp[p2 + 1]--;
                                temp[p2]--;
                            }
                            seq.emplace_back(calc(p3, p2));
                        }

                        seq.emplace_back(calc(p3, p2_j));
                    }
                    reverse(seq.begin(), seq.end());
                    for (auto ai : seq) a.emplace_back(ai);
                    p2_j = p2_i;
                }
                reverse(a.begin(), a.end());
                for (auto ai : a) cout << ai << " ";
                cout << "\n";
                goto next;
            }
        }

        cout << "NO\n";
        next:;
    }
}
