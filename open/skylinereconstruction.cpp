#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int n = 2025;
    vector<int> queries(405);
    for (int q = 1; q <= 405; q++) {
        int r = 5 * q, l = max(1, r - 14);
        cout << "? " << l << " " << r << "\n" << flush;

        int c;
        cin >> c;

        if (!~c) exit(0);
        queries[q - 1] = c;
    }

    array<int, 5> w5;
    for (int i = 1; i <= 5; i++) w5[i - 1] = i * (6 - i);

    array<int, 10> w10;
    for (int i = 1; i <= 10; i++) w10[i - 1] = i * (11 - i);

    array<int, 15> w15;
    for (int i = 1; i <= 15; i++) w15[i - 1] = i * (16 - i);

    int bound = accumulate(w15.begin() + 10, w15.end(), 0);
    vector<int> masks_by_sum(bound + 1, -1);
    for (int mask = 0; mask < 1 << 5; mask++) {
        int s = 0;
        for (int k = 0; k < 5; k++)
            if ((mask >> k) & 1) s += w15[k + 10];
        masks_by_sum[s] = mask;
    }

    string s(n, '0');
    for (int mask = 0; mask < 1 << 10; mask++) {
        fill(s.begin(), s.end(), '0');
        for (int i = 0; i < 10; i++) s[i] += (mask >> i) & 1;

        int sum5 = 0;
        for (int i = 0; i < 5; i++)
            if (s[i] == '1') sum5 += w5[i];
        if (sum5 != queries[0]) continue;

        int sum10 = 0;
        for (int i = 0; i < 10; i++)
            if (s[i] == '1') sum10 += w10[i];
        if (sum10 != queries[1]) continue;

        int sum15 = 0;
        for (int i = 0; i < 10; i++)
            if (s[i] == '1') sum15 += w15[i];
        int sum = queries[2] - sum15;
        if (!(0 <= sum && sum <= bound) || !~masks_by_sum[sum]) continue;

        for (int k = 0; k < 5; k++) s[k + 10] += (masks_by_sum[sum] >> k) & 1;

        for (int q = 4; q <= 405; q++) {
            int r = 5 * q, l = r - 14;

            sum15 = 0;
            for (int i = 0; i < 10; i++)
                if (s[i + l - 1] == '1') sum15 += w15[i];

            sum = queries[q - 1] - sum15;
            if (!(0 <= sum && sum <= bound) || !~masks_by_sum[sum]) goto next;

            for (int k = 0; k < 5; k++) s[k + r - 5] += (masks_by_sum[sum] >> k) & 1;
        }

        cout << "! " << s;
        exit(0);
        next:;
    }
    cout << "! " << string(n, '0');
}
