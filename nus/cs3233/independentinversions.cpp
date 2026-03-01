#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, K;
    cin >> n >> K;

    vector<int> b(n);
    for (int &bi : b) cin >> bi;

    vector<int> p3(12, 1);
    for (int i = 1; i < 12; i++) p3[i] = p3[i - 1] * 3;

    int kl = K / 2, kr = K - kl, base_l = p3[kl], base_r = p3[kr];
    vector<int> quo(n), rem(n);
    for (int i = 0; i < n; i++) {
        quo[i] = i / base_l;
        rem[i] = i % base_l;
    }

    vector<vector<int>> other{{1, 2}, {0, 2}, {0, 1}};
    auto build = [&](int kh, int base) {
        vector<int> trits(kh);
        vector<vector<int>> diffs(base);
        for (int x = 0; x < base; x++) {
            for (int i = 0, temp = x; i < kh; i++, temp /= 3) trits[i] = temp % 3;
            for (int mask = 0; mask < 1 << kh; mask++) {
                int y = 0;
                for (int i = 0; i < kh; i++) y += p3[i] * other[trits[i]][(mask >> i) & 1];
                diffs[x].emplace_back(y);
            }
        }
        return diffs;
    };
    auto diffs_l = build(kl, base_l), diffs_r = build(kr, base_r);

    int c = 0;
    vector<int> count(n, 0), order(n), temp(n);
    iota(order.begin(), order.end(), 0);
    auto dnc = [&](auto self, int l, int r) -> void {
        if (l + 1 >= r) return;

        int m = l + (r - l) / 2;
        self(self, l, m);
        self(self, m, r);

        vector<int> undo;
        for (int lm = l, mr = m; mr < r; mr++) {
            int i = order[mr];
            for (; lm < m && b[i] < b[order[lm]]; lm++) {
                int j = order[lm];
                for (int x : diffs_l[rem[j]]) {
                    int pos = quo[j] * base_l + x;
                    if (!count[pos]) undo.emplace_back(pos);
                    count[pos]++;
                }
            }
            for (int x : diffs_r[quo[i]]) c += count[x * base_l + rem[i]];
        }
        for (int pos : undo) count[pos] = 0;

        int i = l, j = m, k = l;
        for (; i != m && j != r; k++)
            if (b[order[i]] > b[order[j]]) temp[k] = order[i++];
            else temp[k] = order[j++];
        for (; i < m; k++) temp[k] = order[i++];
        for (; j < r; k++) temp[k] = order[j++];
        copy(temp.begin() + l, temp.begin() + r, order.begin() + l);
    };
    dnc(dnc, 0, n);
    cout << c;
}
