#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, z;
    string s, t;
    cin >> n >> m >> z >> s >> t;

    auto total = (long long) n * m;
    int k = max(n, m) + 1, size = ceil(sqrt(k)), blocks = (k + size - 1) / size;
    vector<int> b_l(blocks), b_r(blocks);
    for (int b = 0; b < blocks; b++) {
        b_l[b] = b * size + 1;
        b_r[b] = min(k, (b + 1) * size);
    }

    auto sum_of_sq = [&](long long n) {
        return n * (n + 1) * (2 * n + 1) / 6;
    };

    vector<long long> sum_i(blocks), sum_i_sq(blocks);
    for (int b = 0; b < blocks; b++) {
        sum_i[b] = (b_l[b] + b_r[b]) * (b_r[b] - b_l[b] + 1) / 2;
        sum_i_sq[b] = sum_of_sq(b_r[b]) - sum_of_sq(b_l[b] - 1);
    }

    pair<array<vector<long long>, 5>, set<int>> v{{vector<long long>(k + 2, 0), vector<long long>(blocks, 0), vector<long long>(blocks, 0),
                                                   vector<long long>(blocks, 0), vector<long long>(blocks, 0)}, set<int>()},
                                                h{{vector<long long>(k + 2, 0), vector<long long>(blocks, 0), vector<long long>(blocks, 0),
                                                   vector<long long>(blocks, 0), vector<long long>(blocks, 0)}, set<int>()};

    vector<long long> both(blocks, 0);

    auto block_id = [&](int i) {
        return (i - 1) / size;
    };

    auto add_pref = [&](int len, int c0, int c1, auto &a1, auto &a2) {
        int B = block_id(len);
        for (int b = 0; b < B; b++) {
            a1[3][b] += c0 * (b_r[b] - b_l[b] + 1) + c1 * sum_i[b];
            a1[4][b] += c0 * sum_i[b] + c1 * sum_i_sq[b];
            both[b] += c0 * a2[3][b] + c1 * a2[4][b];
            a1[1][b] += c0;
            a1[2][b] += c1;
        }

        for (int i = b_l[B]; i <= len; i++) {
            auto d = c0 + c1 * i;
            a1[0][i] += d;
            a1[3][B] += d;
            a1[4][B] += d * i;
            both[B] += d * (a2[0][i] + a2[1][B] + a2[2][B] * i);
        }
    };

    auto handle_boundary = [&](int len, int sgn, auto &a1, auto &a2) {
        if (len > 0) add_pref(len, sgn * (len + 1), -sgn, a1, a2);
    };

    auto handle_gap = [&](int l, int sgn, auto &a1, auto &a2) {
        if (l <= 0) return;
        int len = l / 2;
        if (len) add_pref(len, sgn * (l + 2), -2 * sgn, a1, a2);
        if (l & 1) {
            int i = len + 1, b = block_id(i);
            a1[0][i] += sgn;
            a1[3][b] += sgn;
            a1[4][b] += sgn * i;
            both[b] += sgn * (a2[0][i] + a2[1][b] + a2[2][b] * i);
        }
    };

    auto init = [&](string &s, auto &axis, auto &a2) {
        auto &[a1, indices] = axis;
        for (int i = 1; i <= s.size(); i++)
            if (s[i - 1] == '+') indices.emplace(i);
        if (indices.empty()) return;
        handle_boundary(*indices.begin() - 1, 1, a1, a2);
        handle_boundary(s.size() - *indices.rbegin(), 1, a1, a2);
        for (auto it = next(indices.begin()); it != indices.end(); it++) handle_gap(*it - *prev(it) - 1, 1, a1, a2);
    };
    init(s, v, h.first);
    init(t, h, v.first);

    while (z--) {
        char q;
        cin >> q;

        auto toggle = [&](int len, int i, auto &axis, auto &a2) {
            auto &[a1, indices] = axis;
            auto it = indices.find(i);
            if (it != indices.end()) {
                int l = it == indices.begin() ? 0 : *prev(it), r = next(it) == indices.end() ? 0 : *next(it);
                if (!l && !r) {
                    handle_boundary(i - 1, -1, a1, a2);
                    handle_boundary(len - i, -1, a1, a2);
                } else if (!l) {
                    handle_boundary(i - 1, -1, a1, a2);
                    handle_gap(r - i - 1, -1, a1, a2);
                    handle_boundary(r - 1, 1, a1, a2);
                } else if (!r) {
                    handle_boundary(len - i, -1, a1, a2);
                    handle_gap(i - l - 1, -1, a1, a2);
                    handle_boundary(len - l, 1, a1, a2);
                } else {
                    handle_gap(i - l - 1, -1, a1, a2);
                    handle_gap(r - i - 1, -1, a1, a2);
                    handle_gap(r - l - 1, 1, a1, a2);
                }
                indices.erase(it);
            } else {
                it = indices.lower_bound(i);
                int l = it == indices.begin() ? 0 : *prev(it), r = it == indices.end() ? 0 : *it;
                if (!l && !r) {
                    handle_boundary(i - 1, 1, a1, a2);
                    handle_boundary(len - i, 1, a1, a2);
                } else if (!l) {
                    handle_boundary(r - 1, -1, a1, a2);
                    handle_boundary(i - 1, 1, a1, a2);
                    handle_gap(r - i - 1, 1, a1, a2);
                } else if (!r) {
                    handle_boundary(len - l, -1, a1, a2);
                    handle_boundary(len - i, 1, a1, a2);
                    handle_gap(i - l - 1, 1, a1, a2);
                } else {
                    handle_gap(r - l - 1, -1, a1, a2);
                    handle_gap(i - l - 1, 1, a1, a2);
                    handle_gap(r - i - 1, 1, a1, a2);
                }
                indices.emplace(i);
            }
        };

        if (q == 'N') {
            int i;
            cin >> i;

            toggle(n, i, v, h.first);
        } else if (q == 'M') {
            int i;
            cin >> i;

            toggle(m, i, h, v.first);
        } else {
            long long c;
            cin >> c;

            long long v_extra = v.second.empty() ? n : 0, h_extra = h.second.empty() ? m : 0;
            auto calc_2D = [&](int i) {
                if (i > k) return v_extra * h_extra;

                int b = block_id(i);
                auto calc_1D = [&](auto &a) {
                    return a[0][i] + a[1][b] + a[2][b] * i;
                };
                return (calc_1D(v.first) + v_extra) * (calc_1D(h.first) + h_extra);
            };
            
            int i = 0;
            long long sum_v = 0, sum_h = 0, sum_both = 0, cost = 0;
            for (int b = 0; b < blocks; b++) {
                auto temp = sum_both + both[b] + v_extra * (sum_h + h.first[3][b]) + h_extra * (sum_v + v.first[3][b]) + (calc_2D(k + 1) - calc_2D(b_r[b] + 1)) * b_r[b];
                if (c >= temp) {
                    sum_v += v.first[3][b];
                    sum_h += h.first[3][b];
                    sum_both += both[b];
                    cost = temp;
                    i = b_r[b];
                    continue;
                }

                for (int j = b_l[b]; j <= b_r[b]; j++) {
                    auto v_geq = v.first[0][j] + v.first[1][b] + v.first[2][b] * j, h_geq = h.first[0][j] + h.first[1][b] + h.first[2][b] * j;
                    temp = sum_both + v_geq * h_geq + v_extra * (sum_h + h_geq) + h_extra * (sum_v + v_geq) + (calc_2D(k + 1) - calc_2D(j + 1)) * j;
                    if (c >= temp) {
                        sum_v += v_geq;
                        sum_h += h_geq;
                        sum_both += v_geq * h_geq;
                        cost = temp;
                        i = j;
                    } else goto done;
                }
            }
            done:;
            cout << total - calc_2D(i + 1) + min(calc_2D(i + 1) - calc_2D(i + 2), (c - cost) / (i + 1)) << "\n";
        }
    }
}
