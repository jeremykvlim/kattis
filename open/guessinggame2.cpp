#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int p, n;
    cin >> p >> n;

    int k = bit_width((unsigned) bit_width((unsigned) n - 1) - 2) + 1, p2 = 1 << k, mod = bit_ceil((unsigned) n) >> 1;
    if (p == 1) {
        cout << k + 2 << "\n" << flush;
        for (int _ = 0; _ < n - p2; _++) cout << k + 2 << "\n";
        cout << flush;

        auto sum = (long long) n * (n - 1) / 2;
        vector<bool> seen(n);
        for (int _ = 0; _ < n - p2; _++) {
            int i;
            cin >> i;

            seen[i] = true;
            sum -= i;
        }

        vector<int> unseen, id(n);
        for (int i = 0; i < n; i++)
            if (!seen[i]) {
                unseen.emplace_back(i);
                id[i] = unseen.size() - 1;
            }

        for (int _ = 0; _ < p2 - 1; _++) {
            int i;
            cin >> i;

            seen[i] = true;
            i = id[i];
            int ai = 0;
            for (int m = 2;; ai++, m <<= 1) {
                int l = i & -m;
                if (any_of(unseen.begin() + l, unseen.begin() + l + m, [&](int j) { return !seen[j]; })) break;
            }
            if (!ai && ((sum & (mod - 1)) >> (i >> 1)) & 1) ai = k;
            cout << ai + 1 << "\n" << flush;
        }
    } else {
        int count = 0;
        vector<int> a(n);
        for (int &ai : a) {
            cin >> ai;
            ai--;

            count += ai != k + 1;
        }

        if (count == p2) {
            vector<int> unseen, id(n), rem(k);
            for (int i = 0; i < n; i++)
                if (a[i] != k + 1) {
                    unseen.emplace_back(i);
                    id[i] = unseen.size() - 1;
                    rem[a[i] %= k]++;
                }

            int aj = 0;
            while (rem[aj] == 1 << (k - 1 - aj)) aj++;

            int m = 1 << (aj + 1);
            for (int i = 0; i < p2; i++) {
                if (a[unseen[i]] <= aj) continue;
                int l = i & -m;
                for_each(unseen.begin() + l, unseen.begin() + l + m, [&](int j) { if (a[j] == aj) a[j] = -1; });
            }

            vector<int> s;
            for (int i = 0; i < p2; i++)
                if (a[unseen[i]] == aj) s.emplace_back(unseen[i]);
            int s1 = s[0], s2 = s.size() == 1 ? s1 : s[1];
            cout << s1 << " " << s2 << "\n" << flush;
        } else {
            int s1 = 0;
            for (int j = 0; int ai : a)
                if (!ai) j++;
                else if (ai == k) s1 |= 1 << j++;

            for (int i = 0; i < n; i++)
                if (a[i] != k + 1) s1 = (s1 - i) & (mod - 1);

            int s2 = s1 + mod;
            if (s2 >= n) s2 = s1;
            cout << s1 << " " << s2 << "\n" << flush;
        }
    }
}
